#!/usr/bin/env python3

import matplotlib
matplotlib.use('Agg')   # Backend per PNG 
import matplotlib.pyplot as plt
import pandas as pd

df1n = pd.read_csv("mpi_cpi-1n.csv", names=["par","n","e2","t","nt","host"])
df2n = pd.read_csv("mpi_cpi-2n.csv", names=["par","n","e2","t","nt","host"])

df1n1g=df1n[df1n["n"]==1000000000]
df1n2g=df1n[df1n["n"]==2000000000]
df1n4g=df1n[df1n["n"]==4000000000]

df2n1g=df2n[df2n["n"]==1000000000]
df2n2g=df2n[df2n["n"]==2000000000]
df2n4g=df2n[df2n["n"]==4000000000]


print (df2n4g)

plt.subplot(2,1,1)

plt.title('MPI "CPI scaling  1vs2 nodes" - Roberto Alfieri 22/04/26')

plt.title('CPI MPI scaling ')
plt.grid()
plt.xlabel('tasks')
plt.ylabel('time')
plt.yscale('log')
plt.plot(df1n4g.nt,  df1n4g.t, '-o', label="4Ginterv-1node")
plt.plot(df2n4g.nt,  df2n4g.t, '-o', label="4Ginterv-2nodes")
plt.legend(shadow=True,loc="best")

plt.subplot(2,1,2)

plt.grid()
plt.xlabel('tasks')
plt.ylabel('speedup')
plt.plot(df1n4g.nt,  df1n4g.t.iloc[0]/df1n4g.t,  '-o', label="4Ginterv-1node")
plt.plot(df2n4g.nt,  df1n4g.t.iloc[0]/df2n4g.t,  '-o', label="4Ginterv-2nodes")
#plt.plot(range(1,32),range(1,32),'-r', label='Ideal')
plt.plot(range(1,4),range(1,4),'-r', label='Ideal')
plt.legend(shadow=True,loc="best")

plt.savefig('mpi_cpi.png')
