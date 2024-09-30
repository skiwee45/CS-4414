#include "SinglyLinkedList.hpp"
#include <iostream>
#include <cstring>

int main(int argc, char *argv[])
{
  /*** TODO: implement main function
   * 1. parse the argument to get input csv directory, output csv directory, and the range number (start position, end position) via flag.
   *    (e.g. ./CityPopulationAnalysis -o output.csv -s 10 -e 12 -i world_population_by_city_2024.csv )
   * 2. construct singly linked list by loading it from CSV
   * 3. write the output to the csv
   */

  std::string inputFile;
  std::string outputFile;
  int start = 0;
  int end = 0;

  for (int i = 1; i < argc; ++i)
  {
    if (strcmp(argv[i], "-i") == 0 && i + 1 < argc)
    {
      inputFile = argv[++i];
    }
    else if (strcmp(argv[i], "-o") == 0 && i + 1 < argc)
    {
      outputFile = argv[++i];
    }
    else if (strcmp(argv[i], "-s") == 0 && i + 1 < argc)
    {
      start = std::stoi(argv[++i]);
    }
    else if (strcmp(argv[i], "-e") == 0 && i + 1 < argc)
    {
      end = std::stoi(argv[++i]);
    }
  }

  SinglyLinkedList list;
  list.loadFromCSV(inputFile);
  list.printList();
  list.writeRangeOfCitiesToCSV(outputFile, start, end);

  return 0;
}
