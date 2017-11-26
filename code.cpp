#include <cryptominisat5/cryptominisat.h>
#include <assert.h>
#include <vector>
#include <iostream>
#include <climits>

using std::vector;
using namespace CMSat;

typedef vector<int> vi;

class Graph
{
  private:
    int n_vertices;
    std::vector<std::vector<int>> adj_list;

    SATSolver solver;
    std::vector<Lit> clause;

    std::vector<bool> visited;
    std::vector<int> path;

    int get_var_id(int ith, int vertex)
    {
        return ith * n_vertices + vertex;
    }

    bool backtracking(int v, int n, int last)
    {
        visited[v] = true;
        path[n] = v;

        if (n == n_vertices - 1)
        {
            visited[v] = false;
            if (v == last)
                return true;
            return false;
        }

        for (int neighbor : adj_list[v])
        {
            if (!visited[neighbor] && backtracking(neighbor, n + 1, last))
                return true;
        }

        visited[v] = false;
        return false;
    }

  public:
    Graph(int n_vertices)
    {

        this->n_vertices = n_vertices;
        adj_list.assign(n_vertices, std::vector<int>());

        std::cout << "Construindo grafo...\n";
        std::cout << "Entre com as arestas:\n";
        while (true)
        {
            int a, b;
            std::cin >> a;
            if (a < 0)
                break;
            std::cin >> b;
            adj_list[a].push_back(b);
        }
        std::cout << "Termino da construção do grafo...\n";
    }

    void construct_sat()
    {
        std::cout << "Construindo formula proposicional...\n";
        solver.new_vars(n_vertices * n_vertices);

        // clauses that guarantees that every vertex is visited during the path
        for (int vertex = 0; vertex < n_vertices; vertex++)
        {
            clause.clear();
            for (int ith = 0; ith < n_vertices; ith++)
            {
                int var_id = get_var_id(ith, vertex);
                clause.push_back(Lit(var_id, false));
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
                clause.push_back(Lit(var_id, false));
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
                    clause.push_back(Lit(var_id_1, true));
                    clause.push_back(Lit(var_id_2, true));
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
                clause.push_back(Lit(var_id, true));
                for (int neighbor : adj_list[vertex])
                {
                    int neighbor_id = get_var_id(ith + 1, neighbor);
                    clause.push_back(Lit(neighbor_id, false));
                }
                solver.add_clause(clause);
            }
        }

        std::cout << "Término da construção formula proposicional...\n";
    }

    void print_hamiltonian_path(bool cicle = false, int first_id = 0)
    {
        bool first_vertex = true;
        for (int ith = 0; ith < n_vertices; ith++)
        {
            for (int vertex = 0; vertex < n_vertices; vertex++)
            {
                int var_id = get_var_id(ith, vertex);
                if (solver.get_model()[var_id] == l_True)
                {
                    if (!first_vertex)
                        std::cout << " -> ";
                    first_vertex = false;
                    std::cout << vertex;
                    break;
                }
            }
        }
        if (cicle)
            std::cout << " -> " << first_id;
        std::cout << "\n";
    }

    void solve_hamiltonian_path(int source, int last)
    {
        std::cout << "Procurando caminho hamiltoniano comecando no vertice "
                  << source << " e terminando no vertice " << last << " ...\n";
        int source_id = get_var_id(0, source);
        int last_id = get_var_id(n_vertices - 1, last);

        std::vector<Lit> assumptions;
        assumptions.push_back(Lit(source_id, false));
        assumptions.push_back(Lit(last_id, false));

        lbool ret = solver.solve(&assumptions);

        if (ret == l_True)
        {
            std::cout << "Existe um caminho hamiltoniano :)\n";
            print_hamiltonian_path();
        }
        else
        {
            std::cout << "Não existe um caminho hamiltoniano "
                      << "comecando no vertice " << source
                      << " e terminando no vertice " << last
                      << " :(\n";
        }
    }

    void solve_hamiltonian_cycle()
    {
        std::cout << "Procurando ciclo hamiltoniano...\n";

        int least_degree = INT_MAX;
        int vertex_id = 0;
        for (int v = 0; v < n_vertices; v++)
            if (adj_list[v].size() < least_degree)
            {
                least_degree = adj_list[v].size();
                vertex_id = v;
            }

        bool exist_cycle = false;
        for (int source : adj_list[vertex_id])
        {
            int source_id = get_var_id(0, source);
            int last_id = get_var_id(n_vertices - 1, vertex_id);

            std::vector<Lit> assumptions;
            assumptions.push_back(Lit(source_id, false));
            assumptions.push_back(Lit(last_id, false));

            lbool ret = solver.solve(&assumptions);
            if (ret == l_True)
            {
                std::cout << "Existe um ciclo hamiltoniano :)\n";
                print_hamiltonian_path(true, source);
                exist_cycle = true;
                break;
            }
        }

        if (!exist_cycle)
            std::cout << "Não existe ciclo hamiltoniano :(\n";
    }

    void hamiltonian_path_bt(int source, int last)
    {
        std::cout << "Procurando caminho hamiltoniano comecando no vertice "
                  << source << " e terminando no vertice " << last << " ...\n";
        visited.assign(n_vertices, false);
        path.resize(n_vertices);

        if (backtracking(source, 0, last))
        {
            std::cout << "Existe um caminho hamiltoniano :)\n";
            std::cout << path[0];
            for (int i = 1; i < n_vertices; i++)
                std::cout << " -> " << path[i];
            std::cout << "\n";
        }
        else
        {
            std::cout << "Não existe um caminho hamiltoniano "
                      << "comecando no vertice " << source
                      << " e terminando no vertice " << last
                      << " :(\n";
        }
    }

    void hamiltonian_cycle_bt()
    {
        std::cout << "Procurando ciclo hamiltoniano...\n";

        int least_degree = INT_MAX;
        int vertex_id = 0;
        for (int v = 0; v < n_vertices; v++)
            if (adj_list[v].size() < least_degree)
            {
                least_degree = adj_list[v].size();
                vertex_id = v;
            }

        path.resize(n_vertices);

        bool exist_cycle = false;
        for(int source : adj_list[vertex_id]){
            visited.assign(n_vertices, false);
            if (backtracking(source, 0, vertex_id)){
                std::cout << "Existe um ciclo hamiltoniano :)\n";
                std::cout << path[0];
                for (int i = 1; i < n_vertices; i++)
                    std::cout << " -> " << path[i];
                std::cout << " -> " << path[0] << "\n";

                exist_cycle = true;
                break;
            }
        }
        
        if(!exist_cycle){
            std::cout << "Não existe um ciclo hamiltoniano :(\n";
        }
    }

    void solve_hamiltonian_path(int source, int last, bool sat)
    {
        if (sat)
            solve_hamiltonian_path(source, last);
        else
            hamiltonian_path_bt(source, last);
    }

    void solve_hamiltonian_cycle(bool sat)
    {
        if (sat)
            solve_hamiltonian_cycle();
        else
            hamiltonian_cycle_bt();
    }
};

void foo()
{
    int n_vertices;
    std::cout << "Numero de vertices: ";
    std::cin >> n_vertices;

    Graph graph(n_vertices);
    graph.construct_sat();

    while (true)
    {
        std::cout << "\nEntre com o primeiro e o último destino do caminho hamiltoniano: ";
        int source, last;
        std::cin >> source;
        if (source < 0)
            break;
        std::cin >> last;

        if (source != last)
            graph.solve_hamiltonian_path(source, last, false);
        else
            graph.solve_hamiltonian_cycle(false);
    }
}

int main()
{
    foo();

    return 0;
}