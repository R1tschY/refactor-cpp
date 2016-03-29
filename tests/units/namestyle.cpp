#include <boost/test/unit_test.hpp>

#include <initializer_list>
#include <vector>

#include "utils/container.h"
#include "utils/stringgenerator.h"
#include <utils/algorithm.h>
#include "namestyle.h"

using namespace Refactor;
using namespace Refactor::UnitTests;

BOOST_AUTO_TEST_SUITE(NameStyle_Tests)

// CPP_NARG

BOOST_AUTO_TEST_CASE(NameStyle_GeneralId_CamelCase)
{
  CHECK_CONTAINERS(list({"My", "Class"}), splitIdentifer("MyClass"));
  CHECK_CONTAINERS(list({"my", "Class"}), splitIdentifer("myClass"));
  CHECK_CONTAINERS(list({"my", "Class2"}), splitIdentifer("myClass2"));
  CHECK_CONTAINERS(list({"my2", "Class"}), splitIdentifer("my2Class"));
  CHECK_CONTAINERS(
    list({"my", "Func", "With", "Many", "Args"}),
    splitIdentifer("myFuncWithManyArgs")
  );

  // real world examples
  CHECK_CONTAINERS(list({"Vec3f"}), splitIdentifer("Vec3f"));
}

BOOST_AUTO_TEST_CASE(NameStyle_GeneralId_LowerCase)
{
  CHECK_CONTAINERS(list({"my", "class"}), splitIdentifer("my_class"));
  CHECK_CONTAINERS(list({"my", "class2"}), splitIdentifer("my_class2"));
  CHECK_CONTAINERS(
    list({"my", "func", "with", "many", "args"}),
    splitIdentifer("my_func_with_many_args")
  );
}

BOOST_AUTO_TEST_CASE(NameStyle_GeneralId_UpperCase)
{
  CHECK_CONTAINERS(list({"MY", "MACRO"}), splitIdentifer("MY_MACRO"));
  CHECK_CONTAINERS(list({"MY", "2", "MACRO"}), splitIdentifer("MY_2_MACRO"));
  CHECK_CONTAINERS(
    list({"MY", "FUNC", "WITH", "MANY", "ARGS"}),
    splitIdentifer("MY_FUNC_WITH_MANY_ARGS")
  );
}

BOOST_AUTO_TEST_CASE(NameStyle_GeneralId_MixedCase)
{
  CHECK_CONTAINERS(list({"my", "Class", "with", "ARGS"}), splitIdentifer("my_Class_with_ARGS"));
  CHECK_CONTAINERS(list({"my", "Class", "with", "ARGS"}), splitIdentifer("myClass_with_ARGS"));
  CHECK_CONTAINERS(list({"my", "Class", "with", "ARGS"}), splitIdentifer("my_Class_withARGS"));
  CHECK_CONTAINERS(list({"my", "Class", "with", "ARGS"}), splitIdentifer("myClass_withARGS"));

  CHECK_CONTAINERS(list({"MY", "Base64", "Impl"}), splitIdentifer("MYBase64Impl"));
  CHECK_CONTAINERS(list({"my", "Base64", "Impl"}), splitIdentifer("myBase64Impl"));
  CHECK_CONTAINERS(list({"My", "Base64", "Impl"}), splitIdentifer("MyBase64Impl"));
  CHECK_CONTAINERS(list({"my", "Base64", "IMPL"}), splitIdentifer("myBase64IMPL"));
  CHECK_CONTAINERS(list({"my", "BASE64", "Impl"}), splitIdentifer("myBASE64Impl"));
  CHECK_CONTAINERS(list({"my", "BASE64", "impl"}), splitIdentifer("myBASE64impl"));
}

BOOST_AUTO_TEST_CASE(NameStyle_GeneralId_PreSufffix)
{
  CHECK_CONTAINERS(list({"my", "Class"}), splitIdentifer("myClass__"));
  CHECK_CONTAINERS(list({"my", "Class"}), splitIdentifer("__myClass"));
  CHECK_CONTAINERS(list({"my", "Class"}), splitIdentifer("__myClass__"));
  CHECK_CONTAINERS(list({"My", "Class"}), splitIdentifer("__My_Class__"));
}

