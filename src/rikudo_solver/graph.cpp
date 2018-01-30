//
// Created by:
//    Cauim de S. Lima - cauimsouza@gmail.com
//    Victor Hugo Vianna Silva - victor.vianna10@gmail.com
//

#include "graph.h"
#include <climits>
#include <utility>
#include <fstream>
#include <cstdio>
#include <vector>
#include <utility>
#include <iostream>
#include <queue>
#include <sstream>
#include <random>
#include <algorithm>
#include <libgen.h>
#include <unistd.h>



#define sat_input "sat_input.txt"
#define sat_output "sat_output.txt"
#define sat_aux_input "sat_aux_input.txt"
#define sat_solver "cryptominisat5_simple"


int Graph::encode(int ith, int vertex, bool offset)
{
    int code = ith * n_vertices + vertex + 1;
    if(offset)  code += n_vertices * n_vertices;
    return code;
}

void Graph::decode(int var, int &i, int &v){
    var--;
    i = var / n_vertices;
    v = var % n_vertices;
}

bool Graph::backtracking(int v,
                         int n,
                         int last,
                         bool count)
{
    visited[v] = true;
    path[n] = v;

    if (n == n_vertices - 1)
    {
        visited[v] = false;
        if(v == last){
            paths.push_back(path);
            return true;
        }
        
        return false;
    }

    for (int neighbor : adj_list[v])
    {
        if (!visited[neighbor] && backtracking(neighbor, n + 1, last, count) && !count)
            return true;
    }

    visited[v] = false;
    return false;
}

bool Graph::valid(const std::vector<int> &candidate,
                  const std::vector< std::pair<int,int> >& map,
                  const std::vector< std::pair<int,int> >& diamonds)
{    
    for(auto pair : map){
        int ith = pair.first;
        int vertex = pair.second;
        if(candidate[ith] != vertex)    return false;
    }

    for(auto pair : diamonds){
        int u = pair.first;
        int v = pair.second;
        for(int ith = 0; ith < candidate.size(); ith++)
            if(candidate[ith] == u){
                if(ith > 0 && candidate[ith-1] == v)    continue;
                if(ith < candidate.size() - 1 && candidate[ith+1] == v) continue;
                return false;
            }
    }

    return true;
}

Graph::Graph(std::ifstream &file)
{
    int n_vertices;
    file >> n_vertices;

    if(n_vertices <= 0)
        throw "Number of vertices should be a positive integer.";

    this->n_vertices = n_vertices;
    adj_list.assign(static_cast<unsigned long>(n_vertices), std::vector<int>());

    while (true)
    {
        int a, b;
        file >> a;
        if (a < 0)
            break;
        file >> b;

        if(a >= n_vertices || b < 0 || b >= n_vertices)
            throw "Invalid vertex index";

        adj_list[a].push_back(b);
    }
}

Graph::Graph(const std::vector< std::vector<int> >& adj_list){
    this->adj_list = adj_list;
    n_vertices = adj_list.size();
}

int Graph::get_n_vertices(){
    return n_vertices;
}

const std::vector< std::vector<int> > Graph::get_adj_list()
{
    return adj_list;
}

// every vertex visited
void Graph::condition1(std::vector< std::vector<int> >& clauses){
    std::vector<int> clause;

     // every vertex visited
    for(int i = 0; i < n_vertices; i++){
        clause.clear();
        for(int j = 0; j < n_vertices; j++){
            int id = encode(j, i);
            clause.push_back(id);
        }
        clauses.push_back(clause);
    }
}

// every vertex at most once
void Graph::condition2(std::vector< std::vector<int> >& clauses){
    std::vector<int> clause;
    for(int i = 0; i < n_vertices; i++){
        for(int j = 0; j < n_vertices; j++){
            for(int k = j + 1; k < n_vertices; k++){
                clause.clear();
                int id_1 = -encode(j, i);
                int id_2 = -encode(k, i);
                clause.push_back(id_1);
                clause.push_back(id_2);
                clauses.push_back(clause);                
            }
        }
    }
}

// in every instant a vertex
void Graph::condition3(std::vector< std::vector<int> >& clauses){
    std::vector<int> clause;

    for(int i = 0; i < n_vertices; i++){
        clause.clear();
        for(int j = 0; j < n_vertices; j++){
            int id = encode(i, j);
            clause.push_back(id);
        }
        clauses.push_back(clause);
    }
}

