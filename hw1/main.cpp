#include <fstream>  // std::ifstream (Input File Stream)
#include <iostream> // std::cout (Standard Output)
#include <string>   // std::string
#include <map>

int main(int argc, char **argv)
{
    /*** TODO: implement main(), which should read in the provided CSV file and output according to the homework spec */
    std::ifstream File(argv[1]);
    int n = std::stoi(argv[2]);

    std::cout << "==========" << std::endl;

    std::string line;
    int total = 0;
    while (getline(File, line))
    {
        std::string delimiter = ",";
        std::string name = line.substr(0, line.find(delimiter));
        int number = std::stoi(line.substr(line.find(delimiter) + delimiter.length()));
        if (name.length() >= n)
        {
            total += number;
        }
        if (name.length() == n)
        {
            std::cout << name << std::endl;
        }
    }
    std::cout << total << std::endl;
    std::cout << "==========" << std::endl;

    // Close the file
    File.close();

    return 0;
}