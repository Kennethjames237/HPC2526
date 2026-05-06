#!/usr/bin/env python3

import matplotlib
matplotlib.use('Agg')   # Backend per PNG 
import matplotlib.pyplot as plt
import pandas as pd

df1n = pd.read_csv("mpi_cpi-1n.csv", names=["par","n","e2","t","nt","host"])

#print(df)

df1n1g=df1n[df1n["n"]==1000000000]  # 1 nodo - 1G intervalli
df1n2g=df1n[df1n["n"]==2000000000]  # 1 nodo - 2G intervalli
df1n4g=df1n[df1n["n"]==4000000000]  # 1 nodo - 4G intervalli

print (df1n1g)

plt.subplot(2,1,1)

plt.title('MPI "CPI scaling 1 node" - Roberto Alfieri 21/04/26')

plt.grid()
plt.xlabel('tasks')
plt.ylabel('time')
plt.yscale('log')
plt.plot(df1n1g.nt,  df1n1g.t, '-o', label="1Ginterv-1node")
plt.plot(df1n2g.nt,  df1n2g.t, '-o', label="2Ginterv-1node")
plt.plot(df1n4g.nt,  df1n4g.t, '-o', label="4Ginterv-1node")
plt.legend(shadow=True,loc="best")

plt.subplot(2,1,2)

plt.grid()
plt.xlabel('tasks')
plt.ylabel('speedup')
plt.plot(df1n1g.nt,  df1n1g.t.iloc[0]/df1n1g.t,  '-o', label="1Ginterv-1node")
plt.plot(df1n2g.nt,  df1n2g.t.iloc[0]/df1n2g.t,  '-o', label="2Ginterv-1node")
plt.plot(df1n4g.nt,  df1n4g.t.iloc[0]/df1n4g.t,  '-o', label="4Ginterv-1node")
#plt.plot(range(1,32),range(1,32),'-r', label='Ideal')
plt.plot(range(1,4),range(1,4),'-r', label='Ideal')
plt.legend(shadow=True,loc="best")

plt.savefig('mpi_cpi-1n.png')
