#include <iostream>
#include <fstream>
#include "graph.h"

void print_path(const std::vector<int>& path);
void foo();
std::vector<int> more_than_k_paths(Graph &graph, int source, int destination, int k);
void count_path_between_corners(int n);

void print_path(const std::vector<int>& path){
    std::cout << path[0];
    for(int i = 1; i < path.size(); i++)
        std::cout << " -> " << path[i];
    std::cout << "\n";
}

void print_path_file(const std::vector<int>& path){
    std::ofstream myfile;
    myfile.open ("example.txt");
   
    myfile << path[0] << "\n";
    for(int i = 1; i < path.size(); i++)
        myfile << path[i] << "\n";

    myfile.close();    
}

void foo()
{
    int n_vertices;

    std::cout << "Number of vertices: ";
    std::cin >> n_vertices;

    Graph graph(n_vertices);
    std::cout << "Graph constructed\n";

    while (true)
    {
        std::cout << "\nEnter the origin and the destination of the hamiltonian path: ";
        int source, destination;
        std::cin >> source;
        if (source < 0)
            break;
        std::cin >> destination;

        if (source != destination){
            auto paths = graph.hamiltonian_path(source, destination, true, true);
            std::cout << "Number of hamiltonian paths: " << paths.size() << "\n";
            for(auto path : paths)  print_path(path);
        }
        else{
            auto paths = graph.hamiltonian_cycle(true, true);
            std::cout << "Number of hamiltonian cycles: " << paths.size() << "\n";
            for(auto path : paths)  print_path(path);
        }
    }
}

std::vector<int> more_than_k_paths(Graph &graph, int source, int destination, int k){
    auto paths = graph.hamiltonian_path(source, destination, true, true);
    if(paths.size() >= k)   return paths[0];
    return std::vector<int>();
}

void count_path_between_corners(int n){
    if(n <= 2){
        std::cout << n << std::endl;
        return;
    }

    std::vector<std::vector<int>> adj_list(n*n, std::vector<int>());

    int di[] = {1,0,-1,0};
    int dj[] = {0,1,0,-1};
    for(int i = 0; i < n; i++)
        for(int j = 0; j < n; j++)
            for(int k = 0; k < 4; k++) {
                int ii = i + di[k];
                int jj = j + dj[k];
                if (ii >= 0 && ii < n && jj >= 0 && jj < n)
                    adj_list[ i*n+j ].push_back( ii*n+jj );
            }

    Graph graph(adj_list);
    auto paths = graph.hamiltonian_path(0, n*n-1, true, true);

}

void run_from_file(std::ifstream &file){
    int n_vertices;
    file >> n_vertices;

    Graph graph(n_vertices, file);

    int source, destination;
    file >> source >> destination;

    auto paths = graph.hamiltonian_path(source, destination, true, true);
    for(auto path : paths){
        print_path_file(path);
        break;
    }
}

int main(int argc, char const *argv[])
{
    //count_path_between_corners(14);
    std::ifstream file;
    file.open(argv[1]);
    if(!file){
        std::cerr << "Unable to open file ";
        exit(1);
    }

    run_from_file(file);
    
    file.close();

    return 0;

    try{
        foo();
    }
    catch(const char* str){
        std::cout << "Error: ";
        std::cout << str << "\n";
    }
    catch(...){
        std::cout << "Unknown error\n";
    }
    

    return 0;
}
