#include <iostream>
#include <fstream>
#include <cstring>
#include "graph.h"


/**
 * @brief Prints a path to the specified
 * 
 * @param path a vector of integers representing the vertices visited in order in the path
 * @param ofile output stream to print the file
 */
void print_path(const std::vector<int>& path, std::ofstream &ofile){
    ofile << path[0];
    for(int i = 1; i < path.size(); i++)
        ofile << " -> " << path[i];
    ofile << "\n";
}

/**
 * @brief Checks if there exists more than k hamiltonian paths in the graph and returns any of them
 * in the form of vector of integers in the positive case and and empty vector otherwise
 * 
 * @param graph graph where to look for paths  
 * @param source source of the paths
 * @param destination destination of the paths
 * @param k minimum number of paths
 * @return a hamiltonian path in graph, starting at vertex source and ending at vertex destination,
 * if graph contains more than k hamiltonian paths, and returns an empty vector otherwise
 */
std::vector<int> more_than_k_paths(Graph &graph, int source, int destination, int k){
    auto paths = graph.hamiltonian_path(source, destination, true, true);
    if(paths.size() >= k)   return paths[0];
    return std::vector<int>();
}

/**
 * @brief Count the number of hamiltonian paths between two opposite corners of a squared grid
 * 
 * @param n size of the squared grid
 */
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

/**
 * @brief Reads graph from file, tries to find hamiltonian paths in this graph
 * and writes to specified output the existing hamiltonian paths
 * 
 * @param file input file from where to read the graph
 * @param file output file where to write the hamiltonian paths
 */
void solves_rikudo(std::ifstream &ifile, std::ofstream &ofile){
    Graph graph(ifile);

    int source, destination;
    ifile >> source >> destination;

    auto paths = graph.hamiltonian_path(source, destination, true, true);
    for(auto path : paths){
        print_path(path, ofile);
        break;
    }
}


/**
 * @brief Tests function hamiltonian_path with backtracking 
 */
void test1(){
    const char* test_in_file = "tests/input/test1.txt";
    const char* test_out_file = "tests/output/test1.txt";
    
    std::ifstream ifile;
    std::ofstream ofile;

    ifile.open(test_in_file);
    ofile.open(test_out_file);

    Graph graph(ifile);
    ofile << "Graph constructed\n";
   
    int source, destination;
    ifile >> source >> destination;

    std::vector< std::pair<int,int> > map;
    while(true){
        int a, b;
        ifile >> a;
        if(a < 0)   break;
        ifile >> b;
        map.push_back(std::make_pair(a, b)); 
    }
    
    std::vector< std::pair<int,int> > diamonds;
    while(true){
        int a, b;
        ifile >> a;
        if(a < 0)   break;
        ifile >> b;
        diamonds.push_back(std::make_pair(a, b)); 
    }

    bool sat, count;
    ifile >> sat >> count; 

    if (source != destination){
        auto paths = graph.hamiltonian_path(source, destination, sat, count, map, diamonds);
        ofile << "Number of hamiltonian paths: " << paths.size() << "\n";
        for(auto path : paths)  print_path(path, ofile);
    }
    else{
        auto paths = graph.hamiltonian_cycle(sat, count);
        ofile << "Number of hamiltonian cycles: " << paths.size() << "\n";
        for(auto path : paths)  print_path(path, ofile);
    }
    
    ofile << "Test completed succesfully\n";

    ifile.close();
    ofile.close();
}

int main(int argc, char const *argv[])
{
    if(argc == 1){
        count_path_between_corners(14);
    }
    else if(argc == 2){
        if(strcmp("test1", argv[1]) == 0)        test1();
    }
    else if(argc == 3){
        std::ifstream ifile;
        std::ofstream ofile;

        ifile.open(argv[1]);
        if(!ifile){
            std::cerr << "Unable to open input file " << argv[1] << "\n";
            exit(1);
        }

        ofile.open(argv[2]);
        if(!ofile){
            std::cerr << "Unable to open output file " << argv[2] << "\n";
            exit(1);
        }

        solves_rikudo(ifile, ofile);

        ifile.close();
        ofile.close();
    }

    return 0;
}


