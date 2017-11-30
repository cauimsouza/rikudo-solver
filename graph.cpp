//
// Created by cauim on 26/11/17.
//

#include "graph.h"
#include <climits>
#include <cassert>

int Graph::get_var_id(int ith, int vertex)
{
    return ith * n_vertices + vertex;
}

// done
bool Graph::backtracking(int v, int n, int last, bool cycle, bool count)
{
    visited[v] = true;
    path[n] = v;

    if (n == n_vertices - 1)
    {
        visited[v] = false;
        if(v == last){
            if(cycle)   path[n_vertices] = path[0];
            paths.push_back(path);
            return true;
        }
        
        return false;
    }

    for (int neighbor : adj_list[v])
    {
        if (!visited[neighbor] && backtracking(neighbor, n + 1, last, cycle, count) && !count)
            return true;
    }

    visited[v] = false;
    return false;
}

// done
Graph::Graph(int n_vertices)
{
    assert(n_vertices > 0);
    this->n_vertices = n_vertices;
    adj_list.assign(static_cast<unsigned long>(n_vertices), std::vector<int>());

    while (true)
    {
        int a, b;
        std::cin >> a;
        if (a < 0)
            break;
        std::cin >> b;
        assert(a < n_vertices && b < n_vertices);
        adj_list[a].push_back(b);
    }
}

// done
Graph::Graph(const std::vector<std::vector<int>>& adj_list){
    this->adj_list = adj_list;
    n_vertices = adj_list.size();
}

// done
void Graph::construct_sat(CMSat::SATSolver& solver)
{
    std::vector<CMSat::Lit> clause;
    solver.new_vars(n_vertices * n_vertices);

    // clauses that guarantees that every vertex is visited during the path
    for (int vertex = 0; vertex < n_vertices; vertex++)
    {
        clause.clear();
        for (int ith = 0; ith < n_vertices; ith++)
        {
            int var_id = get_var_id(ith, vertex);
            clause.push_back(CMSat::Lit(var_id, false));
        }
        solver.add_clause(clause);
    }

    // adds clauses such that each index i in [n_vertices] is occupied precisely once
    for (int ith = 0; ith < n_vertices; ith++)
    {
        // adds clause x_i_0 or x_i_1 or ... or x_i_n-1
        clause.clear();
        for (int vertex = 0; vertex < n_vertices; vertex++)
        {
            int var_id = get_var_id(ith, vertex);
            clause.push_back(CMSat::Lit(var_id, false));
        }
        solver.add_clause(clause);

        // adds clauses !x_i_j or !x_i_k for every j and every k such that j != k
        for (int vertex_1 = 0; vertex_1 < n_vertices; vertex_1++)
        {
            int var_id_1 = get_var_id(ith, vertex_1);
            for (int vertex_2 = vertex_1 + 1; vertex_2 < n_vertices; vertex_2++)
            {
                clause.clear();
                int var_id_2 = get_var_id(ith, vertex_2);
                clause.push_back(CMSat::Lit(var_id_1, true));
                clause.push_back(CMSat::Lit(var_id_2, true));
                solver.add_clause(clause);
            }
        }
    }

    // adds clauses so that x_i_j and x_i+1_k => there is an edge from j to k
    for (int ith = 0; ith < n_vertices - 1; ith++)
    {
        for (int vertex = 0; vertex < n_vertices; vertex++)
        {
            int var_id = get_var_id(ith, vertex);
            clause.clear();
            clause.push_back(CMSat::Lit(var_id, true));
            for (int neighbor : adj_list[vertex])
            {
                int neighbor_id = get_var_id(ith + 1, neighbor);
                clause.push_back(CMSat::Lit(neighbor_id, false));
            }
            solver.add_clause(clause);
        }
    }
}

// done
std::vector<int>& Graph::get_path(CMSat::SATSolver& solver, bool cycle, int first_id)
{
    path.resize(n_vertices);
    path[0] = first_id;
    int cur = first_id;
    for (int ith = 1; ith < n_vertices; ith++)
    {
        for(int neighbor : adj_list[cur]){
            int var_id = get_var_id(ith, neighbor);
            if (solver.get_model()[var_id] == CMSat::l_True)
            {
                path[ith] = neighbor;
                cur = neighbor;
                break;
            }
        }
    }
    if (cycle)
        path.push_back(first_id);
    return path;
}

