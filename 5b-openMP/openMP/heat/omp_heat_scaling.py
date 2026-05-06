import matplotlib
matplotlib.use('Agg') # backend per png
import matplotlib.pyplot as plt
import pandas as pd

df = pd.read_csv("omp_heat_scaling.csv" ,comment="#",  names=["nt", "r", "c", "iter", "t", "tj", "hostname" ])

#print (df)

df1 = df[df["r"] == 1024]
df2 = df[df["r"] == 2048]
df4 = df[df["r"] == 4096]

print (df2)

plt.subplot(2,1,1)

plt.title('openMP "HEAT scaling" - Roberto Alfieri 28/03/25')

plt.grid()
plt.xlabel('threads')
plt.ylabel('time')
plt.yscale('log')
plt.plot(df1.nt,df1.t,'-o', label='1024')
plt.plot(df2.nt,df2.t,'-o', label='2048')
plt.plot(df4.nt,df4.t,'-o', label='4096')
plt.legend(shadow=True,loc="best")


plt.subplot(2,1,2)

plt.grid()
plt.xlabel('threads')
plt.ylabel('Speedup')
plt.plot(df1.nt,df1.t.iloc[0]/df1.t,'-o', label='1024')
plt.plot(df2.nt,df2.t.iloc[0]/df2.t,'-o', label='2048')
plt.plot(df4.nt,df4.t.iloc[0]/df4.t,'-o', label='4096')
plt.plot(range(1,10),range(1,10),'-r', label='Ideal')
plt.legend(shadow=True,loc="best")

plt.savefig('omp_heat_scaling.png')
