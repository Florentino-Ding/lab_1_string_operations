//
// Created by 丁麓然 on 9/22/23.
//

#include "../../inc/datastructure/customstring.h"
#include "../../inc/datastructure/customlist.h"
#include <cwchar>
#include <istream>
#include <ostream>

namespace custom {
string::string() : _data(nullptr), _length(0), _capacity(DEFAULT_CAPACITY) {}
string::string(const char *str) {
  if (str == nullptr) {
    _data = nullptr;
    _length = 0;
    _capacity = DEFAULT_CAPACITY;
  } else {
    size_t _length = strlen(str) + 1;
    _data = new wchar_t[_length];
    mbstowcs(_data, str, _length);
    _length = _length - 1;
    _capacity = _length;
  }
}
string::string(const wchar_t *str) {
  if (str == nullptr) {
    _data = nullptr;
    _length = 0;
    _capacity = DEFAULT_CAPACITY;
  } else {
    _length = wcslen(str);
    _data = new wchar_t[_length];
    wcscpy(_data, str);
    _capacity = _length;
  }
}
string::string(const string &str) {
  _length = str._length;
  _capacity = str._capacity;
  _data = new wchar_t[_capacity];
  wcscpy(_data, str._data);
}
string::string(const std::string &str) {
  size_t _length = str.length() + 1;
  _data = new wchar_t[_length];
  mbstowcs(_data, str.c_str(), _length);
  _length = _length - 1;
  _capacity = _length;
}
string::~string() { delete[] _data; }
string &string::operator=(const string &str) {
  if (this == &str) {
    return *this;
  }
  delete[] _data;
  _length = str._length;
  _capacity = str._capacity;
  _data = new wchar_t[_capacity];
  wcscpy(_data, str._data);
  return *this;
}
string &string::operator+=(const string &str) {
  if (str._length == 0) {
    return *this;
  }
  if (_length + str._length > _capacity) {
    wchar_t *temp = new wchar_t[_length + str._length];
    wcscpy(temp, _data);
    wcscat(temp, str._data);
    delete[] _data;
    _data = temp;
    _capacity = _length + str._length;
    _length = _length + str._length;
  } else {
    wcscat(_data, str._data);
    _length = _length + str._length;
  }
  return *this;
}
string string::operator+(const string &str) {
  string temp(*this);
  temp += str;
  return temp;
}
wchar_t &string::operator[](int index) { return _data[index]; }
wchar_t string::operator[](int index) const { return _data[index]; }
bool string::operator==(const string &str) const {
  if (_length != str._length) {
    return false;
  }
  for (int i = 0; i < _length; ++i) {
    if (_data[i] != str._data[i]) {
      return false;
    }
  }
  return true;
}
std::wostream &string::operator<<(std::wostream &os) const {
  os << _data;
  return os;
}
std::wistream &string::operator>>(std::wistream &is) {
  is >> _data;
  return is;
}
string::operator std::string() const {
  std::string temp;
  temp.resize(_length);
  wcstombs(&temp[0], _data, _length);
  return temp;
}
string::operator const wchar_t *() const { return _data; }
int string::size() const { return _length; }
int string::length() const { return _length; }
int string::capacity() const { return _capacity; }
bool string::empty() const { return _length == 0; }
void string::clear() {
  delete[] _data;
  _data = nullptr;
  _length = 0;
  _capacity = DEFAULT_CAPACITY;
}
string string::reverse() const {
  string temp(*this);
  for (int i = 0; i < _length / 2; ++i) {
    wchar_t t = temp._data[i];
    temp._data[i] = temp._data[_length - i - 1];
    temp._data[_length - i - 1] = t;
  }
  return temp;
}
list<string> string::split(const wchar_t *delimiters) const {
  if (_length == 0) {
    return list<string>();
  }
  if (delimiters == nullptr) {
    delimiters = L" ";
  }
  list<string> temp;
  wchar_t temp_data[_length + 1];
  wcscpy(temp_data, _data);
  wchar_t *token = std::wcstok(_data, delimiters, nullptr);
  while (token != nullptr) {
    temp.push_back(token);
    token = std::wcstok(nullptr, delimiters, nullptr);
  }
  return temp;
}

double string_similarity(const string &str1, const string &str2) {
  if (str1.empty() || str2.empty()) {
    return 0;
  }
  int len1 = str1.length();
  int len2 = str2.length();
  int **dp = new int *[len1 + 1];
  for (int i = 0; i <= len1; ++i) {
    dp[i] = new int[len2 + 1];
  }
  for (int i = 0; i <= len1; ++i) {
    dp[i][0] = 0;
  }
  for (int i = 0; i <= len2; ++i) {
    dp[0][i] = 0;
  }
  for (int i = 1; i <= len1; ++i) {
    for (int j = 1; j <= len2; ++j) {
      if (str1[i - 1] == str2[j - 1]) {
        dp[i][j] = dp[i - 1][j - 1] + 1;
      } else {
        dp[i][j] = dp[i - 1][j] > dp[i][j - 1] ? dp[i - 1][j] : dp[i][j - 1];
      }
    }
  }
  double similarity = dp[len1][len2] * 1.0 / (len1 > len2 ? len1 : len2);
  for (int i = 0; i <= len1; ++i) {
    delete[] dp[i];
  }
  delete[] dp;
  return similarity;
}

} // namespace custom