// in every instant at most one vertex
void Graph::condition4(std::vector< std::vector<int> >& clauses){
    std::vector<int> clause;
    for(int i = 0; i < n_vertices; i++){
        for(int j = 0; j < n_vertices; j++){
            for(int k = j+1; k < n_vertices; k++){
                clause.clear();
                int id_1 = -encode(i, j);
                int id_2 = -encode(i, k);
                clause.push_back(id_1);
                clause.push_back(id_2);
                clauses.push_back(clause); 
            }
        }
    }
}

// adds clauses so that x_i_j and x_i+1_k => there is an edge from j to k
void Graph::condition5(std::vector< std::vector<int> >& clauses){
    std::vector<int> clause;
    for (int ith = 0; ith < n_vertices - 1; ith++)
    {
        for (int vertex = 0; vertex < n_vertices; vertex++)
        {
            int var_id = encode(ith, vertex);
            clause.clear();
            clause.push_back(-var_id);
            for (int neighbor : adj_list[vertex])
            {
                int neighbor_id = encode(ith + 1, neighbor);
                clause.push_back(neighbor_id);
            }
            clauses.push_back(clause);
        }
    }
}

// add map clauses
void Graph::condition6(std::vector< std::vector<int> >& clauses,
    const std::vector< std::pair<int,int> >& map)
{
    std::vector<int> clause;
    for(auto ith_vertex : map){
        clause.clear();
        int var_id = encode(ith_vertex.first, ith_vertex.second);
        clauses.push_back(clause);
    }
}

// add diamonds clauses
void Graph::condition7(std::vector< std::vector<int> >& clauses,
    const std::vector< std::pair<int,int> >& diamonds)
{
    std::vector<int> clause;
    for(auto vi_vj : diamonds){
        for(int ith = 0; ith < n_vertices; ith++){
            clause.clear();

            int vi_id = encode(ith, vi_vj.first);
            clause.push_back(-vi_id);
            if(ith > 0){
                int vj_id = encode(ith-1, vi_vj.second);
                clause.push_back(vj_id);
            }
            if(ith < n_vertices-1){
                int vj_id = encode(ith+1, vi_vj.second);
                clause.push_back(vj_id);
            }
            clauses.push_back(clause);
        }
    }
}

// transitivity
void Graph::condition8(std::vector< std::vector<int> >& clauses){
    std::vector<int> clause;
    for(int i = 0; i < n_vertices; i++){
        for(int j = 0; j < n_vertices; j++){
            for(int k = 0; k < n_vertices; k++){
                if(i == j || i == k || j == k)    continue;
                int v1 = -encode(i, j, true);
                int v2 = -encode(j, k, true);
                int v3 = encode(i, k, true);
                clause.clear();
                clause.push_back(v1);
                clause.push_back(v2);
                clause.push_back(v3);
                clauses.push_back(clause);
            }
        }
    }
}


// total order relation
void Graph::condition9(std::vector< std::vector<int> >& clauses){
    std::vector<int> clause;
    for(int i = 0; i < n_vertices; i++){
        for(int j = 0; j < n_vertices; j++){
            if(i == j)  continue;
            int v1 = encode(i, j, true);
            int v2 = encode(j, i, true);
            clause.clear();
            clause.push_back(v1);
            clause.push_back(v2);
            clauses.push_back(clause);
            clause.clear();
            clause.push_back(-v1);
            clause.push_back(-v2);
            clauses.push_back(clause);
        }
    }
}


// correlation
void Graph::condition10(std::vector< std::vector<int> >& clauses){
    std::vector<int> clause;
    for(int t = 0; t < n_vertices - 1; t++){
        for(int u = 0; u < n_vertices; u++){
            for(int v = 0; v < n_vertices; v++){
                int v1 = -encode(t, u);
                int v2 = -encode(t+1, v);
                int v3 = encode(u, v, true);
                clause.clear();
                clause.push_back(v1);
                clause.push_back(v2);
                clause.push_back(v3);
                clauses.push_back(clause);
            }
        }
    }
}

// first
void Graph::condition11(std::vector< std::vector<int> >& clauses, int source){
    std::vector<int> clause;
    for(int i = 0; i < n_vertices; i++){
        if(i == source) continue;
        int v = encode(source, i, true);
        clause.clear();
        clause.push_back(v);
        clauses.push_back(clause);
    }
}

