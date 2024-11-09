#include <list>
#include <vector>
#include <set>
#include <functional>
#include <iostream>
#include <memory>
#include <chrono>
#include <queue>
#include "BigNum.hpp"

bool checkArgs(int numArgs, char const *argv[])
{
  std::vector<int> nums;
  bool success = true;
  for (size_t i = 0; i < numArgs; i++)
  {
    // check for all ints in string, and no negative signs
    std::string arg = argv[i + 2];
    for (char c : arg)
    {
      if (c < '0' || c > '9')
      {
        std::cout << "Error: \"" << arg << "\" is not an unsigned integer" << std::endl;
        success = false;
        break;
      }
    }
  }

  return success;
}

void printArgs(int n, char const *argv[])
{
  std::cout << "bignum ";
  for (size_t i = 1; i < n - 1; i++)
  {
    std::cout << argv[i] << " ";
  }
  std::cout << argv[n - 1];
  std::cout << std::endl;
}

int main(int argc, char const *argv[])
{
  if (argc < 2)
  {
    std::cout << "Usage: " << argv[0] << " <operation> <num1> <num2> [num3]" << std::endl;
    return 1;
  }

  char operation = argv[1][0];

  if (operation != '+' && operation != '-' && operation != '*' && operation != '/' && operation != '%' && operation != '^')
  {
    std::cout << "Error: " << "\"" << operation << "\" is not a supported operator" << std::endl;
    return 1;
  }

  // check if the operation is ^ and there are only 2 numbers
  if (operation == '^')
  {
    if (argc != 5)
    {
      std::cout << "Error: Exponent requires three numbers" << std::endl;
      return 1;
    }

    if (!checkArgs(3, argv))
      return 1;

    printArgs(5, argv);

    BigNum a = BigNum(argv[2]);
    BigNum b = BigNum(argv[3]);
    BigNum c = BigNum(argv[4]);

    std::cout << a.modExp(b, c) << std::endl;
  }
  else
  {
    if (argc != 4)
    {
      std::cout << "Error: +-*/% requires two numbers" << std::endl;
      return 1;
    }

    if (!checkArgs(2, argv))
      return 1;

    printArgs(4, argv);

    BigNum a = BigNum(argv[2]);
    BigNum b = BigNum(argv[3]);

    if (operation == '+')
    {
      std::cout << (a + b) << std::endl;
    }
    else if (operation == '-')
    {
      std::cout << (a - b) << std::endl;
    }
    else if (operation == '*')
    {
      std::cout << (a * b) << std::endl;
    }
    else if (operation == '/')
    {
      std::cout << (a / b) << std::endl;
    }
    else if (operation == '%')
    {
      std::cout << (a % b) << std::endl;
    }
  }

  return 0;
}
