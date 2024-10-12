#include <unordered_map>

#include "Gene.hpp"

Gene::Gene() : gene(""), id(-1) {}

Gene::Gene(const std::string &gene_) : gene(gene_), id(-1) {}

Gene::Gene(const std::string &gene_, int id_) : gene(gene_), id(id_) {}

bool Gene::operator<(const Gene &other) const
{
  if (gene.size() < other.gene.size())
  {
    return true;
  }

  if (gene.size() > other.gene.size())
  {
    return false;
  }

  return gene < other.gene;
}

bool Gene::operator==(const Gene &other) const
{
  return gene == other.gene;
}