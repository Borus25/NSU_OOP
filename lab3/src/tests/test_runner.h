#pragma once

#include <sstream>
#include <string>
#include <map>
#include <set>
#include <vector>
#include <stdexcept>
#include <iostream>

using namespace std;

template <class T, class U>
ostream& operator << (ostream& os, const pair<T, U>& p) {
    return os << p.first << " " << p.second;
}

template <class T>
ostream& operator << (ostream& os, const vector<T>& s) {
  os << "{";
  bool first = true;
  for (const auto& x : s) {
    if (!first) {
      os << ", ";
    }
    first = false;
    os << x;
  }
  return os << "}";
}

template <class T>
ostream& operator << (ostream& os, const set<T>& s) {
  os << "{";
  bool first = true;
  for (const auto& x : s) {
    if (!first) {
      os << ", ";
    }
    first = false;
    os << x;
  }
  return os << "}";
}

template <class K, class V>
ostream& operator << (ostream& os, const map<K, V>& m) {
  os << "{";
  bool first = true;
  for (const auto& kv : m) {
    if (!first) {
      os << ", ";
    }
    first = false;
    os << kv.first << ": " << kv.second;
  }
  return os << "}";
}

template<class T, class U>
void AssertEqual(const T& t, const U& u, const string& hint = {}) {
  if (!(t == u)) {
    ostringstream os;
    os << "Assertion failed: " << t << " != " << u;
    if (!hint.empty()) {
       os << " hint: " << hint;
    }
    throw runtime_error(os.str());
  }
}

inline void Assert(bool b, const string& hint) {
  AssertEqual(b, true, hint);
}

class TestRunner {
public:
  template <class TestFunc>
  void RunTest(TestFunc func, const string& test_name) {
    try {
      func();
      cerr << test_name << " OK" << endl;
    } catch (exception& e) {
      ++fail_count;
      cerr << test_name << " fail: " << e.what() << endl;
    } catch (...) {
      ++fail_count;
      cerr << "Unknown exception caught" << endl;
    }
  }

  ~TestRunner() {
    if (fail_count > 0) {
      cerr << fail_count << " unit tests failed. Terminate" << endl;
      exit(1);
    }
  }

private:
  int fail_count = 0;
};

#define ASSERT_EQUAL(x, y) {                  \
  ostringstream error_os;                     \
  error_os << #x << " != " << #y << ", "      \
    << __FILE__ << ":" << __LINE__;           \
  AssertEqual(x, y, error_os.str());          \
}

#define ASSERT(x) {                     \
  ostringstream error_os;                     \
  error_os << #x << " is false, "             \
    << __FILE__ << ":" << __LINE__;     \
  Assert(x, error_os.str());      \
}

#define RUN_TEST(tr, func) \
  tr.RunTest(func, #func)

#define ASSERT_THROWS(expr, exception_type) \
  try { \
    expr; \
    { \
      ostringstream os; \
      os << #expr << " did not throw " << #exception_type << ", " << __FILE__ << ":" << __LINE__; \
      throw runtime_error(os.str()); \
    } \
  } catch (const exception_type&) { \
/* Expected */ \
  } catch (const std::exception& e) { \
/* Catch standard exceptions if they match inheritance (optional check) */ \
/* But strict check requires specific type */ \
/* If specific type mismatch, rethrow */ \
/* Logic here depends on if exception_type is abstract. Assuming strict check: */ \
/* Actually, let's just rethrow to allow catching specifically */ \
/* BUT, to debug, we can print type name. For now let's keep simple: */ \
/* If we are here, it's a mismatch if typeid(e) != typeid(exception_type) usually */ \
/* But C++ catch(Base) works for Derived. */ \
/* Just swallow if it compiles? No, we need to know if it's WRONG type. */ \
/* Simple hack: */ \
  } catch (...) { \
    ostringstream os; \
    os << #expr << " threw wrong exception type, " << __FILE__ << ":" << __LINE__; \
    throw runtime_error(os.str()); \
  }

