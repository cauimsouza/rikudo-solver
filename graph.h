//
// Created by cauim on 26/11/17.
//

#ifndef RIKUDOSOLVER_GRAPH_H
#define RIKUDOSOLVER_GRAPH_H

#include <vector>
#include <cryptominisat5/cryptominisat.h>


class Graph
{
private:
    int n_vertices;
    std::vector< std::vector<int> > adj_list;

    std::vector<bool> visited;
    std::vector< std::vector<int> > paths;
    std::vector<int> path;

    int get_var_id(int ith, int vertex);

    bool backtracking(int v, int n, int last, bool cycle, bool count);

    std::vector< std::vector<int> >&
    hamiltonian_path_sat(int source,
                                int last,
                                bool count,
                                const std::vector< std::pair<int,int> >& map,
                                const std::vector< std::pair<int,int> >& diamonds);

    std::vector< std::vector<int> >& hamiltonian_cycle_sat(bool count);

    std::vector< std::vector<int> >& hamiltonian_path_bt(int source,int last, bool count);

    std::vector< std::vector<int> >& hamiltonian_cycle_bt(bool count);

    void construct_sat(CMSat::SATSolver& solver,
                       const std::vector< std::pair<int,int> >& map = {},
                       const std::vector< std::pair<int,int> >& diamonds = {});

    std::vector<int>& get_path(CMSat::SATSolver& solver, bool cycle = false, int first_id = 0);

public:
    explicit Graph(int n_vertices);

    Graph(const std::vector< std::vector<int> >& adj_list);

    std::vector< std::vector<int> >& hamiltonian_path(int source,
                                                      int last,
                                                      bool sat,
                                                      bool count,
                                                      const std::vector< std::pair<int,int> >& map = {},
                                                      const std::vector< std::pair<int,int> >& diamonds = {});

    std::vector< std::vector<int> >& hamiltonian_cycle(bool sat, bool count);
};

#endif //RIKUDOSOLVER_GRAPH_H
