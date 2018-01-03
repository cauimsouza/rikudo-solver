#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include "graph.h"



/**
 * @brief Prints a path to the specified
 * 
 * @param path a vector of integers representing the vertices visited in order in the path
 * @param ofile output stream to print the file
 */
void print_path(const std::vector<int>& path,
                std::ofstream &ofile,
                const std::vector< std::pair<int, int> >& maps = {},
                const std::vector< std::pair<int, int> >& diamonds = {},
                bool conditions = false){
    if(path.size() < 1){
        ofile << "-1\n";
        return;
    }

    for(size_t i = 0; i < path.size(); i++) ofile << path[i] << "\n";
    ofile << "-1\n";

    if(conditions){
        for(auto pair : maps){
            ofile << pair.first << " " << pair.second << "\n";
        }
        ofile << "-1\n";

        for(auto pair : diamonds){
            ofile << pair.first << " " << pair.second << "\n";
        }
        ofile << "-1\n";
    }
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
void bababoo(std::ifstream &ifile, std::ofstream &ofile){
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

/**
 * @brief Checks if pair of vertices is not already in the diamonds list
 * The order of the pair (u, v) is not important.
 * 
 * @param diamonds list of diamonds already inserted
 * @param u first vertex of the new diamond
 * @param v second vertex of the new diamond
 * @return true iff (u, v) was not inserted in the diamonds list previously 
 */
bool valid_new_diamond(std::vector< std::pair<int, int> > &diamonds, int u, int v){
    for(auto pair : diamonds)
    {
       if(pair.first == u && pair.second == v)  return false;
       if(pair.first == v && pair.second == u)  return false; 
    }
    return true;
}

/**
 * @brief Adds new diamond to diamonds list
 * @details It uses the function rand() to generate a pseudo random  
 * integer u in [0...n_vertices-1] and to generate a pseudo random 
 * integer v in [0...out_degree(u)-1], then it checks if the pair (u, v)
 * is already in the diamonds list. If not, then it adds this pair and returns true.
 * Otherwise, it tries again until a maximum number of iterations is reached.   
 * 
 * @param graph graph from where to extract the adjacence list
 * @param diamonds list of diamonds pre-inserted
 * 
 * @return true if a new diamond was successfuly inserted in the diamonds list and false otherwise
 */
bool new_diamond(std::vector<int> &path, std::vector< std::pair<int, int> > &diamonds)
{
    int n_vertices = path.size();

    for(uint64_t iter = 0; iter < n_vertices * n_vertices; iter++){
        int ith = rand() % (n_vertices-1);

        if(valid_new_diamond(diamonds, path[ith], path[ith+1])){
            diamonds.push_back(std::make_pair(path[ith], path[ith+1]));
            return true;
        }
    }

    return false;
}


/**
 * @brief Checks if map is already in the maps list
 * 
 * @param maps list of maps already inserted
 * @param ith instant of visit
 * @param u vertex visited
 * @return true iff (ith, u) was not already inserted in the maps list previously 
 */
bool valid_new_map(std::vector< std::pair<int, int> > &maps, int ith, int u)
{
    for(auto pair : maps){
        if(pair.first == ith || pair.second == u)   return false;
    }

    return true;
}

/**
 * @brief Adds new map to maps list
 * @details It uses the function rand() to generate a pseudo random  
 * integer u in [0...n_vertices-1] and to generate a pseudo random 
 * integer ith in [0...n_vertices-1], then it checks if the pair (ith, u)
 * is already in the maps list. If not, then it adds this pair and returns true.
 * Otherwise, it tries again until a maximum number of iterations is reached.   
 * 
 * @param graph graph from where to extract the adjacence list
 * @param maps list of maps pre-inserted
 * 
 * @return true if a new map was successfuly inserted in the maps list and false otherwise
 */
bool new_map(std::vector<int> &path, std::vector< std::pair<int, int> > &maps)
{
    uint32_t n_vertices = path.size();
    for(uint64_t iter = 0; iter < n_vertices * n_vertices; iter++){
        int ith = rand() % (n_vertices - 2) + 1;
        if(valid_new_map(maps, ith, path[ith])){
            maps.push_back(std::make_pair(ith, path[ith]));
            return true;
        }
    }

    return false;
}


void solves_rikudo(std::ifstream &ifile, std::ofstream &ofile)
{
    Graph graph(ifile);

    int begin, end;
    ifile >> begin >> end;

    std::vector< std::pair<int, int> > diamonds;
    std::vector< std::pair<int, int> > maps;

    uint32_t MAX_ITER = 100000;
    for(uint32_t iter = 0; iter < MAX_ITER; iter++){
        auto paths = graph.k_paths(2, begin, end, maps, diamonds);
        
        // there exists at least two solutions, then we need to add constraint
        if(paths.size() == 2){
            int coin = rand() & 1;
            // add diamond constraint if coin == 0
            if(coin == 0)   new_diamond(paths[0], diamonds);
            else    new_map(paths[0], maps);
        }

        // there exists only one solution, print it to output file and return
        else if(paths.size() == 1){
            print_path(paths[0], ofile, maps, diamonds, true);
            return;
        }      
    }
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


