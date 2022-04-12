#include <iostream>
#include <fstream>
#include <list>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <omp.h>
#include <time.h>

using namespace std;

#define CACHE_SIZE 128000/2 //L1 cache used for M1 since L2 is shared.
//Or sun at 3MB/2 for the allocated shared of shared L2


struct in_edge {
    int pair_id;
    bool in_numa;
};


struct Page {
    //SP: should we keep this as vector or as a list ? is there be a specific order of Ids that can better the run time ?
    int my_id;
    vector <in_edge> in_ids; //ids of pages that point to this page
    int indegree;
    int outdegree;
    double score; //page-rank score
    double score_new; //used as temp variable to update the score
    Page(int id){
        my_id = id;
        indegree = 0;
        outdegree = 0;
        score = 0.0;
    }
};

struct Edge {
    Page *first;
    int second;
};

template<>
struct std::hash<Page> {
    std::size_t operator()(Page const &page) const {
        return page.my_id;
    }
};


template<>
struct std::hash<Edge> {
    std::size_t operator()(Edge const &edge) const {
        return std::hash<Page>()(*edge.first) << 32 | edge.second;
    }
};


bool operator==(const Page& lhs, const Page& rhs) {
    return lhs.my_id == rhs.my_id;
}
bool operator==(const Edge& lhs, const Edge& rhs) {
    return (*lhs.first == *rhs.first) && (lhs.second == rhs.second);
}

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

    //print input file name follwoed by num_itera and alpha
    cout << "Parallel parameters: " << input_file << " " << num_iters << " " << alpha << endl;

    auto tStart = std::chrono::high_resolution_clock::now();
    int num_pages = 0;
    unordered_map <int, Page*> pages; //mapping page ID to its struct

    ifstream cin(input_file);

    int id, out;
    long long edge_count = 0;
    while(cin>>id) {
        cin >> out;
        if(!pages.count(id)) {
            pages[id] = new Page(id);
            num_pages++;
        }
        if(!pages.count(out)) {
            pages[out] = new Page(id);
            num_pages++;
        }
        pages[out]->indegree++;
        pages[out]->in_ids.push_back({id, false});
        pages[id]->outdegree++;
        edge_count++;
    }


    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - tStart);
    cout << "Load time: " << elapsed.count()*1e-9 <<endl;
    //cout<<"Loaded "<<num_pages<<" pages"<<endl;


    // initialize pageRank
    float equal_prob = 1.0 / num_pages;
    for(auto& x : pages)
        x.second->score = equal_prob;

    float broadcastScore;

    //omp_set_num_threads(num_threads);
    //PHASE 2 : Page ID Distribution
    //TODO: ID distribution logic for different threads comes here.

    long long edges_per_thread = edge_count / num_threads + 1;//The last thread may get slightly shortchanged; this should be fine on large graphs
    std::vector<std::vector< std::unordered_set<Page *> *> *> partitions;
    std::vector<std::unordered_set<Page *> *> *current_partition = new std::vector<std::unordered_set<Page *> *>();
    std::unordered_set<Page *> *current_cacheable_partition = new std::unordered_set<Page *>();
    current_partition->push_back(current_cacheable_partition);
    partitions.push_back(current_partition);
    long long edges_left = edges_per_thread;
    long long vertices_per_partition = CACHE_SIZE / sizeof(Page);
    long long vertices_left = vertices_per_partition;
    long long start_id = 0;
    for (auto page_entry : pages) {
        int current_vertex = page_entry.first;
        if (edges_left <= 0) {
            for (std::unordered_set<Page *> *cacheable_partition : *current_partition) {
                for (Page *p : *cacheable_partition) {
                    for (in_edge in : p->in_ids) {
                        in.in_numa = (in.pair_id > start_id) && (in.pair_id < current_vertex);
                    }
                }
            }
            current_partition = new std::vector<std::unordered_set<Page *> *>();
            current_cacheable_partition = new std::unordered_set<Page *>();
            current_partition->push_back(current_cacheable_partition);
            partitions.push_back(current_partition);
            edges_left = edges_per_thread;
            vertices_left = vertices_per_partition;
            start_id = current_vertex;
        } else if (vertices_left <= 0) {
            current_cacheable_partition = new std::unordered_set<Page *>();
            current_partition->push_back(current_cacheable_partition);
            vertices_left = vertices_per_partition;
        }
        current_cacheable_partition->insert(page_entry.second);
        edges_left -= page_entry.second->outdegree;
        vertices_left -= 1;
    }

    //PHASE 3 : iteratively update ranks
    //Once page distribution is done, we can spawn the threads
    //The code below simply distributes the pages across threads based on page id, we need to change it
     tStart = std::chrono::high_resolution_clock::now();
    omp_set_num_threads(num_threads);


    for(int j=0;j<num_iters;j++){
        clock_t tStart = clock();
        broadcastScore = 0.0;

        // evaluate the score for each node
        #pragma omp parallel reduction(+: broadcastScore)
        {
            int thread_num = omp_get_thread_num();
            std::vector<std::unordered_set<Page * > *> *numa_partition = partitions[thread_num];
            std::unordered_set<Edge> sync_later;
            for(std::unordered_set<Page *> *cacheable_partition : *numa_partition){
                for (Page *current_page : *cacheable_partition) {

                    // used to store the new score
                    current_page->score_new = 0.0;

                    // if the node has no outgoing edges, then add its value to the broadcast score
                    if(!current_page->outdegree)
                        broadcastScore += current_page->score;

                    // iterate over all the vertices with an incoming edge to compute the new value of the vertices
                    if(current_page->indegree>0){
                        for(vector <in_edge> :: iterator it = current_page->in_ids.begin(); it != current_page->in_ids.end(); ++it) {
                            if (it->in_numa) {
                                current_page->score_new += pages[it->pair_id]->score / pages[it->pair_id]->outdegree;
                            } else {
                                sync_later.insert(Edge({current_page, it->pair_id}));
                            }
                        }
                    }

                }
            }
            for (Edge edge : sync_later) {//The paper syncs in a single edge across each partition; this will force us to break out of the omp framework and restart from scratch
                edge.first->score_new += pages[edge.second]->score / pages[edge.second]->outdegree;
            }

            for(std::unordered_set<Page *> *cacheable_partition : *numa_partition){
                for (Page *current_page : *cacheable_partition) {
                    // apply pageRank equation
                    current_page->score_new = alpha * current_page->score_new + (1.0 - alpha) / num_pages;
                }
            }
        }

        // update broadcast score
        broadcastScore = alpha * broadcastScore / num_pages;
        for(auto& x : pages){

            // add the global broadcast score to each edge to compute its final score
            x.second->score = x.second->score_new + broadcastScore;
        }
        //cout<<"Iteration "<<j<<" completed in "<<(double)(clock() - tStart)/CLOCKS_PER_SEC<<"s"<<endl;
    }

    //print time taken to converge
    end = std::chrono::high_resolution_clock::now();
    elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - tStart);
    cout << "Iteration time: " << elapsed.count() *1e-9 <<endl;

    //PHASE 4: Write the result


    string result_path = input_file + string("_prank_edge_partition_ranks") ;
    ofstream result_file(result_path);
    for(auto& x : pages){
        result_file << x.first << " " << x.second->score << "\n";
    }
    result_file.close();

    return 0;
}
