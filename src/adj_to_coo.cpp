#include <iostream>
#include <fstream>
#include <list>
#include <vector>
#include <unordered_map>
#include <chrono>

using namespace std;

struct Page {
    //SP: should we keep this as vector or as a list ? is there be a specific order of Ids that can better the run time ?
    vector <int> in_ids; //ids of pages that point to this page
    int indegree;
    int outdegree;
    double score; //page-rank score
    double score_new; //used as temp variable to update the score
    Page(){
        indegree = 0;
        outdegree = 0;
        score = 0.0;
    }
};

//Global Variables
int num_threads; //number of threads - 1st arg in main()
int num_iters; //number of iterations - 2nd argu in main()
double alpha; //alpha factor - 3rd arg in main()
char *input_file; //input graph file name - 4th arg in main()


int main(int argc, char** argv){
    //PHASE 0 : Read commandline args and initialize global params
    //TODO: Add exception handling for parsing cline args
    input_file = argv[1];



    //PHASE 1 : Read graph from input file

    //print input file name follwoed by num_itera and alpha
    //cout << "Parallel parameters: " << input_file << " " << num_iters << " " << alpha << endl;

    auto tStart = std::chrono::high_resolution_clock::now();
    int num_pages = 0;
    unordered_map <int, Page> pages; //mapping page ID to its struct

    ifstream cin(input_file);

    int id, out;

    //count of non-zero entries in adj matrix
    int nz=0;
    while(cin>>id) {
        cin >> out;
        if(!pages.count(id)) {
            pages[id] = Page();
            num_pages++;
        }
        if(!pages.count(out)) {
            pages[out] = Page();
            num_pages++;
        }
        pages[out].indegree++;
        pages[out].in_ids.push_back(id);
        pages[id].outdegree++;
        nz++;
    }


    // Write these values in coordinate format to another file
    string result_path = input_file + string("_coo") ;
    FILE *fptr = fopen(result_path.c_str(), "w");

    // edge is from u to v
    int u,v;

    //val is the row normalized value in adjacency matrix
    double val;

    //first line in the file describes number of row, cols and non zero values in adj matrix
    fprintf(fptr, "%d %d %d\n", num_pages, num_pages, nz);
    for(auto& x : pages){
        v = x.first;
        for(int i=0; i<x.second.in_ids.size(); i++) {
            u = x.second.in_ids[i];
            val = 1 / (1.0 * pages[u].outdegree);
            fprintf(fptr, "%d %d %.10f\n", u, v, val);
        }
    }
    fclose(fptr);

    return 0;
}