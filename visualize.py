import numpy as np
import matplotlib.pyplot as plt
import matplotlib

INF = 9999999


class Point():

    def __init__(self, x,y):
        self.x = x
        self.y = y



class Road():

    def __init__(self, b,e):
        self.b = b
        self.e = e

class Line():

    def __init__(self, xpl,ypl):
        self.xpl=xpl
        self.ypl=ypl

def calc_cross_point(pointA, pointB, pointC, pointD):
    cross_points = Point(0,0)
    F_point=Point(INF,INF)
    bunbo = (pointB.x - pointA.x) * (pointD.y - pointC.y) - (pointB.y - pointA.y) * (pointD.x - pointC.x)
    # 直線が平行な場合
    if (bunbo == 0):
        return F_point

    vectorAC = Point((pointC.x - pointA.x), (pointC.y - pointA.y))
    r = ((pointD.y - pointC.y) * vectorAC.x - (pointD.x - pointC.x) * vectorAC.y) / bunbo
    #s = ((pointB[1] - pointA[1]) * vectorAC[0] - (pointB[0] - pointA[0]) * vectorAC[1]) / bunbo

    # 線分AB、線分AC上に存在しない場合
    if (r <= 0) or (1 <= r):
        return F_point

    # rを使った計算の場合
    distance = Point((pointB.x - pointA.x) * r, (pointB.y - pointA.y) * r)
    cross_points = Point(pointA.x + distance.x, pointA.y + distance.y)
    # sを使った計算の場合
    # distance = ((pointD[0] - pointC[0]) * s, (pointD[1] - pointC[1]) * s)
    # cross_points = (int(pointC[0] + distance[0]), int(pointC[1] + distance[1]))
    return cross_points




xl=[]
yl=[]

line=[]

xp=[]
yp=[]
tmpxp=[]
tmpyp=[]

crossx=[]
crossy=[]

#ans[]
points=[]
roads=[]

n,m,p,q=map(int,input().split())

for i in range(n):
    s,t=map(int,input().split())
    xl.append(s)
    yl.append(t)
    p=Point(s,t)
    points.append(p)

for i in range(m):
    u,v=map(int,input().split())
    u=u-1
    v=v-1
    del tmpxp[0:2]
    del tmpyp[0:2]

    xp.append(xl[u])
    yp.append(yl[u])
    xp.append(xl[v])
    yp.append(yl[v])

    tmpxp.append(xl[u])
    tmpyp.append(yl[u])
    tmpxp.append(xl[v])
    tmpyp.append(yl[v])
    #print(tmpxp)

    r=Road(u,v)
    roads.append(r)
    #plt.plot(tmpxp,tmpyp)
    l=Line(tmpxp,tmpyp)
    line.append(l)
    plt.plot(line[0].xpl,line[0].ypl,c='green')


for i in range(m):
    j=i+1
    for j in range(m):
        a=roads[i].b
        b=roads[i].e
        c=roads[j].b
        d=roads[j].e
        tmp=calc_cross_point(points[a],points[b],points[c],points[d])
        if tmp.x==INF: continue
        crossx.append(tmp.x)
        crossy.append(tmp.y)

#print(xl)
plt.scatter(xl,yl)
plt.scatter(crossx,crossy,c='red')
plt.show()
