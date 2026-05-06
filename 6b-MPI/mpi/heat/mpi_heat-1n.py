import matplotlib
matplotlib.use('Agg') # backend per png
import matplotlib.pyplot as plt
import pandas as pd

df = pd.read_csv("mpi_heat_original-1n.csv" , comment="#", names=["par", "nt", "r", "c", "iter", "t"])

#print (df)

df1 = df[df["r"] == 1024]
df2 = df[df["r"] == 2048]

print (df2)

plt.subplot(2,1,1)

plt.title('MPI "HEAT scaling -> 1 Node (Overlap comunicazione)" - Nna Minkousse Kenneth James 29/04/26')

plt.grid()
plt.xlabel('tasks')
plt.ylabel('time')
plt.plot(df1.nt,df1.t,'-o', label='1024')
plt.plot(df2.nt,df2.t,'-o', label='2048')
plt.legend(shadow=True,loc="best")

plt.subplot(2,1,2)

plt.grid()
plt.xlabel('tasks')
plt.ylabel('Speedup')
plt.plot(df1.nt,df1.t.iloc[0]/df1.t,'-o', label='1024')
plt.plot(df2.nt,df2.t.iloc[0]/df2.t,'-o', label='2048')
plt.plot(range(1,5),range(1,5),'-r', label='Ideal')
plt.legend(shadow=True,loc="best")

plt.savefig('mpi_heat_original-1n.png')
