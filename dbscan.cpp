#include "dbscan.h"



double euclidean_distance(Point p1, Point p2){
    return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
}



Point* readFile(std::string filename, int &total_number){

    std::string line;
    std::ifstream myfile; 
    myfile.open(filename);
    
    if (!myfile.is_open()){
    std::cerr << "Couldn't open file" << std::endl;
    exit(1);
    }
    
    std::getline(myfile, line); //skipping first line
    total_number = 0;
    while (getline(myfile, line)){
        if(line.empty()) continue;        
        total_number++;            
    }
    myfile.clear();
    myfile.seekg(0);
    
    Point* points = new Point[total_number];
    std::getline(myfile, line);
    int i = 0;
    while (i < total_number){
        std::getline(myfile, line);
        if (line.empty()) continue;
        std::size_t pos = line.find(',', 0);
        double x_coor = std::stod(line.substr(0, pos));
        
        std::size_t pos2 = line.find(',', pos+1);
        double y_coor = std::stod(line.substr(pos+1, pos2));
        
        points[i].x = x_coor;
        points[i].y = y_coor;
        points[i].visited = false;
        points[i].clusterID = -1;
        i++;
    }
    return points;
}
  
    

void find_neighbors(Point* points, int total_number, int current_point, int* neighbors, int& number_of_neighbors){
    number_of_neighbors = 0;
    for (int i = 0; i < total_number; i++){

        double distance = euclidean_distance(points[current_point], points[i]);
        if (distance <= EPSILON){
            neighbors[number_of_neighbors] = i;
            number_of_neighbors++;
        }
    }
}



void expand_cluster(Point* points, int total_number, int current_cluster, int current_point){
    int* neighbors = new int[total_number];
    int number_of_neighbors;
    find_neighbors(points, total_number, current_point, neighbors, number_of_neighbors);
  
    //If the current point is a core point, assign it to the current cluster and expand the cluster to its neighbors
    if (number_of_neighbors >= minPts){
        points[current_point].clusterID = current_cluster;
        points[current_point].visited = true;
        for (int i = 0; i < number_of_neighbors; i++){
            int neighbor = neighbors[i];
            if (points[neighbor].clusterID == -1){
                expand_cluster(points, total_number, current_cluster, neighbor);
            }
        }
    }
  
    //If the current point is not a core point but is reachable from some other core point, assign it to the current cluster
    else if (points[current_point].clusterID == -1){
        for (int i = 0; i < number_of_neighbors; i++){
            int neighbor = neighbors[i];
            points[current_point].visited = true;
            if (points[neighbor].clusterID == current_cluster){
                points[current_point].clusterID = current_cluster;
                break;
            }
        }
    }

    delete[] neighbors;
}



void dbscan(Point* points, int total_number){
    int current_cluster = 0;
    int number_of_neighbors;
    
    for (int i = 0; i < total_number; i++){
        //If the point has already been assigned to a cluster, skip it
        if (points[i].clusterID != -1) continue;

        int* neighbors = new int[total_number];
        
        find_neighbors(points, total_number, i, neighbors, number_of_neighbors);

        //If the point is a core point, expand the cluster starting from this point
        if (number_of_neighbors >= minPts){
            current_cluster++;
            expand_cluster(points, total_number, current_cluster, i);
        }
     
        points[i].visited = true;
        delete[] neighbors;
    }
}



void print_dataset(Point* points, int total_number){

    for(int i = 1; i < total_number+1; i++){
        std::cout << "Point "<< i <<": " << points[i-1].x << "  |  " << points[i-1].y << "  Visited: "<< points[i-1].visited 
        << "  Cluster: "<< points[i-1].clusterID << std::endl;
    }
}



void write_csv_file(std::string filename, Point* points, int total_number){

    std::ofstream myfile; 
    myfile.open(filename);
    if (!myfile.is_open()){
    std::cerr << "Couldn't open file" << std::endl;
    exit(1);
    }
    
    myfile << "x, y, visited, clusterID" << "\n";
    
    for(int i = 0; i < total_number; i++){
        
        myfile << points[i].x << ", "
               << points[i].y << ", "
               << points[i].visited << ", "
               << points[i].clusterID << "\n";
    }
}







