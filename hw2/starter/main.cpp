#include "SinglyLinkedList.hpp"
#include <iostream>

int main(int argc, char *argv[])
{
  /*** TODO: implement main function
   * 1. parse the argument to get input csv directory, output csv directory, and the range number (start position, end position) via flag.
   *    (e.g. ./CityPopulationAnalysis -o output.csv -s 10 -e 12 -i world_population_by_city_2024.csv )
   * 2. construct singly linked list by loading it from CSV
   * 3. write the output to the csv
   */

  // for now just test the singly linked list
  SinglyLinkedList list;
  list.insertCity("Beijing", "China", "Asia", 27'058'480, 25'463'000, 0.027);
  list.insertCity("Shanghai", "China", "Asia", 27'058'480, 26'317'104, 0.028);
  list.insertCity("Istanbul", "Turkey", "Europe", 15'190'336, 14'798'000, 0.026);
  list.insertCity("Karachi", "Pakistan", "Asia", 27'058'480, 20'463'000, 0.022);

  list.printList();

  return 0;
}
