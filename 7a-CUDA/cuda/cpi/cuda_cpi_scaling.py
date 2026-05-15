import matplotlib
matplotlib.use('Agg')   # Backend per PNG
import matplotlib.pyplot as plt
import pandas as pd


dfG = pd.read_csv("cuda_cpi_global.csv",comment="#", names=["par", "N", "gridDim",  "blockDim", "pi" ,"err", "wtime", "mtime",  "ktime"] )
dfS = pd.read_csv("cuda_cpi_shared.csv",comment="#", names=["par", "N", "gridDim",  "blockDim", "pi" ,"err", "wtime", "mtime", "ktime"] )

print (dfG)

plt.title('CUDA "CPI scaling" - Roberto Alfieri 24/04/26')
plt.grid()
plt.xlabel('blockDim')
plt.ylabel('time')
plt.yscale('log')
plt.plot(dfG.blockDim,dfG.ktime,'-o', label='Glob-kern')
plt.plot(dfG.blockDim,dfG.mtime,'-o', label='Glob-memc')
plt.plot(dfG.blockDim,dfG.wtime,'-o', label='Glob-wall')
plt.plot(dfS.blockDim,dfS.ktime,'-o', label='Shar-kern')
plt.plot(dfS.blockDim,dfS.mtime,'-o', label='Shar-memc')
plt.plot(dfS.blockDim,dfS.wtime,'-o', label='Shar-wall')
plt.legend(shadow=True,loc="best")
plt.savefig('cuda_cpi_scaling.png')
