#include <iostream>
#include <fstream>
#include <list>
#include <vector>
#include <unordered_map>
#include <omp.h>
#include <time.h>

using namespace std;


//Global Variables
int num_threads; //number of threads - 1st arg in main()
int num_iters; //number of iterations - 2nd argu in main()
float alpha; //alpha factor - 3rd arg in main()
char *input_file; //input graph file name - 4th arg in main()


void apply_iteration(long *mr, long *mc, float *weights, float **vp, long n, float alpha) {

    float *vec = *vp;
    float *nv = new float[n]{};
    float rand_start = (1.0-alpha)/((float) n);
    //for (int i =0; i < n; i++) {
        //cout << vec[i] << ", ";
    //}
    //cout << "\n";
    
    #pragma omp parallel for
    for (int r = 0; r < n; r++) {
        long rs = mr[r];
        long re = mr[r+1];
        long outdegree = re - rs;
        //cout << rs << "_" << re << ":";
        while (rs < re) {
            long mcol = mc[rs];
            //cout <<  nv[r] << "+" << vec[mcol] * weights[rs];
            nv[r] += vec[mcol] * weights[rs++];
            //cout << "=" << nv[r] << ", ";
        }
        nv[r] *= alpha;
        nv[r] += rand_start;
        //cout << "\n";
    }

    //for (int i =0; i < n; i++) {
        //cout << nv[i] << ", ";
    //}
    //cout << "\n";


    delete *vp;
    *vp = nv;

}


int main(int argc, char** argv){


    //PHASE 0 : Read commandline args and initialize global params
    //TODO: Add exception handling for parsing cline args
    num_iters = atoi(argv[1]);
    alpha = atof(argv[2]);
    input_file = argv[3];
    num_threads = atoi(argv[4]);



    //PHASE 1 : Read graph from input file
    cout<<"Loading graph data..."<<endl;

    clock_t tStart = clock();

    ifstream cin(input_file);


    long n;
    long nz;
    cin >> n >> nz;
    cin.seekg(1, ios_base::cur);

    long *mr = new long[n + 1];
    long *mc = new long[nz];
    float *vals = new float[nz];

    long cid;
    int i=0;
    while(cin.peek() != '\n') {
        cin >> cid;
        mc[i]=cid;
        i++;
        cin.seekg(1, ios_base::cur);
    }
    cin.seekg(1, ios_base::cur);
    i = 0;
    float val;
    while(cin.peek() != '\n') {
        cin >> val;
        vals[i]=val;
        i++;
        cin.seekg(1, ios_base::cur);
    }
    cin.seekg(1, ios_base::cur);
    i = 0;
    long row_break;
    while(cin.peek() != '\n') {
        cin >> row_break;
        mr[i] = row_break;
        i++;
        cin.seekg(1, ios_base::cur);
    }
    cout<< "Graph data loaded in "<<(double)(clock() - tStart)/CLOCKS_PER_SEC<<"s"<<endl;
    cout<<"Loaded "<<n<<" pages"<<endl;


    //initialize pagerank
    float equal_prob = 1.0 / n;


    float rand_walk = (1.0-alpha) * equal_prob;

    float *vec = new float[n];
    omp_set_num_threads(num_threads);
    
    #pragma omp parallel for
    for(i=0; i<n; i++) {
        vec[i] = equal_prob;
    }
    

    //PHASE 2 : Page ID Distribution
    //TODO: ID distribution logic for different threads comes here.
    //PHASE 3 : iteratively update ranks
    //Once page distribution is done, we can spawn the threads
    //The code below simply distributes the pages across threads based on page id, we need to change it
    tStart = clock();
    //for (int i =0; i < n; i++) {
        //cout << vec[i] << ", ";
    //}
    //cout << "\n";
    for(int j=0;j<num_iters;j++){
        apply_iteration(mr, mc, vals, &vec, n, alpha);
        //for (int i =0; i < n; i++) {
            //cout << vec[i] << ", ";
        //}
        //cout << "\n";
    }
    cout<<"Algorithm terminated in "<<(double)(clock() - tStart)/CLOCKS_PER_SEC<<"s"<<endl;

    //PHASE 4: Write the result
    cout<<"Writing result..."<<endl;
    string result_path = input_file + string("_parallel_ranks") ;
    FILE *fptr = fopen(result_path.c_str(), "w");
    for(int i=0; i<n; i++){
        fprintf(fptr, "%d %.10f\n", i, vec[i]);
    }
    fclose(fptr);
    return 0;
}
