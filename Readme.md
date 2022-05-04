Parallel Page Rank
___
**Directories and files descriptions**

_src_ - contains core logic 
1) prank_serial.cpp - serial implementation
2) prank_parallel.cpp - parallel implementation using mpi
3) prank_parallel_spmv.cpp - sparse matrix vector multiplication using coordinate
4) prank_parallel_csr.cpp - sparse matrix vector multiplication using compressed sparse row format
5) adj_to_csr.cpp - utility code to convert graph data to CSR (Compressed Sparse Row) format
6) adj_to_coo.cpp - utility code to convert graph data to COO (Coordinate) format
7) verify.cpp - utility code to evaluate correctness of computed page ranks against the baseline values

_data_ - contains sample files to run Page Rank on \
NOTE: We have only provided 1 dataset here. Any other data source can be put in this directory for evaluation.
To make sure that the processing is consistent, all the files in this directory contain only two space separated integers in every line indicating an edge between the first integer and the second.

_test_ - contains test data
This directory has custom generated data for testing purposes. The data format is same as that in _data_ directory.

_data_formatter.py_
It can be run on the datasets downloadable from the links provided for the dataset in the ppt. This converts the files to the standard format we are using in _data_ directory.
Once downloaded, the file script can be run as - \
@Nathan to fill in here -

___

**Steps to execute -**
1) Make sure you have the dataset in desired format inside _data_ directory (we have provided one for testing). If you want to test on some other data, you can use _data_formatter.py_ to format it and place it in _data_ directory (instructions for this given in the previous section)
2) Make sure that you are inside _src_ directory before executing any of the commands below
3) Run prank_serial.cpp
   1) g++ -fopenmp prank_serial.cpp -o prank_serial
   2) ./prank_serial 50 0.85 ../data/cit-Patents
   3) arguments - (number of iterations, alpha value for page rank algorithm, input file name)
   4) This creates a file named _cit-Patents_serial_ranks_ in the _data_ directory with node id and their corresponding weight (in range 0 to 1) in each line.
4) Run prank_parallel.cpp
   1) g++ -fopenmp prank_parallel.cpp -o prank_parallel
   2) ./prank_parallel 7 50 0.85 ../data/cit-Patents
   3) arguments - (number of threads, number of iterations, alpha value for page rank algorithm, input file name)
   4) This creates a file named _cit-Patents_parallel_ranks_ in the _data_ directory with node id and their corresponding weight (in range 0 to 1) in each line.
5) Run prank_parallel_spmv.cpp
   1) Make sure that you have the file in coo format inside _data_ directory. For this, execute adj_to_coo.cpp as instructued below
      1) g++ adj_to_coo.cpp -o adjcoo
      2) ./adjcoo ../data/cit-Patents
   2) This creates a file named _cit-Patents_coo_ in _data_ directory
   3) g++ -fopenmp prank_parallel_spmv.cpp -o prank_parallel_spmv
   4) ./prank_parallel_spmv 7 50 0.85 ../data/cit-Patents_coo
   5) arguments - (number of threads, number of iterations, alpha value for page rank algorithm, input file name)
   6) This creates a file named _cit-Patents_coo_parallel_ranks_ in the _data_ directory with node id and their corresponding weight (in range 0 to 1) in each line.
6) Run prank_parallel_csr.cpp
   1) Make sure that you have the file in csr format inside _data_ directory. For this, execute adj_to_coo.cpp as instructued below
       1) g++ adj_to_csr.cpp -o adjcsr
       2) ./adjcsr ../data/cit-Patents
   2) This creates a file named _cit-Patents_csr_ in _data_ directory
   3) g++ -fopenmp prank_parallel_csr.cpp -o prank_parallel_csr
   4) ./prank_parallel_csr 7 50 0.85 ../data/cit-Patents_csr
   5) arguments - (number of threads, number of iterations, alpha value for page rank algorithm, input file name)
   6) This creates a file named _cit-Patents_csr_parallel_ranks_ in the _data_ directory with node id and their corresponding weight (in range 0 to 1) in each line.

We have also created a bash script that sequentially executes all of the above commands.
You can execute it as -
@Nathan to add description here