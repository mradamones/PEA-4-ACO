#include <vector>
#include <climits>
#include <valarray>
#include <iostream>
#include "ant.h"
#include <chrono>
#pragma once
#ifndef ANT_ANT_COLONY_H
#define ANT_ANT_COLONY_H

#endif //ANT_ANT_COLONY_H

class AntColony{
private:
    int** distances;
    float** pheromones;
    Ant** ants;
    int ant_num;
    int best_cost = INT_MAX;
    float q_dens = 10.0;
    float q_quan = 100.0;
    float ratio = 0.5;
    float alpha;
    float beta;
    int vertices;
    bool pherom_type;

public:
    AntColony();
    ~AntColony();

    int algorithm(int vert, int** dist, float a, float b, bool pherom_type);
    void initPheromones();
    void initAnts();
    void updatePheromones();
    float calculateProb(int i, int j);
    int nextVert(int ant);
    bool stopCondition();
    void evaporatingPheromones();
    void runAnts();
    void restartAnts();
    int calculateExpectedCost();
};