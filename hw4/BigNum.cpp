#include <list>
#include <vector>
#include <set>
#include <functional>
#include <iostream>
#include <memory>
#include <chrono>
#include <queue>
#include <string>
#include "BigNum.hpp"

BigNum::BigNum() : value(std::vector<int>()) {}

BigNum::BigNum(const std::string &stringValue)
{
  // go from end to start
  for (int i = stringValue.size() - 1; i >= 0; i--)
  {
    value.push_back(stringValue[i] - '0');
  }
}

static std::tuple<std::vector<int>, std::vector<int>> matchSize(const BigNum &a, const BigNum &b)
{
  size_t size = std::max(a.value.size(), b.value.size());

  auto _a = a.value;
  auto _b = b.value;

  while (_a.size() < size)
  {
    _a.push_back(0);
  }

  while (_b.size() < size)
  {
    _b.push_back(0);
  }

  return {_a, _b};
}

static void trim(BigNum &a)
{
  int i = a.value.size() - 1;
  while (i >= 0 && a.value[i] == 0)
  {
    a.value.pop_back();
    i--;
  }
}

BigNum BigNum::operator+(const BigNum &other) const
{
  auto [v1, v2] = matchSize(*this, other);

  int carryOver = 0;
  for (int i = 0; i < v1.size(); i++)
  {
    int sum = v1[i] + v2[i] + carryOver;
    v1[i] = sum % 10;
    carryOver = sum / 10;
  }

  if (carryOver != 0)
  {
    v1.push_back(carryOver);
  }

  BigNum revResult = BigNum();
  revResult.value = v1;
  return revResult;
}

BigNum BigNum::operator-(const BigNum &other) const
{
  if (*this < other)
  {
    std::cout << "Unsupported: Negative number" << std::endl;
    return BigNum();
  }
  auto [v1, v2] = matchSize(*this, other);

  int carryOver = 0;
  for (int i = 0; i < v1.size(); i++)
  {
    int diff = v1[i] - v2[i] - carryOver;
    if (diff < 0)
    {
      diff += 10;
      carryOver = 1;
    }
    else
    {
      carryOver = 0;
    }

    v1[i] = diff;
  }

  BigNum revResult = BigNum();
  revResult.value = v1;
  trim(revResult);
  return revResult;
}

BigNum BigNum::intMult(int factor) const
{
  BigNum revResult = BigNum();
  auto value = this->value;
  int carryOver = 0;
  for (int i = 0; i < value.size(); i++)
  {
    int product = value[i] * factor + carryOver;
    revResult.value.push_back(product % 10);
    carryOver = product / 10;
  }

  if (carryOver != 0)
  {
    revResult.value.push_back(carryOver);
  }
  return revResult;
}

static std::vector<BigNum> getIntMultLookup(const BigNum &a)
{
  std::vector<BigNum> lookup;
  for (int i = 0; i < 10; i++)
  {
    lookup.push_back(a.intMult(i));
  }
  return lookup;
}

BigNum BigNum::operator*(const BigNum &other) const
{
  std::vector<BigNum> lookup = getIntMultLookup(*this);
  BigNum revResult = BigNum();
  for (size_t i = 0; i < other.value.size(); i++)
  {
    // first append zeros
    BigNum product = BigNum();
    for (size_t j = 0; j < i; j++)
    {
      product.value.push_back(0);
    }

    BigNum temp = lookup[other.value[i]];

    for (int digit : temp.value)
    {
      product.value.push_back(digit);
    }

    revResult = revResult + product;
  }

  return revResult;
}

static std::tuple<BigNum, BigNum> divmod(const BigNum &a, const BigNum &b)
{
  if (b == BigNum("0"))
  {
    std::cout << "Error: Divide by zero" << std::endl;
    return {BigNum("0"), a};
  }

  std::vector<BigNum> lookup = getIntMultLookup(b);

  int i = a.value.size() - 1;
  BigNum revResult;
  std::vector<int> revCurrentValue;
  while (i >= 0)
  {
    auto currentValue = BigNum(std::vector<int>(revCurrentValue.rbegin(), revCurrentValue.rend()));
    while (true)
    {
      revCurrentValue.push_back(a.value[i]);
      currentValue = BigNum(std::vector<int>(revCurrentValue.rbegin(), revCurrentValue.rend()));
      i--;

      if (currentValue < b && i >= 0)
      {
        revResult.value.push_back(0);
      }
      else
        break;
    }

    int multiple = 0;
    while (b < currentValue || b == currentValue)
    {
      currentValue = currentValue - b;
      multiple++;
    }

    revCurrentValue = std::vector<int>(currentValue.value.rbegin(), currentValue.value.rend());
    revResult.value.push_back(multiple);
  }

  std::reverse(revResult.value.begin(), revResult.value.end());
  std::reverse(revCurrentValue.begin(), revCurrentValue.end());
  auto currentValue = BigNum(revCurrentValue);

  trim(revResult);
  trim(currentValue);

  return {revResult, currentValue};
}
BigNum BigNum::operator/(const BigNum &other) const
{
  auto [result, remainder] = divmod(*this, other);
  return result;
}

BigNum BigNum::operator%(const BigNum &other) const
{
  auto [result, remainder] = divmod(*this, other);
  return remainder;
}

BigNum BigNum::modExp(const BigNum &exp, const BigNum &mod) const
{
  // get each level by div 2 and mod 2, first one gets you lowest binary digit
  // if digit is a 1, multiply ongoing power to double the power, then multiply that with a
  BigNum a = *this % mod;
  BigNum b = exp;
  BigNum result = BigNum("1");
  while (b > BigNum("0"))
  {
    auto [quotient, remainder] = divmod(b, BigNum("2"));
    b = quotient;

    if (remainder == BigNum("1"))
    {
      result = result * a % mod;
    }

    a = a * a % mod;
  }
  return result;
}

bool BigNum::operator==(const BigNum &other) const
{
  BigNum a = *this;
  trim(a);

  BigNum b = other;
  trim(b);

  if (a.value.size() != b.value.size())
    return false;

  for (size_t i = 0; i < a.value.size(); i++)
  {
    if (a.value[i] != b.value[i])
      return false;
  }
  return true;
}
bool BigNum::operator<(const BigNum &other) const
{
  BigNum a = *this;
  trim(a);

  BigNum b = other;
  trim(b);

  if (a.value.size() != b.value.size())
  {
    return a.value.size() < b.value.size();
  }

  for (int i = a.value.size() - 1; i >= 0; i--)
  {
    if (a.value[i] < b.value[i])
    {
      return true;
    }

    if (a.value[i] > b.value[i])
    {
      return false;
    }
  }

  return false;
}

bool BigNum::operator>(const BigNum &other) const
{
  return !(*this == other) && !(*this < other);
}

std::string BigNum::toString() const
{
  std::string revResult;
  for (int i = this->value.size() - 1; i >= 0; i--)
  {
    revResult += std::to_string(value[i]);
  }
  if (revResult.size() == 0)
    revResult = "0";
  return revResult;
}