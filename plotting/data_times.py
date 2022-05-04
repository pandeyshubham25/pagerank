import matplotlib.pyplot as plt

#NO OF EDGES
X2 = [5138066,16518948,22507155,25165784,63501393,79023142,936364282,1403019900,1468365182]

#normal
Y21 = [5.34433, 17.279, 20.9601, 21.2118,48,     58,    583.643,7109.85,3366.95]

#csr
Y22 = [28.7373, 70.7193, 104.43, 113.547,258.73,    309,    3595.64,5477.05,5696.82]

#coo
Y23 = [2.20386, 7.88892, 10.4723, 11.0423,28.1323,   37,    409.195,695.81,733.099]


plt.plot(X2,Y21, label='Base')
plt.plot(X2,Y22, label='CSR')
plt.plot(X2,Y23, label='COO')
current_values = plt.gca().get_xticks()
# using format string '{:.0f}' here but you can choose others
plt.gca().set_xticklabels(['{:.0f}M'.format((x/(1000000))) for x in current_values])

plt.legend()
plt.xlabel('no. of edges')
plt.ylabel('time to load(s)')
plt.show()

exit(0)


