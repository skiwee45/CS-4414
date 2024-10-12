#pragma once

#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

#include "Gene.hpp"

class Animal
{
public:
    Animal();
    Animal(std::ifstream &file);

    const std::string &getName() const { return name; }
    const std::string &getLatinName() const { return latinName; }
    const std::string &getImageFilename() const { return imageFilename; }
    std::vector<Gene> &getGenes() { return genes; }

    void sortGenesById();
    bool operator<(const Animal &other) const; // friend allows this method to access Animal private members

private:
    std::string name;
    std::string latinName;
    std::string imageFilename;
    std::vector<Gene> genes;

    void extractLineData(const std::string &line, std::string &data, int numCharsAtEnd = 4);
    void createGenes(const std::string &dna);
};