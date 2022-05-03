#include <iostream>
#include <fstream>
#include <list>
#include <vector>
#include <unordered_map>
#include <omp.h>
#include <time.h>

using namespace std;

//To store matrix in coo form
int rows, cols, nonzeros;
int *row_indexes;
int *col_indexes;
float *values;


//Global Variables
int num_threads; //number of threads - 1st arg in main()
int num_iters; //number of iterations - 2nd argu in main()
double alpha; //alpha factor - 3rd arg in main()
char *input_file; //input graph file name - 4th arg in main()


void multiply(float *B, float *C) {

    // for storing intermediate result for each thread
    float **temp = new float*[num_threads];
    for(int i = 0; i < num_threads; ++i)
        temp[i] = new float[cols];

    #pragma omp parallel for
    for (int i=0; i<nonzeros; i++) {
        int row_index = row_indexes[i];
        int col_index = col_indexes[i];
        float val = values[i];
        int tid = omp_get_thread_num();
        temp[tid][row_index] += B[col_index] * val;
    }

    #pragma omp parallel for
    for (int i=0; i<cols; i++){
        float sum = 0;
        for (int j=0; j<num_threads; j++){
            sum += temp[j][i];
        }
        C[i] = sum;
    }
    for(int i = 0; i < num_threads; ++i)
        delete temp[i];
    delete[] temp;
}


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

    ifstream cin(input_file);

    cin >> rows >> cols >> nonzeros;

    row_indexes = new int[nonzeros];
    col_indexes = new int[nonzeros];
    values = new float[nonzeros];

    int rid, cid;
    double val;
    int i=0;
    while(cin >> rid >> cid >> val) {
        row_indexes[i]=cid;
        col_indexes[i]=rid;
        values[i]=val;
        i++;
    }
    cout<< "Graph data loaded in "<<(double)(clock() - tStart)/CLOCKS_PER_SEC<<"s"<<endl;
    cout<<"Loaded "<<cols<<" pages"<<endl;

    float *C0 = new float[cols];
    float *C1 = new float[cols];

    float *t_swap;

    //initialize pagerank
    float equal_prob = 1.0 / cols;

    //equation stuff
    float alpha_prob = equal_prob*alpha;

    float rand_walk = (1.0-alpha)/(1.0*cols);

    //cout << alpha_prob << endl;

    for(i=0; i<cols;i++) {
        C0[i]=equal_prob;
    }
    omp_set_num_threads(num_threads);

    //PHASE 2 : Page ID Distribution
    //TODO: ID distribution logic for different threads comes here.
    //PHASE 3 : iteratively update ranks
    //Once page distribution is done, we can spawn the threads
    //The code below simply distributes the pages across threads based on page id, we need to change it
    tStart = clock();
    for(int j=0;j<num_iters;j++){
        multiply(C0,C1);
        #pragma omp parallel for
        for(int i=0; i<cols; i++) {
            C1[i]=alpha*C1[i] + rand_walk;
        }

        t_swap = C0;
        C0=C1;
        C1=t_swap;

    }
    cout<<"Algorithm terminated in "<<(double)(clock() - tStart)/CLOCKS_PER_SEC<<"s"<<endl;

    //PHASE 4: Write the result
    cout<<"Writing result..."<<endl;
    string result_path = input_file + string("_parallel_ranks") ;
    FILE *fptr = fopen(result_path.c_str(), "w");
    for(int i=0; i<cols; i++){
        fprintf(fptr, "%d %.10f\n", i, C0[i]);
    }
    fclose(fptr);
    return 0;
}