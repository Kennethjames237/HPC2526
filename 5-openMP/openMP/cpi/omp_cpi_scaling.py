import matplotlib
matplotlib.use('Agg')   # Backend per PNG 
import matplotlib.pyplot as plt
import pandas as pd

df = pd.read_csv("omp_cpi_scaling.csv", names=["n","e","t","nt","host"])

print(df)

df1=df[df["n"]==1000000000]
df2=df[df["n"]==2000000000]
df4=df[df["n"]==4000000000]

plt.subplot(2,1,1)

plt.title('openMP "CPI scaling" - Roberto Alfieri 28/03/25')
plt.grid()
plt.xlabel('threads')
plt.ylabel('time')
plt.yscale('log')
plt.plot(df1.nt,  df1.t, '-o', label="CPI-1Giter")
plt.plot(df2.nt,  df2.t, '-o', label="CPI-2Giter")
plt.plot(df4.nt,  df4.t, '-o', label="CPI-4Giter")
plt.legend(shadow=True,loc="best")

plt.subplot(2,1,2)

plt.grid()
plt.xlabel('threads')
plt.ylabel('speedup')
plt.plot(df1.nt,  df1.t.iloc[0]/df1.t,  '-o', label="CPI-1Giter")
plt.plot(df2.nt,  df2.t.iloc[0]/df2.t,  '-o', label="CPI-2Giter")
plt.plot(df4.nt,  df4.t.iloc[0]/df4.t,  '-o', label="CPI-4Giter")
plt.plot(range(1,30),range(1,30),'-r', label='Ideal')
plt.legend(shadow=True,loc="best")

plt.savefig('omp_cpi_scaling.png')


