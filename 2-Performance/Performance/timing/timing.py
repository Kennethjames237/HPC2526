import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt 
import pandas as pd


df = pd.read_csv("timing.csv",comment="#", sep="\\s+" )

#print (df.timer)

# sistemare nome e data
plt.title('Performance - "Timing" - Nna Minkousse Kenneth James 02/03/2026')
plt.ylabel('seconds')
plt.bar(df.timer,  df.time)
#plt.xticks(rotation=45)
plt.savefig('timing.png')
