#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import os
import os.path
import sys
import re
import subprocess
import shutil
import json

# command line arguments
use_gdb = '--gdb' in sys.argv

# ignore dirs regex
ignore_dirs = re.compile(r"(.*)\.(act|expected)")

# match `*.orig.cpp`
orig_re = re.compile(r"(.*)\.orig")

# refactor-cpp path
refactorbin = os.path.abspath('../bin/refactor-cpp')

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

def print_test_result(name, result, reason = None):
  if result:
    Style.print(
      Style.Passed,
      u'✓ %s passed' % name)
  else:
    Style.print(
      Style.Fail,
      u'✗ %s failed: %s' % (name, reason))

  return result

def cmd_print(msg):
  Style.print(Style.Cmd, msg)
  sys.stdout.flush()

def process_test(root, name):
  test_name = os.path.join(root, name)
  origdir = name + '.orig'
  origpath = os.path.join(root, origdir)
  actdir = name + '.act'
  actpath = os.path.join(root, actdir)
  buildpath = os.path.join(actpath, 'build')
  ccjson = os.path.join(actpath, 'compile_commands.json')
  expdir = name + '.expected'
  exppath = os.path.join(root, expdir)
  configpath = os.path.join(origpath, 'test_config.json')

  try:
    Style.print(Style.Start, u'🚀 %s' % test_name)

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
      return print_test_result(test_name, False,
        'Failed to generate `compile_commands.json`.')

    # search corresponding *.expected
    if not os.path.isdir(exppath):
      return print_test_result(test_name, False, "%s does not exist." % expdir)

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
    return print_test_result(
            test_name,
            result == 0,
            'Got not expected result.')

  except:
    return print_test_result(test_name, False,
      "internal error: %s." % sys.exc_info()[1])

  finally:
    #shutil.rmtree(actpath)
    pass

num_tests = 0
failed_tests = 0
os.chdir('../..')

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

    num_tests = num_tests + 1

    if not process_test(root, m.group(1)):
      failed_tests = failed_tests + 1

  for d in remove_dirs:
    dirs.remove(d)


print(u'%d tests (%d failed)' % (num_tests, failed_tests))
exit(failed_tests)

