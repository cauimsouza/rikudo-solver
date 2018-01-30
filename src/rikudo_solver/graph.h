//
// Created by cauim on 26/11/17.
//

#ifndef RIKUDOSOLVER_GRAPH_H
#define RIKUDOSOLVER_GRAPH_H

#include <vector>
#include <fstream>


std::string get_path(std::string file_name);


class Graph
{
private:
    /**
     * number of vertices in the graph
     */
    int n_vertices; 

    std::vector<int> dist_s;
    std::vector<int> dist_t;    

    /**
     * adjacence list of the graph
     * the i-th list contains the neighbors of the vertices i, i.e, 
     * vertex j is in the i-th list if there is an edge from i to j in the graph
     */
    std::vector< std::vector<int> > adj_list;
    
    /**
     * list of visited vertices in a partial path
     * this variable is used in the method 'backtracking'
     */
    std::vector<bool> visited;

    /**
     * list of hamiltonian paths/cycles computed
     * each inner std::vector of integers is a path/cycle
     * this variable is returned by the public methods hamiltonian_path and hamiltonian_cycle
     */
    std::vector< std::vector<int> > paths;

    /**
     * list of vertices visited in a hamiltonian cycle/path, in order  
     */
    std::vector<int> path;

    
    /**
     * @brief Returns index of propositional variable coding a vertex visited
     * at a certain instant
     * 
     * @param ith order of the vertex in a path
     * @param vertex vertex number 
     * 
     * @return index of the corresponding propositional variable
     */
    int encode(int ith, int vertex, bool offset=false);

    void decode(int var, int &i, int &v);

    /**
     * @brief Recursive function that tries to find hamiltonian paths in the graph
     * that ends at vertex 'last'
     * 
     * @param v index of current vertex
     * @param n number of vertices already visited
     * @param last last vertex in the path
     * @param count true if we are counting the total number of existing hamiltonian paths
     * @return true if there is a hamiltonian path ending at vertex 'last' and
     * whose 'n'-th visited vertex is 'v' 
     */
    bool backtracking(int v, int n, int last, bool count);

    /**
     * @brief Checks if given candidate path is a valid path
     * 
     * @param candidate hamiltonian path that might or not respect the conditions
     * given by map and diamonds arguments
     * @param map list of pairs of integers of the form (i, v) representing
     * the condition "vertex v must be visited at instant i"
     * @param diamonds list of diamonds in the form of a list of pairs of integers of the form (u, v) representing
     * the condition "vertex v and vertex u must be visited consecutively, in any order"
     * @return true if candidate path is a valid path
     */
    bool valid(const std::vector<int> &candidate,
                  const std::vector< std::pair<int,int> >& map,
                  const std::vector< std::pair<int,int> >& diamonds);

    /**
     * @brief returns a vector of hamiltonian paths 
     * 
     * @param source origin of the path
     * @param last destination of the path
     * @param count whether to count the total number of exinting paths or not
     * @param map list of pairs of integers of the form (i, v) representing
     * the condition "vertex v must be visited at instant i"
     * @param diamonds list of diamonds in the form of a list of pairs of integers of the form (u, v) representing
     * the condition "vertex v and vertex u must be visited consecutively, in any order"
     * @return list of paths possibly empty
     */
    std::vector< std::vector<int> >&
    ham_path_sat(int source,
                 int last,
                 bool count,
                 const std::vector< std::pair<int,int> >& map,
                 const std::vector< std::pair<int,int> >& diamonds);

    /**
     * @brief returns a vector of hamiltonian cycles
     * 
     * @param count whether to count the total number of exinting cycles or not
     * @return list of cycles possibly empty
     */
    std::vector< std::vector<int> >& ham_cycle_sat(bool count,
                                                       const std::vector< std::pair<int,int> >& map,
                                                       const std::vector< std::pair<int,int> >& diamonds);

    /**
     * @brief returns a vector of hamiltonian paths
     * @detail this methods uses a backtracking approach
     * 
     * @param source origin of the path
     * @param last destination of the path
     * @param count whether to count the total number of exinting paths or not
     * @param map list of pairs of integers of the form (i, v) representing
     * the condition "vertex v must be visited at instant i"
     * @param diamonds list of diamonds in the form of a list of pairs of integers of the form (u, v) representing
     * the condition "vertex v and vertex u must be visited consecutively, in any order" 
     * @return list of paths if any exists
     */
    std::vector< std::vector<int> >& ham_path_bt(int source,
                                                 int last,
                                                 bool count,
                                                 const std::vector< std::pair<int,int> >& map,
                                                 const std::vector< std::pair<int,int> >& diamonds);