BOOST_AUTO_TEST_CASE(NameStyle_GeneralId_WordSep)
{
  CHECK_CONTAINERS(list({"my", "Class"}), splitIdentifer("my_Class__"));
  CHECK_CONTAINERS(list({"my", "Class"}), splitIdentifer("__my__Class"));
  CHECK_CONTAINERS(list({"my", "CLASS"}), splitIdentifer("__my____CLASS__"));
}

BOOST_AUTO_TEST_CASE(NameStyle_GeneralId_Numbers)
{
  CHECK_CONTAINERS(list({"1"}), splitIdentifer("_1"));
  CHECK_CONTAINERS(list({"2th"}), splitIdentifer("_2th"));
  CHECK_CONTAINERS(list({"2TH"}), splitIdentifer("_2TH"));
  CHECK_CONTAINERS(list({"2Th"}), splitIdentifer("_2Th"));
}

BOOST_AUTO_TEST_CASE(NameStyle_GeneralId_SpecialCases)
{
  CHECK_CONTAINERS(list<const char*>({}), splitIdentifer(""));
  CHECK_CONTAINERS(list({"_"}), splitIdentifer("_"));
  CHECK_CONTAINERS(list({"a"}), splitIdentifer("a"));
  CHECK_CONTAINERS(list({"A"}), splitIdentifer("A"));
  CHECK_CONTAINERS(list({"1"}), splitIdentifer("1"));
  CHECK_CONTAINERS(list({"_"}), splitIdentifer("-"));
}

BOOST_AUTO_TEST_CASE(NameStyle_GeneralId_FileNames)
{
  CHECK_CONTAINERS(list({"my", "class"}), splitIdentifer("my-class"));
  CHECK_CONTAINERS(list({"my", "class"}), splitIdentifer("my----class"));
  CHECK_CONTAINERS(list({"my", "class"}), splitIdentifer("my class"));
}

BOOST_AUTO_TEST_CASE(NameStyle_GeneralId_Random)
{
  std::mt19937 rg(42);
  StringGenerator g(
    "abcdefghijklmnopqrstuvw"
    "ABCDEFGHIJKLMNOPQRSTUVW"
    "0123456789"
    "_",
    {1, 15}
  );

  Refactor::Support::runNTimes(1000, [&]()
  {
    bool ok = false;
    auto r = splitIdentifer(g(rg), &ok);

    if (!ok)
    {
      std::ostringstream s;
      s << "parse failed for `" << r[0] << "`";
      BOOST_CHECK_MESSAGE(ok, s.str().c_str());
    }

    // no empty part
    BOOST_CHECK_EQUAL(0, std::count_if(r.begin(), r.end(),
      [](auto& e){ return e.size() == 0; })
    );
  });
}

// join

BOOST_AUTO_TEST_CASE(NameStyle_joinIdentifier_Original)
{
  TypeNameStyle style;
  style.capitalization = Capitalization::Original;
  style.word_delimiter = "_";

  BOOST_CHECK_EQUAL("my_Class", joinIdentifier({"my", "Class"}, style));
  BOOST_CHECK_EQUAL("MY_Class", joinIdentifier({"MY", "Class"}, style));
}

BOOST_AUTO_TEST_CASE(NameStyle_joinIdentifier_LowerCase)
{
  TypeNameStyle style;
  style.capitalization = Capitalization::LowerCase;
  style.word_delimiter = "_";

  BOOST_CHECK_EQUAL("my_class", joinIdentifier({"My", "Class"}, style));
  BOOST_CHECK_EQUAL("my_class", joinIdentifier({"MY", "Class"}, style));
}

BOOST_AUTO_TEST_CASE(NameStyle_joinIdentifier_UpperCase)
{
  TypeNameStyle style;
  style.capitalization = Capitalization::UpperCase;

  BOOST_CHECK_EQUAL("MYCLASS", joinIdentifier({"My", "Class"}, style));
  BOOST_CHECK_EQUAL("MYCLASS", joinIdentifier({"MY", "Class"}, style));
}

BOOST_AUTO_TEST_CASE(NameStyle_joinIdentifier_LowerCamelCase)
{
  TypeNameStyle style;
  style.capitalization = Capitalization::LowerCamelCase;

  BOOST_CHECK_EQUAL("myClass", joinIdentifier({"my", "cLaSs"}, style));
  BOOST_CHECK_EQUAL("myClass", joinIdentifier({"MY", "class"}, style));
}

