#include <unordered_map>

#include "Gene.hpp"
#include <iostream>

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

const int COMMON_COST = 15;
const int DIFFERENT_COST = 8;
const int ONLY_IN_ONE_COST = 25;

const int MIN_MATCH_LENGTH = 4;

static void trim(std::string &G0, std::string &G1)
{
  // trim common prefix and suffix
  int i = 0;
  while (i < G0.size() && i < G1.size() && G0[i] == G1[i])
  {
    i++;
  }
  G0 = G0.substr(i);
  G1 = G1.substr(i);

  i = 0;
  while (i < G0.size() && i < G1.size() && G0[G0.size() - i - 1] == G1[G1.size() - i - 1])
  {
    i++;
  }
  G0 = G0.substr(0, G0.size() - i);
  G1 = G1.substr(0, G1.size() - i);
}

static int simpleScore(const std::string &G0, const std::string &G1)
{
  // first create a frequency map of both
  std::unordered_map<char, int> freqG0;
  std::unordered_map<char, int> freqG1;

  for (char c : G0)
  {
    freqG0[c]++;
  }

  for (char c : G1)
  {
    freqG1[c]++;
  }

  int cost = 0;

  // now calculate the score
  for (char c : "IAYT")
  {
    if (freqG0[c] == 0 && freqG1[c] == 0)
    {
      continue;
    }

    if (freqG0[c] == 0 || freqG1[c] == 0)
    {
      cost += std::max(freqG0[c], freqG1[c]) * ONLY_IN_ONE_COST;
      continue;
    }

    int maxFreq = std::max(freqG0[c], freqG1[c]);
    int minFreq = std::min(freqG0[c], freqG1[c]);
    cost += minFreq * COMMON_COST + (maxFreq - minFreq) * DIFFERENT_COST;
  }

  return cost;
}

static std::tuple<std::string, std::string, std::string, std::string> findMatch(const std::string &G0, const std::string &G1)
{
  std::string G0left = G0;
  std::string G1left = G1;
  std::string G0right = "";
  std::string G1right = "";

  // slide a window of size MIN_MATCH
  for (size_t _i = MIN_MATCH_LENGTH; _i <= G0.length(); _i++)
  {
    auto i = _i - MIN_MATCH_LENGTH;
    auto chunk = G0.substr(i, MIN_MATCH_LENGTH);

    auto pos = G1.find(chunk);

    if (pos == std::string::npos)
    {
      continue;
    }

    // now try to extend the match
    size_t j = MIN_MATCH_LENGTH;
    while (i + j < G0.length() && pos + j < G1.length() && G0[i + j] == G1[pos + j])
    {
      j++;
    }

    G0left = G0.substr(0, i);
    G1left = G1.substr(0, pos);
    G0right = G0.substr(i + j);
    G1right = G1.substr(pos + j);
    break;
  }

  return {G0left, G1left, G0right, G1right};
}

static int distanceRecursive(const std::string &G0, const std::string &G1)
{
  // G0left, G1left, G0right, G1right = findMatch (G0, G1)
  // scoreLeft = simpleScore(G0left, G1left)
  // scoreRight = min(distanceRecursive(G0right, G1right), distanceRecursive(G1right, G0right))
  // return scoreLeft + scoreRight

  auto [G0left, G1left, G0right, G1right] = findMatch(G0, G1);
  int scoreLeft = simpleScore(G0left, G1left);
  int scoreRight = 0;

  if (G0right.empty() || G1right.empty())
    scoreRight = simpleScore(G0right, G1right);
  else
    scoreRight = std::min(distanceRecursive(G0right, G1right), distanceRecursive(G1right, G0right));

  return scoreLeft + scoreRight;
}

int Gene::distance(const Gene &other) const
{
  // G0, G1 = trim (Ga, Gb)
  auto G0 = this->gene;
  auto G1 = other.gene;
  trim(G0, G1);

  return std::min(distanceRecursive(G0, G1), distanceRecursive(G1, G0));

  // return min(distanceRecursive(G0, G1), distanceRecursive(G1, G0))
}

// int main(int argc, char const *argv[])
// {
//   Gene g1("YTTIAYT");
//   Gene g2("TIAYTAAT");

//   std::cout << g1.distance(g2) << std::endl;
//   return 0;
// }