    /**
     * @brief returns a vector of hamiltonian cycles
     * @detail this methods uses a backtracking approach 
     * 
     * @param count whether to count the total number of exinting cycles or not
     * @param map list of pairs of integers of the form (i, v) representing
     * the condition "vertex v must be visited at instant i"
     * @param diamonds list of diamonds in the form of a list of pairs of integers of the form (u, v) representing
     * the condition "vertex v and vertex u must be visited consecutively, in any order" 
     * @return list of cycles if any exists, the first and the last vertices will be the same
     */
    std::vector< std::vector<int> >& ham_cycle_bt(bool count,
                                                  const std::vector< std::pair<int,int> >& map,
                                                  const std::vector< std::pair<int,int> >& diamonds);

    

    

    
    std::string create_ban(std::vector<int> &orig_path);
    void extend_sat(std::string s, int &n_vars, int &n_clauses);
    std::vector<int> create_cons(int n_vertices);
    void recopy(int n_vars, int n_clauses);
    void add_cons(std::vector<int>& orig_path, std::vector<int>& cons, int pos);
    std::string diam_s(int u, int v, int n_vertices);
    void write_min_cons(std::vector<int>& orig_path, std::vector<int>& cons, int num, std::ofstream &ofile);
    void condition1(std::vector< std::vector<int> > &clauses);
    void condition2(std::vector< std::vector<int> > &clauses);
    void condition3(std::vector< std::vector<int> > &clauses);
    void condition4(std::vector< std::vector<int> > &clauses);
    void condition5(std::vector< std::vector<int> > &clauses);
    void condition6(std::vector< std::vector<int> > &clauses,
        const std::vector< std::pair<int,int> >& map);
    void condition7(std::vector< std::vector<int> > &clauses,
        const std::vector< std::pair<int,int> >& diamonds);
    void condition8(std::vector< std::vector<int> > &clauses);
    void condition9(std::vector< std::vector<int> > &clauses);
    void condition10(std::vector< std::vector<int> > &clauses);
    void condition11(std::vector< std::vector<int> > &clauses, int source);
    void condition12(std::vector< std::vector<int> > &clauses, int dest);
    void condition13(std::vector< std::vector<int> > &clauses, int source);
    void condition14(std::vector< std::vector<int> > &clauses, int dest);
    void write_sat(std::vector<std::vector<int> > &clauses);

public:

    /**
     * @brief Constructs SAT expression representing a hamiltonian path/cycle 
     * 
     * @param solver SAT formula's object
     * @param map list of pairs of integers of the form (i, v) representing
     * the condition "vertex v must be visited at instant i"
     * @param diamonds list of diamonds in the form of a list of pairs of integers of the form (u, v) representing
     * the condition "vertex v and vertex u must be visited consecutively, in any order"
     */
    void construct_sat(int source, int dest,
                       const std::vector< std::pair<int,int> >& map = {},
                       const std::vector< std::pair<int,int> >& diamonds = {});

    /**
     * @brief Reads SAT's solution and generates the corresponding path
     * 
     * @param solver SAT solver object
     * @param cycle whether the path is a cycle or not
     * @param first_id first vertex in the path
     * @return list of vertices in the path in order
     */
    std::vector<int>& read_sol();

    void solve_sat();
    /**
     * @brief Reads graph structure from file 
     * @details The first line in the file must be the number of vertices.
     * The next lines must indicate the oriented edges in the graph
     * in the form of pairs of integers representing the vertices.
     * The list of edges must end with a -1
     * 
     * @param file file from where to read the file
     */
    explicit Graph(std::ifstream &file);

    /**
     * @brief Reads graph from adjacence list
     * @details The generated graph will have number of vertices equals to the size
     * of the adjacence list. The edges are considered as oriented.
     * 
     * @param adj_list adjacence list such that the i-th element contains a list of
     * the neighbors of vertex i 
     */
    Graph(const std::vector< std::vector<int> >& adj_list);

    /**
     * @brief Returns the number of vertices in the graph 
     * @return number of vertices in the graph
     */
    int get_n_vertices();

    /**
     * @brief Returns adjacence list of the graph
     * @return adjacence list of the graph
     */
    const std::vector< std::vector<int> > get_adj_list();

    /**
     * @brief Finds existing hamiltonian paths in the graph
     * @details The user can specify the method used internally to find hamiltonian paths.
     * Two options are offered: using a SAT solver or a backtracking algorithm.
     * 
     * @param source source of the hamiltonian paths
     * @param last destination of the hamiltonian paths
     * @param sat whether to use a SAT solver in the solution or not.
     * If false, a backtracking algorithm will be used.
     * @param count whether to count the total number of existing hamiltonian paths or not.
     * @return list of paths
     */
    std::vector< std::vector<int> >& ham_path(int source,
                                              int last,
                                              bool sat=true,
                                              bool count=false,
                                              const std::vector< std::pair<int,int> >& map = {},
                                              const std::vector< std::pair<int,int> >& diamonds = {});

    /**
     * @brief Finds existing hamiltonian cycles in the graph
     * @details The user can specify the method used internally to find hamiltonian cycles.
     * Two options are offered: using a SAT solver or a backtracking algorithm.
     * 
     * @param sat whether to use a SAT solver in the solution or not.
     * If false, a backtracking algorithm will be used.
     * @param count whether to count the total number of existing hamiltonian cycles or not.
     * @return list of cycles
     */
    std::vector< std::vector<int> >& ham_cycle(bool sat=true,
                                               bool count=false,
                                               const std::vector< std::pair<int,int> >& map = {},
                                               const std::vector< std::pair<int,int> >& diamonds = {});
    /**
     * @brief Returns all up to k hamiltonian paths from source to last in the graph
     * 
     * @param k maximum number of hamiltonian paths to be returned
     * @param source source of the hamiltonian paths
     * @param last destination of the hamiltonian paths
     * @param map list of pairs of integers of the form (i, v) representing
     * the condition "vertex v must be visited at instant i"
     * @param diamonds list of diamonds in the form of a list of pairs of integers of the form (u, v) representing
     * the condition "vertex v and vertex u must be visited consecutively, in any order"
     * @return a list of all the distinct hamiltonian paths from the source to the last vertex considering
     * the constraints imposed by the maps and the diamonds. If more than k paths exist, only the first
     * k paths found will be returned
     */
    std::vector<int>& k_paths(int k,
                                              int source,
                                              int last,
                                              const std::vector< std::pair<int, int> >& map,
                                              const std::vector< std::pair<int, int> >& diamonds);

    void unique_sol(int first, int last, std::ofstream &ofile);
};

#endif //RIKUDOSOLVER_GRAPH_H
