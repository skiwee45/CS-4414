#include <fstream>
#include <sstream>
#include <iostream>
#include <memory>
#include "SinglyLinkedList.hpp"

// Constructor for SinglyLinkedList
SinglyLinkedList::SinglyLinkedList() : head(nullptr) {}


// Insert a new city into the linked list in descending order by population (2024)
void SinglyLinkedList::insertCity(std::string name, std::string country, std::string continent, long population_2024, long population_2023, double growth_rate) {
    /*** TODO: implement insert City method */
}


// Delete a city by name from the list
void SinglyLinkedList::deleteCity(std::string name) {
    /*** TODO: implement delete city method */
}

// Print all cities in the list
void SinglyLinkedList::printList() const {
    /*** TODO: implement this */
}

// read city information to construct the Singly Linked list from the file at filename
void SinglyLinkedList::loadFromCSV(const std::string& filename) {
    /*** TODO: implement this */
}

// write city from start index to end index in the singly linked list to csv at filename 
void SinglyLinkedList::writeRangeOfCitiesToCSV(const std::string& filename, int start, int end) const {
    /*** TODO: implement this */
}


std::shared_ptr<CityNode> SinglyLinkedList::getFirstCity() const {
    return head;
}

// get the shared_ptr of the last city in the SinglyLinkedList.
std::shared_ptr<CityNode> SinglyLinkedList::getLastCity() const {
    /*** TODO: implement this. */
}

/*** Get the shared_ptr to the node at the index.
 * Nodes are ordered by descending order of 2024 population. Indices start from 1. 
 * The city with the highest 2024 population has index 1, second highest population with index 2, etc. 
*/
std::shared_ptr<CityNode> SinglyLinkedList::getCityAtIndex(int index) const {
    /*** TODO: implement this. */
}
