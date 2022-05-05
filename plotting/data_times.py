import matplotlib.pyplot as plt
import math

#NO OF EDGES
X2 = [5138066.0,16518948.0,22507155.0,25165784.0,63501393.0,79023142.0,936364282.0,1403019900.0,1468365182.0]
X2 = [math.log(x) for x in X2]
#normal
Y21 = [5.34433, 17.279, 20.9601, 21.2118,48,     58,    583.643,7109.85,3366.95]
Y21 = [math.log(x) for x in Y21]
#csr
Y22 = [28.7373, 70.7193, 104.43, 113.547,258.73,    309,    3595.64,5477.05,5696.82]
Y22 = [math.log(x) for x in Y22]
#coo
Y23 = [2.20386, 7.88892, 10.4723, 11.0423,28.1323,   37,    409.195,695.81,733.099]
Y23 = [math.log(x) for x in Y23]

plt.plot(X2,Y21, label='Base')
plt.plot(X2,Y22, label='CSR')
plt.plot(X2,Y23, label='COO')
current_values = plt.gca().get_xticks()
# using format string '{:.0f}' here but you can choose others
plt.gca().set_xticklabels(['{:.2f}'.format((x/(1))) for x in current_values])

plt.legend()
plt.xlabel('log(no. of edges)')
plt.ylabel('log(time to load(s))')
plt.show()



