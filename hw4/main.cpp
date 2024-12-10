#include <list>
#include <vector>
#include <set>
#include <functional>
#include <iostream>
#include <memory>
#include <chrono>
#include <queue>
#include "BigNum.hpp"
#include <mutex>
#include <thread>
#include <atomic>

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

const BigNum rsa_n = BigNum("9616540267013058477253762977293425063379243458473593816900454019721117570003248808113992652836857529658675570356835067184715201230519907361653795328462699");

const BigNum rsa_e = BigNum("65537");

const BigNum rsa_d = BigNum("4802033916387221748426181350914821072434641827090144975386182740274856853318276518446521844642275539818092186650425384826827514552122318308590929813048801");

std::string pad(const std::string &s, int length, char padChar = ' ')
{
  if (s.length() >= length)
    return s;
  return std::string(length - s.length(), padChar) + s;
}

std::string getCharCode(char c)
{
  int code = (int)c;
  std::string s = std::to_string(code);
  return pad(s, 3, '0');
}

std::string getStringCode(const std::string &line)
{
  std::string code = "";
  for (char c : line)
  {
    code += getCharCode(c);
  }
  return code;
}

std::tuple<std::string, std::string> getLineCodes(const std::string &line, int lineNum)
{
  // first 48, second 48
  char lineNumChar = lineNum + '0';
  std::string lineNumStr = pad({lineNumChar}, 3);
  std::string first = lineNumStr + line.substr(0, 48);
  first += std::string(51 - first.length(), ' ');
  std::string second = line.substr(std::min(static_cast<int>(line.length()), 48));
  second += std::string(48 - second.length(), ' ') + lineNumStr;
  return {getStringCode(first), getStringCode(second)};
}

void encode()
{
  std::string line;

  int lineNum = 1;

  while (std::getline(std::cin, line))
  {
    auto [line1, line2] = getLineCodes(line, lineNum++);

    BigNum m = BigNum(line1);
    BigNum c = m.modExp(rsa_e, rsa_n);
    std::cout << c << std::endl;

    m = BigNum(line2);
    c = m.modExp(rsa_e, rsa_n);
    std::cout << c << std::endl;
  }
}

std::string getStringFromCode(const std::string &code)
{
  std::string result = "";
  for (size_t i = 0; i < code.length(); i += 3)
  {
    std::string s = code.substr(i, 3);
    int code = std::stoi(s);
    result += (char)code;
  }
  return result;
}

void decodeLine(const std::string &line)
{
  BigNum c = BigNum(line);
  BigNum m = c.modExp(rsa_d, rsa_n);
  std::string coded_m = pad(m.toString(), 51 * 3, '0');
  std::string m1 = getStringFromCode(coded_m).substr(3, 48);

  std::cout << m1;
}

void decode()
{
  std::vector<std::string> lines;
  std::string line1;

  while (std::getline(std::cin, line1))
  {
    std::string line2;
    std::getline(std::cin, line2);
    lines.push_back(line1);
    lines.push_back(line2);
  }

  std::vector<std::string> decodedLines(lines.size());
  std::atomic<int> globalLineNum = 0;
  std::mutex mtx;

  auto decodeLine = [&decodedLines, &mtx, &globalLineNum, &lines]()
  {
    int lineNum;
    while ((lineNum = globalLineNum++) < lines.size())
    {
      auto line = lines[lineNum];
      BigNum c = BigNum(line);
      BigNum m = c.modExp(rsa_d, rsa_n);
      std::string coded_m = pad(m.toString(), 51 * 3, '0');
      int subStrStart = 0;
      if (lineNum % 2 == 0)
      {
        subStrStart = 3;
      }
      std::string msg = getStringFromCode(coded_m).substr(subStrStart, 48);

      std::scoped_lock lock(mtx);
      decodedLines[lineNum] = msg;
    }
  };

  std::vector<std::thread> threads;
  int numThreads = 10;
  for (size_t i = 0; i < numThreads; i++)
  {
    threads.push_back(std::thread(decodeLine));
  }

  for (auto &thread : threads)
  {
    thread.join();
  }

  for (size_t i = 0; i < lines.size() / 2; i++)
  {
    auto fullLine = decodedLines[i * 2] + decodedLines[i * 2 + 1];
    fullLine.erase(fullLine.find_last_not_of(' ') + 1);
    std::cout << fullLine << std::endl;
  }
}

int main(int argc, char const *argv[])
{
  if (argc < 2)
  {
    std::cout << "Usage: " << argv[0] << " <operation> <num1> <num2> [num3]" << std::endl;
    return 1;
  }

  char operation = argv[1][0];

  if (operation == 'e')
  {
    encode();
    return 0;
  }

  if (operation == 'd')
  {
    decode();
    return 0;
  }

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
