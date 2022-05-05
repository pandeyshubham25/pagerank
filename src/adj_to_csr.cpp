#include <iostream>
#include <fstream>
#include <list>
#include <vector>
#include <map>
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

void print_pages(map <int, Page> pages, FILE *fptr) {
    // edge is from col to row
    int row, col;

    //val is the row normalized value in adjacency matrix
    double val;
    long long cumulative = 0;
    std::vector <long long> row_breaks;
    std::vector <float> weights;
    for(auto& x : pages){
        row_breaks.push_back(cumulative);
        row = x.first;
        for(int i=0; i<x.second.in_ids.size(); i++) {
            col = x.second.in_ids[i];
            //val = 1 / (1.0 * pages[col].outdegree);
            weights.push_back(1.0 / (float) pages[col].outdegree);
            fprintf(fptr, "%d ", col);
        }
        cumulative += x.second.in_ids.size();
    }
    row_breaks.push_back(cumulative);
    fprintf(fptr, "\n");

    for (float weight : weights) {
        fprintf(fptr, "%f ", weight);
    }
    fprintf(fptr, "\n");

    for (long long row_break : row_breaks) {
         fprintf(fptr, "%lld ", row_break);
    }
    fprintf(fptr, "\n");

}

int main(int argc, char** argv){
    //PHASE 0 : Read commandline args and initialize global params
    //TODO: Add exception handling for parsing cline args
    input_file = argv[1];
    char *output_file = argv[2];



    //PHASE 1 : Read graph from input file

    //print input file name follwoed by num_itera and alpha
    //cout << "Parallel parameters: " << input_file << " " << num_iters << " " << alpha << endl;

    auto tStart = std::chrono::high_resolution_clock::now();
    int num_pages = 0;
    map <int, Page> pages; //mapping page ID to its struct

    ifstream cin(input_file);

    long from, to;
    long nz = 0;

    //count of non-zero entries in adj matrix
    while(cin>>from) {
        cin >> to;
        if(!pages.count(from)) {
            pages[from] = Page();
            num_pages++;
        }
        if(!pages.count(to)) {
            pages[to] = Page();
            num_pages++;
        }
        pages[from].outdegree++;
        pages[to].in_ids.push_back(from);
        pages[to].indegree++;
        nz++;
    }


    // Write these values in coordinate format to another file
    string result_path = output_file;//input_file + string("_csr") ;
    FILE *fptr = fopen(result_path.c_str(), "w");

    //first line in the file describes number of row, cols and non zero values in adj matrix
    fprintf(fptr, "%d %ld\n", num_pages, nz);

    print_pages(pages, fptr);

    fclose(fptr);

    return 0;
}
