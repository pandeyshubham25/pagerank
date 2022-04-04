#include <iostream>
#include <fstream>
#include <list>
#include <vector>
#include <unordered_map>
#include <omp.h>
#include <time.h>

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
    num_threads = atoi(argv[1]);
    num_iters = atoi(argv[2]);
    alpha = atof(argv[3]);
    input_file = argv[4];



    //PHASE 1 : Read graph from input file
    cout<<"Loading graph data..."<<endl;

    clock_t tStart = clock();
    int num_pages = 0;
    unordered_map <int, Page> pages; //mapping page ID to its struct

    ifstream cin(input_file);

    int id, out;
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
    }


    cout<< "Graph data loaded in "<<(double)(clock() - tStart)/CLOCKS_PER_SEC<<"s"<<endl;
    cout<<"Loaded "<<num_pages<<" pages"<<endl;

    /*
    // initialize pageRank
    float equal_prob = 1.0 / num_pages;
    for(auto& x : pages)
        x.second.score = equal_prob;

    float broadcastScore;

     omp_set_num_threads(num_threads);
    //PHASE 2 : Page ID Distribution
    //TODO: ID distribution logic for different threads comes here.


    //PHASE 3 : iteratively update ranks
    //Once page distribution is done, we can spawn the threads
    //The code below simply distributes the pages across threads based on page id, we need to change it
    clock_t aStart = clock();
    for(int j=0;j<num_iters;j++){
        clock_t tStart = clock();
        broadcastScore = 0.0;

        // evaluate the score for each node
#pragma omp parallel for reduction(+: broadcastScore) schedule (dynamic, 32)
        for(int i=0;i<pages.size();i++){

            // find the ID corresponding to the i-page
            int idx = i;

            // used to store the new score
            pages[idx].score_new = 0.0;

            // if the node has no outgoing edges, then add its value to the broadcast score
            if(!pages[idx].outdegree)
                broadcastScore += pages[idx].score;

            // iterate over all the vertices with an incoming edge to compute the new value of the vertices
            if(pages[idx].indegree>0){
                for(list <int> :: iterator it = pages[idx].in_ids.begin(); it != pages[idx].in_ids.end(); ++it)
                    pages[idx].score_new += pages[*it].score / pages[*it].outdegree;
            }

            // apply pageRank equation
            pages[idx].score_new = alpha * pages[idx].score_new + (1.0 - alpha) / num_pages;
        }

        // update broadcast score
        broadcastScore = alpha * broadcastScore / num_pages;
        for(auto& x : pages){

            // add the global broadcast score to each edge to compute its final score
            x.second.score = x.second.score_new + broadcastScore;
        }
        cout<<"Iteration "<<j<<" completed in "<<(double)(clock() - tStart)/CLOCKS_PER_SEC<<"s"<<endl;
    }
    cout<<"Algorithm terminated in "<<(double)(clock() - tStart)/CLOCKS_PER_SEC<<"s"<<endl;

    //PHASE 4: Write the result

    clock_t rStart = clock();
    cout<<"Writing result..."<<endl;
    string result_path = string("output_") + string(input_file);
    ofstream result_file(result_path);
    for(auto& x : pages){
        result_file << x.first<<"\t"<<x.second.score<<"\n";
    }
    cout<<"Results written in "<<(double)(clock() - rStart)/CLOCKS_PER_SEC<<"s"<<endl;
    result_file.close();
     */
    return 0;
}