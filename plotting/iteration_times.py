import matplotlib.pyplot as plt
import sys
import math


if sys.argv[1] not in ["1","2", "3"]:
    print("Please enter any integer value from 1 to 3")
    exit(0)

#NO OF NODES
X1 = [3774767.0,4682830.0,5363201.0,8388607.0,8388608.0,10356390.0,39454746.0,41652230.0,61321359.0]
X1 = [math.log(x) for x in X1]

#serial
Y11 = [918.654,339.177, 3142.79,3330.76,1042.89,1193.62,31948.6,74182,83484.7]
Y11 = [math.log(x) for x in Y11]


#parallel
Y12 = [131.222,64.0472, 421.94,396.917,206.507,860.563,3471.66,23010.1,22141.9]
Y12 = [math.log(x) for x in Y12]
#csr
Y13 = [26.9003,7.51292, 103.926,31.1403,18.9515,42.6769,499.802,2104.45,2386.86]
Y13 = [math.log(x) for x in Y13]

#coo
Y14 = [53.3475,30.0633, 164.073,114.906,72.5826,95.0792,1275.34,4194.98,5636.16]
Y14 = [math.log(x) for x in Y14]

if sys.argv[1]=="1":
    plt.plot(X1,Y11, label='Serial')
    plt.plot(X1,Y12, label='Parallel')
    plt.plot(X1,Y13, label='SPMV CSR')
    plt.plot(X1,Y14, label='SPMV COO')

    current_values = plt.gca().get_xticks()
    # using format string '{:.0f}' here but you can choose others
    plt.gca().set_xticklabels(['{:.3f}'.format((x)) for x in current_values])

    plt.legend()
    plt.xlabel('log(no. of nodes)')
    plt.ylabel('log(time (s))')
    plt.show()


#NO OF EDGES
X2 = [5138066.0,16518948.0,22507155.0,25165784.0,63501393.0,79023142.0,936364282.0,1403019900.0,1468365182.0]
X2 = [math.log(x) for x in X2]

#serial
Y21 = [339.177, 918.654,1193.62,1042.89,1042.89,3142.79,31948.6,83484.7,74182]
Y21 = [math.log(x) for x in Y21]
#parallel
Y22 = [64.0472, 131.222,860.563,206.507,206.507,421.94,3471.66,22141.9,23010.1]
Y22 = [math.log(x) for x in Y22]
#csr
Y23 = [7.51292, 26.9003,42.6769,18.9515,18.9515,103.926,499.802,2386.86,2104.45]
Y23 = [math.log(x) for x in Y23]
#coo
Y24 = [30.0633, 53.3475,95.0792,72.5826,72.5826,164.073,1275.34,5636.16,4194.98]
Y24 = [math.log(x) for x in Y24]

if sys.argv[1]=="2":
    plt.plot(X2,Y21, label='Serial')
    plt.plot(X2,Y22, label='Parallel')
    plt.plot(X2,Y23, label='SPMV CSR')
    plt.plot(X2,Y24, label='SPMV COO')

    current_values = plt.gca().get_xticks()
    # using format string '{:.0f}' here but you can choose others
    plt.gca().set_xticklabels(['{:.2f}'.format((x/(1))) for x in current_values])

    plt.legend()
    plt.xlabel('log(no. of edges)')
    plt.ylabel('log(time (s))')
    plt.show()

#AVG DEGRREE
X3 = [1.097213864,2.173262594, 2.999995232, 4.376150369, 7.569956847, 14.73432415, 22.87979136, 23.73261463, 35.25297882]
X3_nodes = [4682830,10356390,8388608,3774767,8388607,5363201,61321359,39454746,41652230]
X3_edges = [5138066,22507155,25165784,16518948,63501393,79023142,1403019900,936364282,1468365182]
#serial
Y31 = [339.177, 1193.62, 1042.89, 918.654, 3330.76, 3142.79, 83484.7, 31948.6, 74182]
for i in range(len(X3)):
    Y31[i]=Y31[i]/X3_edges[i]
    Y31[i]*=1e6

Y31 = [math.log(x) for x in Y31]
#parallel
Y32 = [64.0472, 860.563, 206.507, 131.222, 64.0472, 421.94, 22141.9, 3471.66, 23010]
for i in range(len(X3)):
    Y32[i]=Y32[i]/X3_edges[i]
    Y32[i]*=1e6

Y32 = [math.log(x) for x in Y32]
#csr
Y33 = [7.51292, 42.6769, 18.9515, 26.9003, 7.51292, 103.926, 2386.86, 499.80, 2104.45]
for i in range(len(X3)):
    Y33[i]=Y33[i]/X3_edges[i]
    Y33[i]*=1e6

Y33 = [math.log(x) for x in Y33]
#coo
Y34 = [30.0633, 95.0792, 72.5826, 53.3475, 30.0633, 164.073, 5636.16, 1275.34, 4194.98]
for i in range(len(X3)):
    Y34[i]=Y34[i]/X3_edges[i]
    Y34[i]*=1e6
Y34 = [math.log(x) for x in Y34]
if sys.argv[1]=="3":
    plt.plot(X3,Y31, label='Serial')
    plt.plot(X3,Y32, label='Parallel')
    plt.plot(X3,Y33, label='SPMV CSR')
    plt.plot(X3,Y34, label='SPMV COO')

    current_values = plt.gca().get_yticks()
    # using format string '{:.0f}' here but you can choose others
    plt.gca().set_yticklabels(['{:.2f}'.format((x)) for x in current_values])

    plt.legend()
    plt.xlabel('avg. degree per node')
    plt.ylabel('log(time/edge (microseconds))')
    plt.show()
