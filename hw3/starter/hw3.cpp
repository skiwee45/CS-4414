#include <list>
#include <vector>
#include <functional>

#include "Animal.hpp"
#include "GeneticTree.hpp"
#include <iostream>
#include <set>
#include <algorithm>
#include <filesystem>
#include <climits>

// from Sagar's wc++
std::vector<std::filesystem::path> find_all_files(
    const std::filesystem::path &dir, std::function<bool(const std::string &)> pred)
{
    std::list<std::filesystem::path> files_to_sweep;
    // iterate recursively to find all files that satisfy pred
    for (auto &entry : std::filesystem::recursive_directory_iterator(dir))
    {
        if (entry.is_regular_file())
        {
            std::filesystem::path cur_file = entry.path();
            std::string type(cur_file.extension());
            if (pred(type))
            {
                files_to_sweep.push_back(std::move(cur_file));
            }
        }
    }
    return std::vector<std::filesystem::path>(std::make_move_iterator(files_to_sweep.begin()), std::make_move_iterator(files_to_sweep.end()));
}

void numberGenes(std::set<Gene> &genes)
{
    int i = 0;

    for (auto &gene : genes)
    {
        const_cast<Gene &>(gene).id = i++;
    }
}

void updateAnimalGeneIds(std::set<Gene> &genes, std::vector<Animal> &animals)
{
    for (auto &animal : animals)
    {
        auto &animalGenes = animal.genes;
        for (std::size_t i = 0; i < animalGenes.size(); ++i)
        {
            auto &gene = animalGenes[i];
            auto it = genes.find(gene);
            if (it != genes.end())
            {
                gene.id = it->id;
            }
        }

        animal.sortGenesById();
    }
}

void printGenes(std::set<Gene> &genes)
{
    for (auto gene : genes)
    {
        std::cout << "G" << gene.id << "=" << gene.gene << std::endl;
    }
}

void printSpecies(std::vector<Animal> &animals)
{
    int i = 0;
    for (auto animal : animals)
    {
        std::cout << "S" << i++ << "=";
        std::cout << animal.getName() << ": Genes [";
        auto genes = animal.genes;
        for (int j = 0; j < genes.size(); j++)
        {
            if (j == genes.size() - 1)
            {
                std::cout << genes[j].id << "]\n";
            }
            else
            {
                std::cout << genes[j].id << ", ";
            }
        }
    }
}

// make a gene distance matrix
// for each pair of genes, calculate the distance
// store the distance in a matrix
// print the matrix
std::vector<std::vector<int>> createGeneMatrix(std::set<Gene> &genes)
{
    std::vector<std::vector<int>> distanceMatrix(genes.size(), std::vector<int>(genes.size(), 0));

    std::vector<Gene> geneVector(genes.begin(), genes.end());
    for (std::size_t i = 0; i < geneVector.size(); ++i)
    {
        distanceMatrix[geneVector[i].id][geneVector[i].id] = 0;
        for (std::size_t j = i + 1; j < geneVector.size(); ++j)
        {
            int distance = geneVector[i].distance(geneVector[j]);
            distanceMatrix[geneVector[i].id][geneVector[j].id] = distance;
            distanceMatrix[geneVector[j].id][geneVector[i].id] = distance;
        }
    }

    return distanceMatrix;
}

void printGeneMatrix(std::vector<std::vector<int>> &matrix)
{
    // Print header row
    // std::cout << "\t";
    for (std::size_t i = 0; i < matrix.size(); ++i)
    {
        std::cout << "G" << i << "\t";
    }
    std::cout << std::endl;

    // Print matrix with header column
    for (std::size_t i = 0; i < matrix.size(); ++i)
    {
        // std::cout << "G" << i << "\t";
        for (auto &cell : matrix[i])
        {
            std::cout << cell << "\t";
        }
        std::cout << std::endl;
    }
}

