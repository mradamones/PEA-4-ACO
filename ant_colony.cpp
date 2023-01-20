#include "ant_colony.h"

AntColony::AntColony() = default;

int AntColony::algorithm(int vert, int** dist, float a, float b, bool type){
    alpha = a;
    beta = b;
    pherom_type = type;
    vertices = vert;
    int max_iter;
    if(vert<26)
        max_iter = 2000;
    else if(vert<351)
        max_iter = 25;
    else
        max_iter = 3;
    distances = dist;
    best_cost = INT_MAX;
    ant_num = vertices;
    int i = 0;
    auto start = std::chrono::high_resolution_clock::now();
    initPheromones();
    initAnts();
    while(true){
        runAnts();
        auto stop = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::seconds>(stop - start).count();
        if((stopCondition()&&i!=0) || i==max_iter || duration > 600)
            break;
        restartAnts();
        i++;
        //std::cout<< i << std::endl;
    }
    return best_cost;
}

bool AntColony::stopCondition(){
    for(int i=0;i<vertices;i++){
        if(ants[i]->tourLength != ants[0]->tourLength)
            return false;
    }
    return true;
}

void AntColony::initPheromones(){
    float val = (float)vertices/(float)calculateExpectedCost();
    this->pheromones = new float*[vertices];
    for(int i = 0; i < vertices; ++i){
        this->pheromones[i] = new float[vertices] {val};
    }
}

int AntColony::calculateExpectedCost(){
    int sumCost = 0;
    int vert = 0;
    int tabu[vertices];
    for(int i = 0;i<vertices;i++) {
        tabu[i]=0;
    }
    tabu[0] = 1;
    for(int i = 0;i<vertices;i++){
        int singleCost = INT_MAX;
        for(int j=0;j<vertices;j++){
            if(distances[vert][j]<singleCost && tabu[j] == 0){
                singleCost = distances[vert][j];
                tabu[j] = 1;
                vert = j;
            }
        }
        if(singleCost!=INT_MAX)
            sumCost += singleCost;
    }
    sumCost+=distances[vert][0];
    return sumCost;
}

void AntColony::initAnts(){
    this->ants = new Ant*[vertices];
    for(int i = 0; i < vertices; i++){
        Ant *ant = new Ant(0, vertices);
        ant->curCity = i;
        for(int from = 0;from < vertices;from++){
            ant->tabu[from]=0;
            ant->path[from]=-1;
        }
        ant->path[vertices]=-1;
        ant->curIter = 0;
        ant->path[0] = ant->curCity;
        ant->tourLength = 0;
        ant->tabu[ant->curCity] = 1;
        ants[i] = ant;
    }
}

void AntColony::runAnts(){
    for(int i=0;i<vertices;i++){
        int n;
        for(int j=0;j<ant_num;j++){
            int next = nextVert(j);
            n = next;
            if(next==-1)
                break;
            ants[j]->prevCity = ants[j]->curCity;
            ants[j]->nextCity = next;
            ants[j]->tabu[next] = 1;
            ants[j]->curIter++;
            ants[j]->path[ants[j]->curIter] = next;
            ants[j]->tourLength += distances[ants[j]->curCity][next];
            ants[j]->curCity = next;
        }
        if(n!=-1)
                updatePheromones();
        evaporatingPheromones();
    }
    for(int i=0;i<ant_num;i++){
        ants[i]->tourLength += distances[ants[i]->curCity][ants[i]->path[0]];
        ants[i]->curCity = ants[i]->path[0];
        ants[i]->path[vertices] = ants[i]->path[0];
        if (ants[i]->tourLength < best_cost)
            best_cost = ants[i]->tourLength;
    }
}

void AntColony::restartAnts() {
    for (int ant = 0; ant < ant_num; ant++) {
        ants[ant]->nextCity = -1;
        ants[ant]->tourLength = 0;

        for (int i = 0; i < vertices; i++) {
            ants[ant]->tabu[i] = 0;
            ants[ant]->path[i] = -1;
        }
        ants[ant]->path[vertices] = -1;
        ants[ant]->curIter = 0;
        ants[ant]->path[0] = ants[ant]->curCity;

        ants[ant]->tabu[ants[ant]->curCity] = 1;
    }
}
float AntColony::calculateProb(int i,int j){
 return pow(pheromones[i][j], alpha)*pow(1.0/(float)distances[i][j], beta);
}

int AntColony::nextVert(int ant){
    int j;
    double denominator = 0.0;
    int i = ants[ant]->curCity;
    for(j=0;j<vertices;j++){
        if(ants[ant]->tabu[j] == 0){
            denominator+= calculateProb(i,j);
        }
    }
    if(denominator == 0)
        return -1;
    double x = (float) rand()/RAND_MAX;
    int k;
    double sumProb = 0.0;
    for(k = 0; k < vertices; k++) {
        if (ants[ant]->tabu[k] == 0) {
            sumProb += (calculateProb(i, k) / denominator);
            if(sumProb>0.99999)
                sumProb = 1;
            if (x <= sumProb)
                return k;
        }
    }
}

void AntColony::evaporatingPheromones(){
    for(int i=0;i<vertices;i++){
        for(int j=0;j<vertices;j++){
            pheromones[i][j] *= ratio;
            if(pheromones[i][j] < 0.1)
                pheromones[i][j]=0.1;
        }
    }
}

void AntColony::updatePheromones() {
    if(pherom_type){
        for(int i=0;i<ant_num;i++){
            pheromones[ants[i]->prevCity][ants[i]->curCity] += q_dens;
            pheromones[ants[i]->curCity][ants[i]->prevCity] += q_dens;
        }
    }else{
        for(int i=0;i<ant_num;i++){
            pheromones[ants[i]->prevCity][ants[i]->curCity] += (q_quan/(float)distances[ants[i]->prevCity][ants[i]->curCity]);
            pheromones[ants[i]->curCity][ants[i]->prevCity] += (q_quan/(float)distances[ants[i]->prevCity][ants[i]->curCity]);
        }
    }
}

AntColony::~AntColony() = default;