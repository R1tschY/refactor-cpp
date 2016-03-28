#include <boost/test/unit_test.hpp>

#include <initializer_list>
#include <vector>
#include <core/parsers.h>

BOOST_AUTO_TEST_SUITE(Parsers_Tests)

// CPP_NARG

template<typename Container>
std::string print_joined(const Container& c, const std::string& sep)
{
  auto first = std::begin(c);
  auto end = std::end(c);

  std::ostringstream s;
  if (first != end)
  {
    s << *first;
  }

  for (++first; first != end; ++first)
  {
    s << sep << *first;
  }

  return s.str();
}

template<typename T1, typename T2>
boost::test_tools::predicate_result
test_container(
  std::initializer_list<T1> exp,
  std::vector<T2> act
)
{
  if (exp.size() != act.size())
  {
    boost::test_tools::predicate_result result(false);
    result.message() << "Different sizes [" << exp.size() << "!=" << act.size() << "]\n";
    result.message() << "expected {" << print_joined(exp, ", ") << "}\n";
    result.message() << "actual {" << print_joined(act, ", ") << "}";
    return result;
  }

  auto iters = std::mismatch(exp.begin(), exp.end(), act.begin());
  if (iters.second != act.end())
  {
    boost::test_tools::predicate_result result(false);
    result.message() << "Different element [" << *iters.first << "!=" << *iters.second << "]\n";
    result.message() << "expected {" << print_joined(exp, ", ") << "}\n";
    result.message() << "actual {" << print_joined(act, ", ") << "}";
    return result;
  }

  return boost::test_tools::predicate_result(true);
}

#define CHECK_CONTAINERS(exp, act) BOOST_CHECK_MESSAGE(test_container((exp), (act)), BOOST_TEST_STRINGIZE(exp == act))

template<typename T>
auto list(std::initializer_list<T> l) -> std::initializer_list<T>
{
  return l;
}

BOOST_AUTO_TEST_CASE(Parsers_GeneralId_CamelCase)
{
  using namespace Refactor::Parsers;

  CHECK_CONTAINERS(list({"My", "Class"}), splitIdentifer("MyClass"));
  CHECK_CONTAINERS(list({"my", "Class"}), splitIdentifer("myClass"));
  CHECK_CONTAINERS(list({"my", "Class", "2"}), splitIdentifer("myClass2"));
  CHECK_CONTAINERS(list({"my2", "Class"}), splitIdentifer("my2Class"));
  CHECK_CONTAINERS(
    list({"my", "Func", "With", "Many", "Args"}),
    splitIdentifer("myFuncWithManyArgs")
  );

  // real world examples
  CHECK_CONTAINERS(list({"Vec3f"}), splitIdentifer("Vec3f"));
}

BOOST_AUTO_TEST_CASE(Parsers_GeneralId_LowerCase)
{
  using namespace Refactor::Parsers;

  CHECK_CONTAINERS(list({"my", "class"}), splitIdentifer("my_class"));
  CHECK_CONTAINERS(list({"my", "class2"}), splitIdentifer("my_class2"));
  CHECK_CONTAINERS(
    list({"my", "func", "with", "many", "args"}),
    splitIdentifer("my_func_with_many_args")
  );
}

BOOST_AUTO_TEST_CASE(Parsers_GeneralId_UpperCase)
{
  using namespace Refactor::Parsers;

  CHECK_CONTAINERS(list({"MY", "MACRO"}), splitIdentifer("MY_MACRO"));
  CHECK_CONTAINERS(list({"MY", "2", "MACRO"}), splitIdentifer("MY_2_MACRO"));
  CHECK_CONTAINERS(
    list({"MY", "FUNC", "WITH", "MANY", "ARGS"}),
    splitIdentifer("MY_FUNC_WITH_MANY_ARGS")
  );
}

BOOST_AUTO_TEST_CASE(Parsers_GeneralId_MixedCase)
{
  using namespace Refactor::Parsers;

  CHECK_CONTAINERS(list({"my", "Class", "with", "ARGS"}), splitIdentifer("my_Class_with_ARGS"));
  CHECK_CONTAINERS(list({"my", "Class", "with", "ARGS"}), splitIdentifer("myClass_with_ARGS"));
  CHECK_CONTAINERS(list({"my", "Class", "with", "ARGS"}), splitIdentifer("my_Class_withARGS"));
  CHECK_CONTAINERS(list({"my", "Class", "with", "ARGS"}), splitIdentifer("myClass_withARGS"));

  CHECK_CONTAINERS(list({"my", "Base64", "Impl"}), splitIdentifer("myBase64Impl"));
  CHECK_CONTAINERS(list({"My", "Base64", "Impl"}), splitIdentifer("MyBase64Impl"));
  CHECK_CONTAINERS(list({"my", "Base64", "IMPL"}), splitIdentifer("myBase64IMPL"));
  CHECK_CONTAINERS(list({"my", "BASE64", "Impl"}), splitIdentifer("myBASE64Impl"));
  CHECK_CONTAINERS(list({"MY", "Base64", "Impl"}), splitIdentifer("MYBase64Impl"));
}

BOOST_AUTO_TEST_SUITE_END()