// last
void Graph::condition12(std::vector< std::vector<int> >& clauses, int dest){
    std::vector<int> clause;
    for(int i = 0; i < n_vertices; i++){
        if(i == dest) continue;
        int v = encode(i, dest, true);
        clause.clear();
        clause.push_back(v);
        clauses.push_back(clause);
    }
}

// first 2
void Graph::condition13(std::vector< std::vector<int> >& clauses, int source){
    std::vector<int> clause;
    clause.clear();
    int x = encode(0, source);
    clause.push_back(x);
    clauses.push_back(clause);
}

// last 2
void Graph::condition14(std::vector< std::vector<int> >& clauses, int dest){
    std::vector<int> clause;
    clause.clear();
    int x = encode(n_vertices-1, dest);
    clause.push_back(x);
    clauses.push_back(clause);
}



void Graph::construct_sat(int source, int dest,
                          const std::vector< std::pair<int,int> >& map,
                          const std::vector< std::pair<int,int> >& diamonds)
{
    std::vector<int> clause;
    std::vector< std::vector<int> > clauses;

    condition1(clauses);
    condition2(clauses);
    condition3(clauses);
    condition4(clauses); // useful
    condition5(clauses);
    condition6(clauses, map);
    condition7(clauses, diamonds);
    condition13(clauses, source); // not useful
    condition14(clauses, dest); // not useful
    // order relation 
    
    condition8(clauses);
    condition9(clauses);
    condition10(clauses);
    condition11(clauses, source);
    condition12(clauses, dest);
    
    

    write_sat(clauses);
}

void Graph::write_sat(std::vector<std::vector<int> > &clauses){
    std::ofstream ofs(get_path(sat_input));

    ofs << "p cnf " << n_vertices*n_vertices*2 << " " << (int) clauses.size() << "\n";
    for(auto c : clauses){
        bool first = true;
        for(auto v : c){
            if(!first)  ofs << " ";
            ofs << v;
            first = false;
        }
        ofs << " 0\n";
    }

    ofs.close();

    std::cout << "sat formula written\n";
}

std::vector<int>& Graph::read_sol()
{
    path.assign(n_vertices, -1);

    std::ifstream myfile;
    myfile.open(get_path(sat_output));

    std::string line;

    while(getline(myfile, line)){
        std::istringstream iss(line);

        std::string type;
        iss >> type;

        if(type == "v"){
            int val;
            while(iss >> val){
                if(val > 0 && val <= n_vertices * n_vertices){
                    int i, j;
                    decode(val, i, j);
                    path[i] = j;
                }
            }
        }
    }


    myfile.close();

    std::cout << "sat file read\n";
    
    for(int i = 0; i < n_vertices; i++){
        if(path[i] == -1) {
            path.clear();
            return path;
        }
    }

    return path;
}

std::vector< std::vector<int> >&
Graph::ham_path_sat(int first,
                    int last,
                    bool count,
                    const std::vector< std::pair<int,int> >& map,
                    const std::vector< std::pair<int,int> >& diamonds)
{
    paths.clear();
    construct_sat(first, last, map, diamonds);
    solve_sat();
    path = read_sol();

    while(!path.empty()){
        paths.push_back(path);
        
        if(!count)  break;

        int nvars, nclauses;
        auto ban = create_ban(path);
        extend_sat(ban, nvars, nclauses);
        recopy(nvars, nclauses);
        solve_sat();
        path = read_sol();
    }
    
    return paths;
}

void Graph::solve_sat(){
    char buffer[1024];  
    int buffer_size = 1024;
    
    readlink("/proc/self/exe", buffer, buffer_size);
    char *path_s = dirname(buffer);
    char *parpath_s = dirname(path_s);
    std::string path(path_s);
    std::string sat_path(std::string(parpath_s) + "/lib/" + sat_solver);
    std::string inp_path(path + "/bin/" + std::string(sat_input));
    std::string out_path(path + "/bin/" + std::string(sat_output));
    std::cout << inp_path << "\n" << out_path << "\n";

    std::string cmd = sat_path + " " + inp_path + " > " + out_path; 
    std::system(cmd.data());
}

