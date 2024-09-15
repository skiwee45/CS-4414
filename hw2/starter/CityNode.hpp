#pragma once

#include <iostream>
#include <string>
#include <memory>

class CityNode
{
public:
    /*** TODO: declare attributes required for this class */
    std::string name;
    std::string country;
    std::string continent;
    long population_2024;
    long population_2023;
    double growth_rate;
    std::shared_ptr<CityNode> next;

    CityNode();
    CityNode(std::string name, std::string country, std::string continent, long population_2024, long population_2023, double growth_rate);
};
