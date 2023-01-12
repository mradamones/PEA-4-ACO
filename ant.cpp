#include "Ant.h"

Ant::Ant(int currIter, int vertices){
    curIter = currIter;
    tabu = new int[vertices];
    path = new int[vertices+1];
}

Ant::~Ant() = default;