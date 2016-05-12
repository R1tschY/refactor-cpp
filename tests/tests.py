#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import os
import os.path
import sys
import re
import subprocess
import shutil
import json

# TODO
# parallel tests
# clean up / refactor
# *.act in CMAKE_BINARY_DIR
# boost tests
# choose tests by regex
# simplelinecheck: check every line without: check-line/modify-line againt original

# command line arguments
use_gdb = '--gdb' in sys.argv

# ignore dirs regex
ignore_dirs = re.compile(r"(.*)\.(act|expected)$")

# match `*.orig`
orig_re = re.compile(r"(.*)\.orig$")

# match simple check files
check_re = re.compile(r"(.*)\.orig\.cpp$")

# refactor-cpp path
refactorbin = os.path.abspath('../bin/refactor-cpp')

class TestResult:
  def print(self, name):
    pass
    
class PassedTest(TestResult):
  def __init__(self):
    self.failed = False
    
  def print(self, name):
    Style.print(Style.Passed, u'✓ %s passed' % name)
    
class FailedTest(TestResult):
  def __init__(self):
    self.failed = True

  def print(self, name):
    Style.print(Style.Fail, u'✗ %s failed' % (name))
    
class UnresolvedTest(TestResult):
  def __init__(self, reason):
    self.reason = reason
    self.failed = True
  
  def print(self, name):
    Style.print(Style.Fail, u'☠ %s failed: %s' % (name, self.reason))

class Test:
  def __init__(self, name):
    self.name = name

  def process(self):
    return TestResult.Unresolved

class Style:
  Passed = '\033[32m'
  Fail = '\033[31m'
  Cmd = '\033[33m'
  Start = '\033[34m'
  Normal = '\033[0m'

  def print(color, msg):
    print(color + msg + Style.Normal)

def generate_compile_commands(folder, sources, args):
  absfolder = os.path.abspath(folder)
  result = []
  for source in sources:
    result.append({
      'directory': absfolder,
      'command': '/usr/bin/c++ ' + args + ' -o ' + source + '.o -c ' + source,
      'file': os.path.join(absfolder, source)
    })
  f = open(os.path.join(absfolder, 'compile_commands.json'), 'w')
  json.dump(result, f)
  f.close()

def ignore_directory(name):
  return re.match(ignore_dirs, name) != None

def cmd_print(msg):
  Style.print(Style.Cmd, msg)
  sys.stdout.flush()
  
  
class CheckLinesTest(Test):

  def __init__(self, root, name):
    self.root = root
    self.name = name      
    
    self.check_re = re.compile(r"(.*)// *CHECK-LINE:(.*)")
    self.run_re = re.compile(r"// *RUN:(.*)")

  def process(self):
    origfile = os.path.join(self.root, self.name + '.orig.cpp')
    actfile = os.path.join(self.root, self.name + '.act.cpp')
    
    # create *.act.cpp
    if os.path.exists(actfile):
      os.remove(actfile)    
    shutil.copyfile(origfile, actfile)
  
    f = open(origfile)
    c = f.read()
    f.close()
    
    m = re.search(self.run_re, c)
    if m == None:
      return UnresolvedTest("no RUN comment")
    
    cmd = m.group(1) % (refactorbin, actfile)
    out = subprocess.check_output(cmd, shell=True) # TODO check error code
    
    f = open(actfile)
    c = f.read()
    f.close()
      
    checks = re.findall(self.check_re, c)
    if len(checks) == 0:
      return UnresolvedTest("no CHECK-LINE comment")
      
    for check in checks:
      if check[0].strip() != check[1].strip():
        print("`%s` != `%s`" % (check[0].strip(), check[1].strip()))
        return FailedTest()
    
    return PassedTest()
        

class ComplexTest(Test):

    def __init__(self, root, name):
        self.root = root
        self.name = name

    def process(self):
        test_name = os.path.join(self.root, self.name)
        origdir = self.name + '.orig'
        origpath = os.path.join(self.root, origdir)
        actdir = self.name + '.act'
        actpath = os.path.join(self.root, actdir)
        buildpath = os.path.join(actpath, 'build')
        ccjson = os.path.join(actpath, 'compile_commands.json')
        expdir = self.name + '.expected'
        exppath = os.path.join(self.root, expdir)
        configpath = os.path.join(origpath, 'test_config.json')

        try:
          # copy *.orig to *.act
          if os.path.isdir(actpath):
            cmd_print(u'☠ remove existing working directory')
            shutil.rmtree(actpath)

          cmd_print(u'⚙ create working directory')
          shutil.copytree(origpath, actpath,
            ignore=shutil.ignore_patterns(
                '*~', 'build', '*.json'
            )
          )

          # read test config
          with open(configpath, encoding='utf-8') as data_file:
            config = json.loads(data_file.read())

          arguments = config['arguments']
          sources = config['sources']
          cc = config['compile commands']

          # generate compile_commands.json
          cmd_print(u'⚙ generate compile_commands.json')
          generate_compile_commands(actpath, sources, cc)
          if not os.path.isfile(ccjson):
            return UnresolvedTest('Failed to generate `compile_commands.json`.')

          # search corresponding *.expected
          if not os.path.isdir(exppath):
            return UnresolvedTest("%s does not exist." % expdir)

          # build command line
          arguments.insert(0, refactorbin)
          arguments.extend([
            '-p', os.path.abspath(actpath),
            '-extra-arg=-I/usr/lib/clang/3.6/include' # FIXME
          ])
          arguments.extend(sources)

          if use_gdb == True:
              arguments.insert(0, 'gdb')
              arguments.insert(1, '--args')

          # make refactoring
          cmd_print(u'🚜 make refactoring')
          subprocess.check_call(
            arguments,
            cwd = actpath,
            stdout = sys.stdout,
            stderr = sys.stderr
          )

          # check differences between *.act and *.expected
          cmd_print(u'🔁 check differences')
          result = subprocess.call(
            [
              'diff', '-wupr',
              '--exclude=build',
              '--exclude=*~',
              '--exclude=*.json',
              actpath, exppath
            ],
            stdout = sys.stdout,
            stderr = sys.stderr
          )
          return PassedTest() if result == 0 else FailedTest()

        except:
          return UnresolvedTest("internal error: %s." % sys.exc_info()[1])

        finally:
          #shutil.rmtree(actpath)
          pass

os.chdir('../..')

tests = []

# walk testfiles directory tree
for root, dirs, files in os.walk('tests/cli'):
  # walk all files
  remove_dirs = []
  for d in dirs:
    # ignore dirs
    if ignore_directory(d):
      remove_dirs.append(d)
      continue

    # match *.orig
    m = re.match(orig_re, d)
    if m == None:
      continue
    remove_dirs.append(d)

    tests.append(ComplexTest(root, m.group(1)))

  for d in remove_dirs:
    dirs.remove(d)
    
  for f in files:
    m = re.match(check_re, f)
    if m != None:
      tests.append(CheckLinesTest(root, m.group(1)))

num_tests = len(tests)
failed_tests = 0
for test in tests:
  Style.print(Style.Start, u'🚀  %s' % test.name)
  r = test.process()
  r.print(test.name)
  if r.failed:
    failed_tests = failed_tests + 1
  

print(u'%d tests (%d failed)' % (num_tests, failed_tests))
exit(failed_tests)

