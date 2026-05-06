
import matplotlib
matplotlib.use('Agg') # backend per png
import matplotlib.pyplot as plt
import pandas as pd

df_ser     = pd.read_csv("heat.csv", comment="#",  names=["r","c","iter", "time", "jtime"])
df_omp     = pd.read_csv("omp_heat_scaling.csv" ,comment="#",   names=["PAR", "thr", "r", "c","iter", "time", "jtime", "node"])
df_mpi     = pd.read_csv("mpi_heat_scaling.csv" ,comment="#",  names=["PAR","task","r", "c","iter", "time"])
df_hyb     = pd.read_csv("mpi+omp_heat_scaling.csv" ,comment="#",  names=["PAR","task","thr","r", "c","iter", "time"])

print (df_ser)
print (df_omp)
print (df_mpi)
print (df_hyb)

plt.subplot(211)
plt.title('Heat  scaling - 8192x8192,  1000 iter ')
plt.grid()
plt.ylabel('time (s)')
plt.plot(df_omp.thr,df_omp.time,'-o', label='openMP')
plt.plot(df_mpi.task,df_mpi.time,'-o', label='MPI')
plt.plot((df_hyb.task*df_hyb.thr),df_hyb.time,'-o', label='MPI+openMP')
plt.plot(1,df_ser.time,'-o', label='serial')
plt.legend(shadow=True,loc="best")


plt.subplot(212)
plt.grid()
plt.xlabel('tasks x threads')
plt.ylabel('Speedup')
plt.plot(df_omp.thr, df_ser.time[0]/df_omp.time,'-o', label='openMP')
plt.plot(df_mpi.task,df_ser.time[0]/df_mpi.time,'-o', label='MPI')
plt.plot(df_hyb.task*df_hyb.thr,df_ser.time[0]/df_hyb.time,'-o', label='MPI+openMP')
plt.plot(range(1,10),range(1,10),'-r', label='Ideal')
plt.legend(shadow=True,loc="best")

plt.savefig('heat_scaling.png')
