#pragma once

#include <string>

class Gene
{
public:
    Gene();
    Gene(const std::string &gene_);
    Gene(const std::string &gene_, int id_);
    bool operator<(const Gene &other) const;
    bool operator==(const Gene &other) const;
    std::string gene;
    int id;

private:
};