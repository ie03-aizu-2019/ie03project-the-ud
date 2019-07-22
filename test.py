import numpy as np
import matplotlib.pyplot as plt
import matplotlib


xl=[]
yl=[]

for i in range(5):
    s,t=map(int,input().split())
    xl.append(s)
    yl.append(t)

plt.plot(xl,yl,"-o")

plt.show()
