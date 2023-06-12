#include "dbscan.h"


int main(){

    int total_number;
    std::string filename = "data.csv";
    Point* points = readFile(filename, total_number);

    std::cout <<"DBSCAN started" << std::endl;
    
    dbscan(points, total_number);
    print_dataset(points, total_number);
    
    write_csv_file("results.csv", points, total_number);
    delete[] points;
    return 0;
}




