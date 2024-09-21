#include <fstream>
#include <sstream>
#include <iostream>
#include <memory>
#include "SinglyLinkedList.hpp"

// Constructor for SinglyLinkedList
SinglyLinkedList::SinglyLinkedList() : head(nullptr) {}

static bool compareCity(CityNode &city1, CityNode &city2)
{
    long popDiff = city1.population_2024 - city2.population_2024;
    if (popDiff != 0)
    {
        return popDiff > 0;
    }

    long popDiff2 = city1.population_2023 - city2.population_2023;
    if (popDiff2 != 0)
    {
        return popDiff2 > 0;
    }

    return city1.name > city2.name;
}

// Insert a new city into the linked list in descending order by population (2024)
void SinglyLinkedList::insertCity(std::string name, std::string country, std::string continent, long population_2024, long population_2023, double growth_rate)
{
    /*** TODO: implement insert City method */
    std::shared_ptr<CityNode> newCity = std::make_shared<CityNode>(name, country, continent, population_2024, population_2023, growth_rate);

    if (head == nullptr)
    {
        head = newCity;
        return;
    }

    if (compareCity(*newCity, *head))
    {
        newCity->next = head;
        head = newCity;
        return;
    }

    std::shared_ptr<CityNode> current = head;
    while (current->next != nullptr && compareCity(*current->next, *newCity))
    {
        current = current->next;
    }

    newCity->next = current->next;
    current->next = newCity;
}

// Delete a city by name from the list
void SinglyLinkedList::deleteCity(std::string name)
{
    if (head == nullptr)
    {
        return;
    }

    if (head->name == name)
    {
        head = head->next;
        return;
    }

    std::shared_ptr<CityNode> current = head;
    while (current->next != nullptr && current->next->name != name)
    {
        current = current->next;
    }

    if (current->next != nullptr)
    {
        current->next = current->next->next;
    }
}

// Print all cities in the list
void SinglyLinkedList::printList() const
{
    /*** TODO: implement this */
    std::shared_ptr<CityNode> current = head;

    while (current != nullptr)
    {
        std::cout << current->name << ", " << current->country << ", " << current->continent << ", " << current->population_2024 << ", " << current->population_2023 << ", " << current->growth_rate << std::endl;
        current = current->next;
    }
}

// read city information to construct the Singly Linked list from the file at filename
void SinglyLinkedList::loadFromCSV(const std::string &filename)
{
    /*** TODO: implement this */
}

// write city from start index to end index in the singly linked list to csv at filename
void SinglyLinkedList::writeRangeOfCitiesToCSV(const std::string &filename, int start, int end) const
{
    /*** TODO: implement this */
}

std::shared_ptr<CityNode> SinglyLinkedList::getFirstCity() const
{
    return head;
}

// get the shared_ptr of the last city in the SinglyLinkedList.
std::shared_ptr<CityNode> SinglyLinkedList::getLastCity() const
{
    /*** TODO: implement this. */
    if (head == nullptr)
    {
        return nullptr;
    }

    std::shared_ptr<CityNode> current = head;
    while (current != nullptr && current->next != nullptr)
    {
        current = current->next;
    }

    return current;
}

/*** Get the shared_ptr to the node at the index.
 * Nodes are ordered by descending order of 2024 population. Indices start from 1.
 * The city with the highest 2024 population has index 1, second highest population with index 2, etc.
 */
std::shared_ptr<CityNode> SinglyLinkedList::getCityAtIndex(int index) const
{
    /*** TODO: implement this. */
    if (index < 1 || head == nullptr)
    {
        return nullptr;
    }

    std::shared_ptr<CityNode> current = head;
    int i = 1;
    while (current != nullptr && i < index)
    {
        current = current->next;
        i++;
    }

    return current;
}
