#include <iostream>
#include <fstream>
#include <string>
#include <cmath>


#define minPts 4
#define EPSILON 15


struct Point{
    double x, y;
    bool visited;
    int clusterID;
};


Point* readFile(std::string filename, int &total_number);
double euclidean_distance(Point p1, Point p2);
void find_neighbors(Point* points, int total_number, int current_point, int* neighbors, int& number_of_neighbors);
void expand_cluster(Point* points, int total_number, int current_cluster, int current_point);;
void dbscan(Point* points, int total_number);
void print_dataset(Point* points, int total_number);
void write_csv_file(std::string filename, Point* points, int total_number);
