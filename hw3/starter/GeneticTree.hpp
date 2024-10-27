#pragma once

#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

#include "Gene.hpp"
#include "Animal.hpp"

class GeneticTree
{
public:
  // no arg constructor
  GeneticTree();
  GeneticTree(const Animal &root_, const std::vector<GeneticTree *> &children_);                                     // use this on the children
  GeneticTree(const std::vector<std::vector<int>> &animalMatrix, const std::vector<Animal> &animals, int rootIndex); // only use this on in the beginning

  void printLevel(int level, std::string &header) const;

  Animal root;
  std::vector<GeneticTree *> children;

private:
};