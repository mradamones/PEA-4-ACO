#include <iostream>
#include <fstream>
#include <chrono>
#include <vector>
#include "ant_colony.h"

int main() {
    srand(time(nullptr));
    std::ifstream file_in;
    file_in.open("conf.ini");
    if(!file_in.is_open()){
        std::cout << " File OPEN error - FILE_IN" << std::endl;
    }
    std::ofstream file_out("output.csv");
    std::string instance_info;

    while(getline(file_in, instance_info) ){

        std::string instance_name, reps_s, best_cost_s, vertex_s;
        int reps, best_cost;
        std::vector<int> best_path;

        bool read_name, read_reps, read_cost, read_best_path;
        read_name = read_reps = read_cost = read_best_path = true;

        for(int i = 0; i < instance_info.size();i++ ){

            if(!isspace(instance_info[i])){
                if(read_name)
                    instance_name.push_back(instance_info[i]);
                else if(read_reps)
                    reps_s.push_back(instance_info[i]);
                else if(read_cost)
                    best_cost_s.push_back(instance_info[i]);
                else if(read_best_path)
                    vertex_s.push_back(instance_info[i]);
                if(i == instance_info.size()-1 && instance_info[i] != 'v')
                    best_path.push_back(std::stoi(vertex_s));
            }
            else if(read_name)
                read_name = false;
            else if(read_reps)
            { read_reps = false; reps = std::stoi(reps_s); }
            else if(read_cost)
            { read_cost = false;  best_cost = std::stoi(best_cost_s); }
            else if(read_best_path)
            { best_path.push_back(std::stoi(vertex_s));vertex_s.clear(); }
        }

        if(instance_name[instance_name.size()-1] == 'v'){
            file_out << instance_name << std::endl;
        } else{

            file_out << instance_info << std::endl;
            std::ifstream instance_file("./instancje/" + instance_name);
            if(!(instance_file.is_open())){
                std::cout << "Instance could not be opened. Check if  all instances are in \"Instancje\" directory " << std::endl;
                break;
            }
            int vertices;
            instance_file >> vertices;

            int **distances;
            distances = new int*[vertices];
            for(int i = 0; i < vertices; i++){
                std::vector<int> row;
                distances[i] = new int[vertices];
                for(int j = 0; j < vertices; j++){
                    int k;
                    instance_file >> k;
                    row.push_back(k);
                    distances[i][j]=k;
                }
            }

                float alphas[] = {0,1,2};
                float betas[] = {2,3,4,5};
                bool pheromType = true;
                int solution_cost;
                AntColony aco = AntColony();
                if(vertices < 203) {
                    for (float alpha: alphas) {
                        for (float beta: betas) {
                            double error_rate_avg = 0;
                            int rep = 1;
                            long long avg_time = 0;
                            while (rep++ <= reps) {
                                auto start = std::chrono::high_resolution_clock::now();
                                solution_cost = aco.algorithm(vertices, distances, alpha, beta, pheromType);
                                auto stop = std::chrono::high_resolution_clock::now();
                                auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
                                avg_time += duration.count();
                                double error_rate =
                                        ((double) solution_cost - (double) best_cost) / (double) best_cost * 100;
                                if (solution_cost != best_cost) {
                                    error_rate_avg += error_rate;
                                }
                                std::cout << "Cost: " << solution_cost << ". Error rate: " << error_rate
                                          << ". Vertices: " << vertices << std::endl;
                            }
                            avg_time /= reps;
                            error_rate_avg /= reps;
                            file_out << avg_time << ";" << alpha << ";" << beta << ";" << "das;" << error_rate_avg
                                     << std::endl;

                            int rep2 = 1;
                            long long avg_time2 = 0;
                            while (rep2++ <= reps) {
                                auto start = std::chrono::high_resolution_clock::now();
                                solution_cost = aco.algorithm(vertices, distances, alpha, beta, !pheromType);
                                auto stop = std::chrono::high_resolution_clock::now();
                                auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
                                avg_time2 += duration.count();
                                double error_rate =
                                        ((double) solution_cost - (double) best_cost) / (double) best_cost * 100;
                                if (solution_cost != best_cost) {
                                    error_rate_avg += error_rate;
                                }
                                std::cout << "Cost: " << solution_cost << ". Error rate: " << error_rate
                                          << ". Vertices: " << vertices << std::endl;
                            }
                            avg_time2 /= reps;
                            error_rate_avg /= reps;
                            file_out << avg_time2 << ";" << alpha << ";" << beta << ";" << "qas;" << error_rate_avg << std::endl;
                        }
                    }
                }
                else
                {
                    double error_rate_avg = 0;
                    int rep = 1;
                    long long avg_time = 0;
                    while (rep++ <= reps) {
                        auto start = std::chrono::high_resolution_clock::now();
                        solution_cost = aco.algorithm(vertices, distances, 1, 2, pheromType);
                        auto stop = std::chrono::high_resolution_clock::now();
                        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
                        avg_time += duration.count();
                        double error_rate =
                                ((double) solution_cost - (double) best_cost) / (double) best_cost * 100;
                        if (solution_cost != best_cost) {
                            error_rate_avg += error_rate;
                        }
                        std::cout << "Cost: " << solution_cost << ". Error rate: " << error_rate
                                  << ". Vertices: " << vertices << std::endl;
                    }
                    avg_time /= reps;
                    error_rate_avg /= reps;
                    file_out << avg_time << ";" << 1 << ";" << 2 << ";" << "das;" << error_rate_avg
                             << std::endl;

                    int rep2 = 1;
                    long long avg_time2 = 0;
                    while (rep2++ <= reps) {
                        auto start = std::chrono::high_resolution_clock::now();
                        solution_cost = aco.algorithm(vertices, distances, 1, 2, !pheromType);
                        auto stop = std::chrono::high_resolution_clock::now();
                        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
                        avg_time2 += duration.count();
                        double error_rate =
                                ((double) solution_cost - (double) best_cost) / (double) best_cost * 100;
                        if (solution_cost != best_cost) {
                            error_rate_avg += error_rate;
                        }
                        std::cout << "Cost: " << solution_cost << ". Error rate: " << error_rate
                                  << ". Vertices: " << vertices << std::endl;
                    }
                    avg_time2 /= reps;
                    error_rate_avg /= reps;
                    file_out << avg_time2 << ";" << 1 << ";" << 2 << ";" << "qas;" << error_rate_avg << std::endl;
                }
            instance_file.close();
        }
        rename("output.csv",instance_name.c_str());
    }
    file_out.close();
    file_in.close();
}
