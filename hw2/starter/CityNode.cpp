#include "CityNode.hpp"

CityNode::CityNode()
{
    /*** TODO: implement the default constructor for CityNode class. */
    name = "";
    country = "";
    continent = "";
    population_2024 = 0;
    population_2023 = 0;
    growth_rate = 0.0;
    next = nullptr;
}

CityNode::CityNode(std::string name, std::string country, std::string continent, long population_2024, long population_2023, double growth_rate)
{
    /*** TODO: implement this parametrized constructor. */
    this->name = name;
    this->country = country;
    this->continent = continent;
    this->population_2024 = population_2024;
    this->population_2023 = population_2023;
    this->growth_rate = growth_rate;
    next = nullptr;
}
