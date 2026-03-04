import matplotlib.pyplot as plt 
import pandas as pd 
from glob import glob

filenames = glob("*mm*.csv")

dfs = []
for filename in filenames:
    dfs.append(pd.read_csv(filename, comment="#", names=["N", "gflop", "tempo" ,"GFLOPs", "Hostname", "opt", "nt"]) )

# Concatenate all data into one DataFrame
df = pd.concat(dfs, ignore_index=True)

print (df)

df.plot(kind='bar',  x='opt', y='GFLOPs', legend='true');
# https://pandas.pydata.org/pandas-docs/version/0.23/generated/pandas.DataFrame.plot.html

plt.title('Performance CPU "matrixMul" Kenneth James Nna Minkousse 02/03/2026')
plt.legend()
plt.yscale('log')
plt.ylabel('GFLOP/s')
plt.tight_layout()
#plt.xticks(rotation=30)
plt.savefig('matrixMul.png')


