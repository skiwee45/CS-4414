#include "GeneticTree.hpp"
#include <queue>
#include <iostream>
#include <climits>

GeneticTree::GeneticTree() : root(Animal()), children(std::vector<GeneticTree *>())
{
}

GeneticTree::GeneticTree(const Animal &root_, const std::vector<GeneticTree *> &children_) : root(root_), children(children_)
{
}

GeneticTree::GeneticTree(const std::vector<std::vector<int>> &animalMatrix, const std::vector<Animal> &animals, int rootIndex)
{
  // run prim's algorithm
  auto comparator = [&animals](const std::pair<int, int> &a, const std::pair<int, int> &b)
  {
    if (a.first == b.first)
    {
      return &animals[a.second] > &animals[b.second];
    }
    return a.first > b.first;
  };
  std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, decltype(comparator)> pq(comparator);

  std::vector<int> key(animalMatrix.size(), INT_MAX);
  std::vector<int> parent(animalMatrix.size(), -1);
  std::vector<bool> inMST(animalMatrix.size(), false);

  pq.push({0, rootIndex});
  key[rootIndex] = 0;

  while (!pq.empty())
  {
    auto u = pq.top().second;
    pq.pop();

    inMST[u] = true;

    for (std::size_t v = 0; v < animalMatrix.size(); ++v)
    {
      if (animalMatrix[u][v] && !inMST[v] && animalMatrix[u][v] < key[v])
      {
        key[v] = animalMatrix[u][v];
        pq.push({key[v], v});
        parent[v] = u;
      }
    }
  }

  std::vector<GeneticTree *> allNodes;
  // first create all nodes
  for (std::size_t i = 0; i < animals.size(); ++i)
  {
    auto node = new GeneticTree(animals[i], std::vector<GeneticTree *>());
    allNodes.push_back(node);
  }

  // now add children to the nodes
  for (std::size_t i = 0; i < animals.size(); ++i)
  {
    if (parent[i] != -1)
    {
      allNodes[parent[i]]->children.push_back(allNodes[i]);
      // std::cout << "Parent: " << allNodes[parent[i]]->root.getName() << " Child: " << allNodes[i]->root.getName() << "\n";
    }
  }

  this->root = allNodes[rootIndex]->root;
  this->children = allNodes[rootIndex]->children;
}

void GeneticTree::printLevel(int level, std::string &header) const
{
  // std::cout << "Level " << level << "\n";

  if (level == 0)
  {
    std::cout << header << root.getName() << "\n";
    return;
  }

  for (const auto &child : children)
  {
    child->printLevel(level - 1, header);
  }
}