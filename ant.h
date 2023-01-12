#include <vector>
#pragma once

class Ant {
public:

    int curCity, nextCity, curIter, prevCity;
    int* tabu;
    int* path;
    int tourLength;

    explicit Ant(int currIter, int vertices);
    virtual ~Ant();
};