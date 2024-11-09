#pragma once

#include <list>
#include <vector>
#include <set>
#include <functional>
#include <iostream>
#include <memory>
#include <chrono>
#include <queue>

class BigNum
{
public:
  std::vector<int> value; // value[0] will be rightmost digit

  BigNum();
  BigNum(const std::string &stringValue);
  BigNum(std::vector<int> values) : value(values) {}

  BigNum operator-(const BigNum &other) const;
  BigNum operator+(const BigNum &other) const;
  BigNum operator*(const BigNum &other) const;
  BigNum operator/(const BigNum &other) const;
  BigNum operator%(const BigNum &other) const;

  BigNum modExp(const BigNum &exp, const BigNum &mod) const;

  bool operator<(const BigNum &other) const;
  bool operator>(const BigNum &other) const;
  bool operator==(const BigNum &other) const;

  std::string toString() const;

  friend std::ostream &operator<<(std::ostream &os, const BigNum &num)
  {
    os << num.toString();
    return os;
  }

private:
  BigNum intMult(int integer) const;
};