BOOST_AUTO_TEST_CASE(NameStyle_joinIdentifier_UpperCamelCase)
{
  TypeNameStyle style;
  style.capitalization = Capitalization::UpperCamelCase;

  BOOST_CHECK_EQUAL("MyClass", joinIdentifier({"my", "cLaSs"}, style));
  BOOST_CHECK_EQUAL("MyClass", joinIdentifier({"MY", "class"}, style));
}

BOOST_AUTO_TEST_CASE(NameStyle_joinIdentifier_PreSuffix)
{
  TypeNameStyle style;
  style.capitalization = Capitalization::LowerCase;
  style.word_delimiter = "_";
  style.prefix = "_P";
  style.suffix = "_S";

  BOOST_CHECK_EQUAL("_Pmy_class_S", joinIdentifier({"my", "cLaSs"}, style));
}

BOOST_AUTO_TEST_CASE(NameStyle_joinIdentifier_Seperator)
{
  TypeNameStyle style;
  style.capitalization = Capitalization::LowerCase;

  style.word_delimiter = "_";
  BOOST_CHECK_EQUAL("my_class", joinIdentifier({"my", "cLaSs"}, style));

  style.word_delimiter = "__";
  BOOST_CHECK_EQUAL("my__class", joinIdentifier({"my", "cLaSs"}, style));

  style.word_delimiter = "";
  BOOST_CHECK_EQUAL("myclass", joinIdentifier({"my", "cLaSs"}, style));

  style.word_delimiter = "Z";
  BOOST_CHECK_EQUAL("myZclassZforZever", joinIdentifier({"my", "cLaSs", "For", "ever"}, style));

  style.word_delimiter = "Z";
  BOOST_CHECK_EQUAL("my", joinIdentifier({"my"}, style));

  style.word_delimiter = "Z";
  BOOST_CHECK_EQUAL("", joinIdentifier({}, style));
}


BOOST_AUTO_TEST_CASE(NameStyle_joinIdentifier_UseCases)
{
  TypeNameStyle style;

  style.capitalization = Capitalization::LowerCamelCase;
  style.word_delimiter = "";
  style.prefix = "";
  style.suffix = "";
  BOOST_CHECK_EQUAL("myClass", joinIdentifier({"MY", "CLASS"}, style));

  style.capitalization = Capitalization::UpperCamelCase;
  style.word_delimiter = "";
  style.prefix = "k";
  style.suffix = "";
  BOOST_CHECK_EQUAL("kMyConstant", joinIdentifier({"MY", "CONSTANT"}, style));

  style.capitalization = Capitalization::UpperCase;
  style.word_delimiter = "_";
  style.prefix = "";
  style.suffix = "";
  BOOST_CHECK_EQUAL("MY_CONSTANT", joinIdentifier({"MY", "CONSTANT"}, style));

  style.capitalization = Capitalization::LowerCase;
  style.word_delimiter = "";
  style.prefix = "";
  style.suffix = "";
  BOOST_CHECK_EQUAL("myvar", joinIdentifier({"MY", "var"}, style));

  style.capitalization = Capitalization::LowerCase;
  style.word_delimiter = "_";
  style.prefix = "__";
  style.suffix = "__";
  BOOST_CHECK_EQUAL("__my_class__", joinIdentifier({"my", "cLaSs"}, style));

  style.prefix = "";
  style.suffix = "_t";
  BOOST_CHECK_EQUAL("my_class_t", joinIdentifier({"my", "cLaSs"}, style));

  style.prefix = "_";
  style.suffix = "";
  BOOST_CHECK_EQUAL("_my_class", joinIdentifier({"my", "cLaSs"}, style));

  style.prefix = "";
  style.suffix = ".cpp";
  BOOST_CHECK_EQUAL("my_class.cpp", joinIdentifier({"my", "cLaSs"}, style));

  style.word_delimiter = "";
  style.prefix = "";
  style.suffix = "_test.cc";
  BOOST_CHECK_EQUAL("myclass_test.cc", joinIdentifier({"my", "cLaSs"}, style));

  style.capitalization = Capitalization::LowerCase;
  style.word_delimiter = "-";
  style.prefix = "";
  style.suffix = ".cpp";
  BOOST_CHECK_EQUAL("my-class-for-ever.cpp", joinIdentifier({"my", "cLaSs", "For", "ever"}, style));
}

BOOST_AUTO_TEST_SUITE_END()
