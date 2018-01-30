//
// Created by:
//    Cauim de S. Lima - cauimsouza@gmail.com
//    Victor Hugo Vianna Silva - victor.vianna10@gmail.com
//

#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <string>
#include <ctime>
#include <climits>
#include "graph.h"

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
    auto paths = graph.ham_path(0, n*n-1);
    printf("Number of paths: %d\n", (int) paths.size());
}


/**
 * @brief Reads a description of a graph from an input file and finds contraints so that
 * an unique hamiltonian path from a source to an origin exists and writes this path
 * as well as these constraints to an output file.
 * @details The input file must be in the following format (the indication in the right
 * are just for clarification and must not be included in the file):
 * ----input.txt----
 * 4 <- number of vertices
 * 0 1 <-oriented edge in the graph
 * 0 2 <-oriented edge in the graph
 * 1 2 <-oriented edge in the graph
 * 2 1 <-oriented edge in the graph
 * 1 3 <-oriented edge in the graph
 * 3 1 <-oriented edge in the graph
 * -1 <- marks the end of the input file
 * ----input.txt---
 * The output file will be in the following format (the indication in the right
 * are just for clarification and will not be included in the file):
 * ----output.txt----
 * 0 <-first vertex in the path
 * 2 <-second vertex in the path
 * 1 <-third vertex in the path
 * 3 <-fourth vertex in the path
 * -1 <-marks the end of the path
 * 1 2 <- the 1-st vertex in the path should be vertex 2
 * -1 <- marks the end of the list of maps conditions
 * 0 2 <- the vertex following vertex 0 in the path should be following 2
 * -1 <-marks the end of the list of diamond conditions
 * ----output.txt---
 * 
 * @param ifile input file from where to read the description of the graph
 * as well as the source and the origin of the desired hamiltonian path
 * @param ofile output file where to write the unique hamiltonian path and
 * the conditions imposed to this graph that make this path unique. 
 */
void solves_rikudo(std::ifstream &ifile, std::ofstream &ofile)
{
    Graph graph(ifile);

    int begin, end;
    ifile >> begin >> end;

    graph.unique_sol(begin, end, ofile);
}

int main(int argc, char const *argv[])
{
    if(argc == 1){
        std::ifstream ifile("graph.txt");
        std::ofstream ofile("solution.txt");

        solves_rikudo(ifile, ofile);

        ifile.close();
        ofile.close();
    }
    else if(argc == 2){
        std::string ifile_name(argv[1]);
        std::ifstream ifile(ifile_name);
        Graph graph(ifile);
        int source, target;
        ifile >> source >> target;
        auto paths = graph.ham_path(source, target);
        if(paths.size() > 0){
            for(int i : paths[0])
                std::cout << i << " ";
            std::cout << "\n";
        }
    }
    else if(argc == 3){
        std::ifstream ifile;
        std::ofstream ofile;

        std::string inp_file(argv[1]);
        std::string out_file(argv[2]);
        ifile.open(get_path(inp_file));
        if(!ifile){
            std::cerr << "Unable to open input file " << argv[1] << "\n";
            exit(1);
        }

        ofile.open(get_path(out_file));
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


