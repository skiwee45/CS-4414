#include <iostream>

int main()
{
  int b = 5;
  for (int i = 0; i < 1000000; ++i)
  {
    b += 1;
  }
  std::cout << b << std::endl;
  return 0;
}