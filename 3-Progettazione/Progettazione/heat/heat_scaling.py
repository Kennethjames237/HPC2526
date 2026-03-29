import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt 
import pandas as pd 

df = pd.read_csv("heat.csv",comment="#",skipinitialspace=True) 
print (df) 

plt.title('design "HEAT Scaling" Nna Minkoisse Kenneth James 12/03/26')
plt.grid()
plt.xlabel('N')
#plt.yscale('log')
plt.ylabel('time')
plt.plot(df.nx,df.time,'r-o',label='Heat scaling')
plt.legend(shadow=True)
plt.savefig('heat_scaling.png')

