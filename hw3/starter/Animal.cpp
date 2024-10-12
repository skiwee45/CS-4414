#include "Animal.hpp"
#include <iostream>

Animal::Animal()
{
}

Animal::Animal(std::ifstream &file)
{

  std::string line;
  std::getline(file, line);
  extractLineData(line, name);

  std::getline(file, line);
  extractLineData(line, latinName);

  std::getline(file, line);
  extractLineData(line, imageFilename);

  std::getline(file, line);
  std::string dna;
  extractLineData(line, dna, 3);

  createGenes(dna);
}

bool Animal::operator<(const Animal &other) const
{
  return getName() < other.getName();
}

void Animal::extractLineData(const std::string &line, std::string &data, int numCharsAtEnd)
{
  std::size_t pos = line.find("=");

  if (pos != std::string::npos)
  {
    data = line.substr(pos + 2, line.size() - pos - numCharsAtEnd);
  }
}

void Animal::createGenes(const std::string &dna)
{
  std::size_t geneStart = 0;
  std::size_t geneEnd = -3;
  while ((geneStart = dna.find("IAY", geneEnd + 3)) != std::string::npos)
  {
    geneEnd = dna.find("TYI", geneStart);
    if (geneEnd == std::string::npos)
    {
      break;
    }

    std::string gene = dna.substr(geneStart + 3, geneEnd - geneStart - 3);
    genes.push_back(Gene(gene));
  }
}

static bool compareGenesById(Gene &a, Gene &b)
{
  return a.id < b.id;
}

void Animal::sortGenesById()
{
  std::sort(genes.begin(), genes.end(), compareGenesById);
}