std::vector< std::vector<int> >&
Graph::ham_cycle_sat(bool count,
                     const std::vector< std::pair<int,int> >& map,
                     const std::vector< std::pair<int,int> >& diamonds)
{
    int min_deg_v = INT_MAX;
    int min_deg = -1;
    for(int i = 0; i < adj_list.size(); i++){
        if(adj_list[i].size() < min_deg){
            min_deg = adj_list[i].size();
            min_deg_v = i;
        }
    }
    std::vector< std::vector<int> > sol;

    for(int source : adj_list[min_deg_v]){
        paths = ham_path_sat(source, min_deg_v, count, map, diamonds);
        sol.insert(sol.begin(), paths.begin(), paths.end());
        if(!count && !sol.empty())  break;
    }
    
    paths = sol;
    return paths;
}


std::vector< std::vector<int> >& Graph::ham_path_bt(int source,
                                                     int last,
                                                     bool count,
                                                     const std::vector< std::pair<int,int> >& map,
                                                     const std::vector< std::pair<int,int> >& diamonds)
{
    paths.clear();
    visited.assign(n_vertices, false);
    path.resize(n_vertices);

    backtracking(source, 0, last, count);
    
    std::vector< std::vector<int> > valid_paths;
    for(auto candidate : paths)
        if(valid(candidate, map, diamonds)) valid_paths.push_back(candidate);
    
    paths = valid_paths;

    return paths;
}

std::vector< std::vector<int> >&
Graph::ham_cycle_bt(bool count,
                    const std::vector< std::pair<int,int> >& map,
                    const std::vector< std::pair<int,int> >& diamonds)
{   
    int min_deg_v = INT_MAX;
    int min_deg = -1;
    for(int i = 0; i < adj_list.size(); i++){
        if(adj_list[i].size() < min_deg){
            min_deg = adj_list[i].size();
            min_deg_v = i;
        }
    }
    std::vector< std::vector<int> > sol;

    for(int source : adj_list[min_deg_v]){
        paths = ham_path_bt(source, min_deg_v, count, map, diamonds);
        for(auto candidate : paths)
            if(valid(candidate, map, diamonds)){
                sol.push_back(candidate);
                if(count) break;
            }
        if(!count && !sol.empty())  break;
    }
    
    paths = sol;
    return paths;
}

std::vector< std::vector<int> >& Graph::ham_path(int source,
                                                 int last,
                                                 bool sat,
                                                 bool count,
                                                 const std::vector< std::pair<int,int> >& map,
                                                 const std::vector< std::pair<int,int> >& diamonds)
{
    if(sat) return ham_path_sat(source, last, count, map, diamonds);
    else    return ham_path_bt(source, last, count, map, diamonds);
}

std::vector< std::vector<int> >& Graph::ham_cycle(bool sat,
                                                 bool count,
                                                 const std::vector< std::pair<int,int> >& map,
                                                 const std::vector< std::pair<int,int> >& diamonds)
{
    if(sat) return ham_cycle_sat(count, map, diamonds);
    else    return ham_cycle_bt(count, map, diamonds);
}


std::vector<int>&
Graph::k_paths(int k,
               int source,
               int last,
               const std::vector< std::pair<int, int> >& map,
               const std::vector< std::pair<int, int> >& diamonds)
{
    paths = ham_path_sat(source, last, false, map, diamonds);
    if(paths.size() > k){
        path.clear();
        return path;
    }

    return paths[0]; 
}

void Graph::unique_sol(int first, int last, std::ofstream &ofile){
    construct_sat(first, last);
    solve_sat();
    auto orig_path = read_sol();

    if(orig_path.empty()){
        write_min_cons(orig_path, orig_path, -1, ofile);
        return;
    }

    std::string ban = create_ban(orig_path);
    int n_vars, n_clauses;
    extend_sat(ban, n_vars, n_clauses);

    recopy(n_vars, n_clauses);
    solve_sat();
    auto extra_path = read_sol();
    if(extra_path.empty()){
        write_min_cons(orig_path, extra_path, -1, ofile);
        return;
    }
    
    std::vector<int> cons = create_cons(n_vertices);

    
    int lo = 0; // adding until lo-1 constraints will always produce solution
    int hi = cons.size() - 1; // adding hi or more will not produce more solutions
    while(lo < hi){
        int mid = lo + (hi-lo)/2;

        recopy(n_vars, n_clauses + mid + 1);
        add_cons(orig_path, cons, mid);
        solve_sat();
        auto extra_path = read_sol();  

        if(extra_path.empty()){
            hi = mid;
        }
        else{
            lo = mid + 1;
        }
    }

    // in the end we have lo == hi
    write_min_cons(orig_path, cons, lo, ofile);
}