// done
std::vector<std::vector<int>>& Graph::hamiltonian_path_sat(int source, int last, bool count)
{
    CMSat::SATSolver solver;
    construct_sat(solver);
    int source_id = get_var_id(0, source);
    int last_id = get_var_id(n_vertices - 1, last);
    paths.clear();

    std::vector<CMSat::Lit> assumptions;
    assumptions.push_back(CMSat::Lit(source_id, false));
    assumptions.push_back(CMSat::Lit(last_id, false));

    while(true) {
        CMSat::lbool ret = solver.solve(&assumptions);
        if (ret != CMSat::l_True) {
            //All solutions found.
            break;
        }

        paths.push_back(get_path(solver, false, source));
        if(!count)  break;

        //Banning found solution
        std::vector<CMSat::Lit> ban_solution;
        for (uint32_t var = 0; var < solver.nVars(); var++) {
            if (solver.get_model()[var] != CMSat::l_Undef) {
                ban_solution.push_back(
                        CMSat::Lit(var, (solver.get_model()[var] == CMSat::l_True)? true : false));
            }
        }
        solver.add_clause(ban_solution);
    }

    return paths;
}

// done
std::vector<std::vector<int>>& Graph::hamiltonian_cycle_sat(bool count)
{
    paths.clear();
    int least_degree = INT_MAX;
    int vertex_id = 0;
    for (int v = 0; v < n_vertices; v++)
        if (adj_list[v].size() < least_degree)
        {
            least_degree = static_cast<int>(adj_list[v].size());
            vertex_id = v;
        }

    for (int source : adj_list[vertex_id])
    {
        int source_id = get_var_id(0, source);
        int last_id = get_var_id(n_vertices - 1, vertex_id);

        std::vector<CMSat::Lit> assumptions;
        assumptions.push_back(CMSat::Lit(source_id, false));
        assumptions.push_back(CMSat::Lit(last_id, false));

        CMSat::SATSolver solver;
        construct_sat(solver);

        while(true) {
            CMSat::lbool ret = solver.solve(&assumptions);
            if (ret != CMSat::l_True) {
                //All solutions found.
                break;
            }

            paths.push_back(get_path(solver, true, source));
            if(!count)  break;

            //Banning found solution
            std::vector<CMSat::Lit> ban_solution;
            for (uint32_t var = 0; var < solver.nVars(); var++) {
                if (solver.get_model()[var] != CMSat::l_Undef) {
                    ban_solution.push_back(
                            CMSat::Lit(var, (solver.get_model()[var] == CMSat::l_True)? true : false));
                }
            }
            solver.add_clause(ban_solution);
        }

        if(!paths.empty() && !count)   break;
    }

    return paths;
}

// verifivar
std::vector<std::vector<int>>& Graph::hamiltonian_path_bt(int source, int last, bool count)
{
    paths.clear();
    visited.assign(n_vertices, false);
    path.resize(n_vertices);

    backtracking(source, 0, last, false, count);
    return paths;
}

// verifivar
std::vector<std::vector<int>>& Graph::hamiltonian_cycle_bt(bool count)
{   
    paths.clear();
    int least_degree = INT_MAX;
    int vertex_id = 0;
    for (int v = 0; v < n_vertices; v++)
        if (adj_list[v].size() < least_degree)
        {
            least_degree = static_cast<int>(adj_list[v].size());
            vertex_id = v;
        }

    path.resize(static_cast<unsigned long>(n_vertices+1));
    visited.assign(static_cast<unsigned long>(n_vertices), false);
    for(int source : adj_list[vertex_id]){
        backtracking(source, 0, vertex_id, true, count);
        if(!count && !paths.empty())  break;
    }

    return paths;
}

// falta implementar error handling
std::vector<std::vector<int>>& Graph::hamiltonian_path(int source, int last, bool sat, bool count)
{
    assert(source >= 0 && source < n_vertices);
    assert(last >= 0 && last < n_vertices);
    if (sat)
        return hamiltonian_path_sat(source, last, count);
    else
        return hamiltonian_path_bt(source, last, count);
}

// falta implementar error handling
std::vector<std::vector<int>>& Graph::hamiltonian_cycle(bool sat, bool count)
{
    if (sat)
        return hamiltonian_cycle_sat(count);
    else
        return hamiltonian_cycle_bt(count);
}