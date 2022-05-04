Parallel Page Rank
___
**Directories and files descriptions**
___
_src_ - contains core logic 
1) prank_serial.cpp - serial implementation
2) prank_parallel.cpp - parallel implementation using mpi
3) prank_parallel_spmv.cpp - sparse matrix vector multiplication using coordinate
4) prank_parallel_csr.cpp - sparse matrix vector multiplication using compressed sparse row format
5) adj_to_csr.cpp - utility code to convert graph data to CSR (Compressed Sparse Row) format
6) adj_to_coo.cpp - utility code to convert graph data to COO (Coordinate) format
7) verify.cpp - utility code to evaluate correctness of computed page ranks against the baseline values
8) measure.cpp - utility code for lazy execution of all implementations

___
_data_ - contains sample files to run Page Rank on \
NOTE: We have only provided 2 datasets here. Any other data source can be put in this directory for evaluation.
To make sure that the processing is consistent, all the files in this directory contain only two space separated integers in every line indicating an edge between the first integer and the second.
___
_test_ - contains test data
This directory has custom generated data for testing purposes. The data format is same as that in _data_ directory.
___
_plotting_ - contains python scripts to generate plots and generated plots \
NOTE: Please make sure you are in _plotting_ directory before executing the below commands
1) iteration_times.py - to run this script, use the following format
   1) python iteration_times.py 1
   2) The commandline arg is integer value from 1 to 3 indicating which data to plot
   3) 1 is for plotting data based on number of nodes, 2 is for plotting data based on number of edges and 3 is for plotting data based on average degree of nodes
2) data_times.py - to run this script, use the following format
   1) python data_times.py
   2) It does not take any commandline argument, and simply plots the time take by different data formats to load based on the number of edges in the graph.
___
_data_formatter.py_
It can be run on the datasets downloadable from the links provided for the dataset in the ppt. This converts the files to the standard format we are using in _data_ directory.
To use:
1) Download data
2) Place individual files (for data only) into the `data_raw` directory
3) `python3 data_formatter.py`

___

**Steps to execute**
___
1) Make sure you have created a _build_ folder in the base directory (this is where some of the outputs get written).
2) Make sure you have the dataset in desired format inside _data_ directory (we have provided two for testing). If you want to test on some other data, you can use _data_formatter.py_ to format it and place it in _data_ directory (instructions for this given in the previous section)
3) Make sure that you are inside _src_ directory before executing any of the commands below
4) Run prank_serial.cpp
   1) clang++ -std=c++17 -fopenmp prank_serial.cpp -o prank_serial
   2) ./prank_serial 100 0.85 ../data/cit-Patents
   3) arguments - (number of iterations, alpha value for page rank algorithm, input file name)
   4) This creates a file named _cit-Patents_serial_ranks_ in the _build_ directory with node id and their corresponding weight (in range 0 to 1) in each line.
5) Run prank_parallel.cpp
   1) clang++ -std=c++17 -fopenmp prank_parallel.cpp -o prank_parallel
   2) ./prank_parallel 100 0.85 ../data/cit-Patents 9
   3) arguments - (number of iterations, alpha value for page rank algorithm, input file name, number of threads)
   4) This creates a file named _cit-Patents_parallel_ranks_ in the _build_ directory with node id and their corresponding weight (in range 0 to 1) in each line.
6) Run prank_parallel_spmv.cpp
   1) Make sure that you have the file in coo format inside _data_ directory. For this, execute adj_to_coo.cpp as instructued below
      1) clang++ -std=c++17 adj_to_coo.cpp -o adjcoo
      2) ./adjcoo ../data/cit-Patents
   2) This creates a file named _cit-Patents_coo_ in _data_ directory
   3) clang++ -std=c++17 -fopenmp prank_parallel_spmv.cpp -o prank_parallel_spmv
   4) ./prank_parallel_spmv 100 0.85 ../data/cit-Patents_coo 9
   5) arguments - (number of iterations, alpha value for page rank algorithm, input file name, number of threads)
   6) This creates a file named _cit-Patents_coo_parallel_ranks_ in the _data_ directory with node id and their corresponding weight (in range 0 to 1) in each line.
7) Run prank_parallel_csr.cpp
   1) Make sure that you have the file in csr format inside _data_ directory. For this, execute adj_to_coo.cpp as instructued below
       1) clang++ -std=c++17 adj_to_csr.cpp -o adjcsr
       2) ./adjcsr ../data/cit-Patents
   2) This creates a file named _cit-Patents_csr_ in _data_ directory
   3) clang++ -std=c++17 -fopenmp prank_parallel_csr.cpp -o prank_parallel_csr
   4) ./prank_parallel_csr 100 0.85 ../data/cit-Patents_csr 9
   5) arguments - (number of iterations, alpha value for page rank algorithm, input file name, number of threads)
   6) This creates a file named _cit-Patents_csr_parallel_ranks_ in the _data_ directory with node id and their corresponding weight (in range 0 to 1) in each line.

___

**Running all**
___

We have also created an executable that runs all methods.

Note: When calling the executable, ensure only the plain edge data is in the _data_ folder. Matrix conversion results will be left in the _build_ folder.

Note 2: If you wish to change any constants, you may do so in src/measure.cpp

To build: `make`

To execute all (after building): `./eval`