void Graph::extend_sat(std::string s, int &n_vars, int &n_clauses){
    std::ifstream ifile(get_path(sat_input));
    std::ofstream ofile(get_path(sat_aux_input));
    
    std::string line;
    std::string token;

    getline(ifile, line);
    std::istringstream iss(line);
    
    for(int i = 0; i < 2; i++){
        iss >> token;
    }
    iss >> n_vars;
    iss >> n_clauses;
    n_clauses++;
    
    while(getline(ifile, line)){
        ofile << line << "\n";
    }

    ofile << s;

    ofile.close();
    ifile.close();
}

std::string Graph::create_ban(std::vector<int> &orig_path){
    int n_vertices = orig_path.size();
    std::string ban = "";
    for(int i = 1; i < n_vertices - 1; i++){
        int var = -encode(i, orig_path[i]);
        ban += std::to_string(var) + " ";
    }
    ban += "0\n";

    return ban;
}

std::vector<int> Graph::create_cons(int n_vertices){
    std::vector<int> cons;
    // add diamonds 
    for(int i = 0; i < n_vertices - 1; i++)
        cons.push_back(i);

    // add map
    for(int i = 1; i < n_vertices - 1; i++)
        cons.push_back(-i);
    
    std::random_device rd;
    std::mt19937 g(rd());
 
    std::shuffle(cons.begin(), cons.end(), g);

    return cons;
}

std::string get_path(std::string file_name){
    char buffer[1024];  
    int buffer_size = 1024;
    
    readlink("/proc/self/exe", buffer, buffer_size);
    std::string path(dirname(buffer));
    path = path + "/" + file_name;
    return path;
}

void Graph::recopy(int n_vars, int n_clauses){
    std::ifstream ifile(get_path(sat_aux_input));
    std::ofstream ofile(get_path(sat_input));
    
    ofile << "p cnf " << n_vars << " " << n_clauses << "\n";

    std::string line;
    while(getline(ifile, line)){
        ofile << line << "\n";
    }

    ofile.close();
    ifile.close();
}

void Graph::add_cons(std::vector<int>& orig_path, std::vector<int>& cons, int pos){
    std::ofstream ofile;
    ofile.open(get_path(sat_input), std::ofstream::app);

    int n_vertices = orig_path.size();
    for(int i = 0; i <= pos; i++){
        int con = cons[i];
        if(con >= 0){
            int u = orig_path[con];
            int v = orig_path[con+1];
            std::string diam = diam_s(u, v, n_vertices);
            ofile << diam;
        }
        else{
            con = -con;
            int var = encode(con, orig_path[con]);
            ofile << std::to_string(var) << " 0\n";
        }
    }

    ofile.close();
}

std::string Graph::diam_s(int u, int v, int n_vertices){
    std::string s;
    for(int i = 0; i < n_vertices; i++){

        int u_id = -encode(i, u);
        s += std::to_string(u_id);
        if(i > 0){
            int v1_id = encode(i-1, v);
            s += " " + std::to_string(v1_id);
        }
        if(i < n_vertices-1){
            int v2_id = encode(i+1, v);
            s += " " + std::to_string(v2_id);                
        }
        s += " 0\n";
    }
    return s;
}

void Graph::write_min_cons(std::vector<int>& orig_path, std::vector<int>& cons, int num, std::ofstream &ofile){
    for(int v : orig_path){
        ofile << v << "\n";
    }
    
    ofile << "-1\n";

    for(int i = 0; i <= num; i++){
        int con = cons[i];

        if(con < 0){
            con = -con;
            ofile << orig_path[con] << " " << con + 1 << "\n";
        }
    }

    ofile << "-1\n";

    for(int i = 0; i <= num; i++){
        int con = cons[i];

        if(con >= 0){
            int u = orig_path[con];
            int v = orig_path[con+1];
            ofile << u << " " << v << "\n";
        }
    }

    ofile << "-1\n";
}
