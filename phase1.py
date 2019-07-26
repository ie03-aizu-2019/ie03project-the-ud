import numpy as np
import matplotlib.pyplot as plt
import matplotlib


xl=[]
yl=[]

line=[]

xp=[]
yp=[]

crossX=[]
crossy=[]

n,m,p,q=map(int,input().split())

for i in range(n):
    s,t=map(int,input().split())
    xl.append(s)
    yl.append(t)

for i in range(m):
    u,v=map(int,input().split())
    xp.append(xl[u-1])
    yp.append(yl[u-1])
    xp.append(xl[v-1])
    yp.append(yl[v-1])


plt.scatter(xl,yl)
plt.plot(xp,yp)

plt.show()