int animalDistance(Animal &a, Animal &b, std::vector<std::vector<int>> &geneMatrix)
{
    if (a.isSibling(b))
    {
        return 100000;
    }

    int distance = 0;
    for (std::size_t i = 0; i < a.genes.size(); ++i)
    {
        int bestDist = INT_MAX;
        for (std::size_t j = 0; j < b.genes.size(); ++j)
        {
            auto dist = geneMatrix[a.genes[i].id][b.genes[j].id];
            bestDist = std::min(bestDist, dist);
        }
        distance += bestDist;
    }
    return distance;
}

// animal distance matrix
// for each pair of animals, calculate the distance between each gene and its closest match
// store the distance in a matrix
// print the matrix
std::vector<std::vector<int>> createAnimalMatrix(std::vector<Animal> &animals, std::vector<std::vector<int>> &geneMatrix)
{
    std::vector<std::vector<int>> distanceMatrix(animals.size(), std::vector<int>(animals.size(), 0));

    for (std::size_t i = 0; i < animals.size(); ++i)
    {
        distanceMatrix[i][i] = 0;
        for (std::size_t j = i + 1; j < animals.size(); ++j)
        {
            auto dist1 = animalDistance(animals[i], animals[j], geneMatrix);
            auto dist2 = animalDistance(animals[j], animals[i], geneMatrix);
            auto avgDist = (dist1 + dist2) / 2;
            distanceMatrix[i][j] = avgDist;
            distanceMatrix[j][i] = avgDist;
        }
    }
    return distanceMatrix;
}

void printAnimalMatrix(std::vector<std::vector<int>> &matrix)
{
    // Print header row
    // std::cout << "\t";
    for (std::size_t i = 0; i < matrix.size(); ++i)
    {
        std::cout << "S" << i << "\t";
    }
    std::cout << std::endl;

    // Print matrix with header column
    for (std::size_t i = 0; i < matrix.size(); ++i)
    {
        // std::cout << "S" << i << "\t";
        for (auto &cell : matrix[i])
        {
            std::cout << cell << "\t";
        }
        std::cout << std::endl;
    }
}

// your program should expect two command-line arguments:
// 1. the program name (as always)
// 2. the name of the directory containing the species data
int main(int argc, char *argv[])
{
    /*** TODO: implement main function
     * 1. read the species-data directory from the command line
     * 2. read all the .dat files in the directory (using Sagar's code, if you like)
     * 3. generate output as specified in the handout
     */
    if (argc < 3)
    {
        std::cout << "Usage: " << argv[0] << " <root index>" << " <level>" << std::endl;
        return 1;
    }

    auto rootIndex = std::stoi(argv[1]);
    auto level = std::stoi(argv[2]);

    auto dataFiles = find_all_files(".", [](const std::string &extension)
                                    { return extension == ".dat"; });

    std::vector<Animal> animals;
    for (const auto &filePath : dataFiles)
    {
        std::ifstream file(filePath);
        if (file.is_open())
        {
            animals.emplace_back(file);
            file.close();
        }
        else
        {
            std::cout << "Error opening file: " << filePath << std::endl;
        }
    }

    // create gene list
    std::set<Gene> allGenes;
    for (auto &animal : animals)
    {
        for (const auto &gene : animal.genes)
        {
            allGenes.insert(gene);
        }
    }

    numberGenes(allGenes);
    updateAnimalGeneIds(allGenes, animals);
    printGenes(allGenes);

    std::cout << std::endl;

    // create and print gene matrix
    auto geneMatrix = createGeneMatrix(allGenes);
    printGeneMatrix(geneMatrix);

    std::cout << std::endl;

    std::sort(animals.begin(), animals.end());
    printSpecies(animals);

    std::cout << std::endl;

    // create and print animal matrix
    auto animalMatrix = createAnimalMatrix(animals, geneMatrix);
    printAnimalMatrix(animalMatrix);

    std::cout << std::endl;

    // create genetic tree
    GeneticTree tree(animalMatrix, animals, rootIndex);
    std::string temp = "";
    tree.printLevel(0, temp);

    // indent by one tab per level
    std::string header = "";
    for (int i = 0; i < level; ++i)
    {
        header += "\t";
    }

    if (level != 0)
        tree.printLevel(level, header);
}
