#include <list>
#include <vector>
#include <functional>

#include "Animal.hpp"
#include <iostream>
#include <set>

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
    if (argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " <species data directory>" << std::endl;
        return 1;
    }

    auto dataFiles = find_all_files(argv[1], [](const std::string &extension)
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
            std::cerr << "Error opening file: " << filePath << std::endl;
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

    std::sort(animals.begin(), animals.end());
    printSpecies(animals);
}
