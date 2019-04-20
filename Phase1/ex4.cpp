#include<iostream>
#include<algorithm>
#include<vector>
#include<math.h>
#include<queue>
using namespace std;
#define INF 1e9
#define EPS 1e-8
#define N 30000

struct Point{
  double x,y;
  bool operator<(const Point& right)const{
    return x==right.x?y<right.y:x<right.x;
  }
  bool operator==(const Point& right)const{
    return x==right.x&&y==right.y;
  }
  double dist(Point p){
    return sqrt((x-p.x)*(x-p.x)+(y-p.y)*(y-p.y));
  }
};

struct Edge{
  int b,e;
};

Point findIntersection(Point p1,Point p2,Point p3,Point p4){
  double ta=(p3.x-p4.x)*(p1.y-p3.y)+(p3.y-p4.y)*(p3.x-p1.x);
  double tb=(p3.x-p4.x)*(p2.y-p3.y)+(p3.y-p4.y)*(p3.x-p2.x);
  double tc=(p1.x-p2.x)*(p3.y-p1.y)+(p1.y-p2.y)*(p1.x-p3.x);
  double td=(p1.x-p2.x)*(p4.y-p1.y)+(p1.y-p2.y)*(p1.x-p4.x);
  //交差判定 交差していなければINFを返す
  if(tc*td>=0||ta*tb>=0)return {INF,INF};

  //交差点のｘ座標が決まっている場合
  if(p1.x==p2.x){
    double x=p1.x;
    double y=x*(p3.y-p4.y)/(p3.x-p4.x)+p3.y-p3.x*(p3.y-p4.y)/(p3.x-p4.x);
    return {x,y};
  } 
  if(p3.x==p4.x){
    double x=p3.x;
    double y=x*(p1.y-p2.y)/(p1.x-p2.x)+p1.y-p1.x*(p1.y-p2.y)/(p1.x-p2.x);
    return {x,y};
  }

  //交差点のｘ座標が決まってない場合
  double ka=(p1.y-p2.y)/(p1.x-p2.x);
  double kb=p1.y-p1.x*(p1.y-p2.y)/(p1.x-p2.x);
  double kc=(p3.y-p4.y)/(p3.x-p4.x);
  double kd=p3.y-p3.x*(p3.y-p4.y)/(p3.x-p4.x);
  double x=(kb-kd)/(kc-ka);
  double y=x*(p1.y-p2.y)/(p1.x-p2.x)+p1.y-p1.x*(p1.y-p2.y)/(p1.x-p2.x);
  return {x,y};
}

typedef pair<double,int> Pdi;//(最短距離、ノード番号)
vector<double> dijkstra(int start,vector<vector<int> > graph,vector<Point> point){
  vector<double> dist(point.size(),INF);//dist[i]:=stratから頂点iに対する最短距離
  dist[start]=0;
  priority_queue<Pdi,vector<Pdi>,greater<Pdi> > que;
  que.push(Pdi(0,start));
  while(que.size()>0){
    Pdi tmp=que.top();
    que.pop();
    int v=tmp.second;
    if(dist[v]<tmp.first)continue;
    for(int i=0;i<graph[v].size();i++){
	 int to=graph[v][i];
	 if(dist[to]>dist[v]+point[v].dist(point[to])){
	   dist[to]=dist[v]+point[v].dist(point[to]);
	   que.push(Pdi(dist[to],to));
	 }
    }
  }
  return dist;
}

typedef pair<int,int> P;
typedef pair<Point,P> PP;//(頂点,(辺番号1,辺番号2))
vector<vector<int> > makeGraph(int n,int m,vector<Point> &point,vector<Edge> road,vector<PP> c){
  vector<vector<int> > graph(N);

  //交差点をx軸の負の方向から見ていき線分を分割する
  for(int i=0;i<c.size();i++){
    point.push_back(c[i].first);
    int edgeNum1=c[i].second.first;
    int edgeNum2=c[i].second.second;
    int np1=road[edgeNum1].b;
    int np2=road[edgeNum1].e;
    Point p1=point[np1];
    Point p2=point[np2];

    if(p1<p2){
	 //交差点の乗った線分に対し左(上)側の端点と交差点をグラフに追加
	 graph[np1].push_back(n+i);
	 graph[n+i].push_back(np1);
	 //左(上)側の頂点を交差点に更新
	 road[edgeNum1].b=n+i;	 
    }else{
	 graph[np2].push_back(n+i);
	 graph[n+i].push_back(np2);
	 road[edgeNum1].e=n+i;	 
    }

    np1=road[edgeNum2].b;
    np2=road[edgeNum2].e;
    p1=point[np1];
    p2=point[np2];

    if(p1<p2){
	 graph[np1].push_back(n+i);
	 graph[n+i].push_back(np1);
	 road[edgeNum2].b=n+i;	 
    }else{
	 graph[np2].push_back(n+i);
	 graph[n+i].push_back(np2);
	 road[edgeNum2].e=n+i;	 
    }   
  }

  for(int i=0;i<m;i++){
    graph[road[i].b].push_back(road[i].e);
    graph[road[i].e].push_back(road[i].b);
  }
  return graph;
}

//交差点を列挙してx、y順にソートする
vector<PP> makeAddPoint(int m,vector<Edge> road,vector<Point> point){
  vector<PP> addPoint;
  for(int i=0;i<m;i++){
    for(int j=i+1;j<m;j++){
	 int a=road[i].b;
	 int b=road[i].e;
	 int c=road[j].b;
	 int d=road[j].e;
	 Point tmp=findIntersection(point[a],point[b],point[c],point[d]);
	 if(tmp.x==INF)continue;
	 addPoint.push_back(PP(tmp,P(i,j)));
    }
  }
  sort(addPoint.begin(),addPoint.end());
  return addPoint;
}

//交差点はci=n+i-1として扱う
int stringToVertex(string s,int n){
  int res=0;
  if(s[0]=='C')res+=n;
  for(int i=0;i<s.size();i++){
    if(s[s.size()-1-i]=='C')continue;
    res+=(s[s.size()-1-i]-'0')*pow(10,i);
  }
  return res-1;
}

//distから経路を復元
vector<int> restore(int s,int t,vector<vector<int> > graph,vector<Point> point,vector<double> dist){
  vector<int> route;
  while(s!=t){
    route.push_back(t);
    for(int i=0;i<graph[t].size();i++){
	 int to=graph[t][i];
	 if(dist[to]-(dist[t]-point[t].dist(point[to]))<EPS){
	   t=to;
	   break;
	 }
    }
  }
  route.push_back(s);
  reverse(route.begin(),route.end());
  return route;
}


int main(){
  int n,m,p,q;
  cin>>n>>m>>p>>q;
  vector<Point> point(n);
  vector<Edge> road(m);
  for(int i=0;i<n;i++){
    double x,y;
    cin>>x>>y;
    point[i]={x,y};
  }
  for(int i=0;i<m;i++){
    int b,e;
    cin>>b>>e;
    b--,e--;//頂点は0-indexedで管理
    road[i]={b,e};
  }

  vector<PP> addPoint=makeAddPoint(m,road,point); 
  vector<vector<int> > graph=makeGraph(n,m,point,road,addPoint);
  
  for(int i=0;i<q;i++){
    string stmp,ttmp;
    int s,t,k;
    cin>>stmp>>ttmp>>k;
    
    s=stringToVertex(stmp,n);
    t=stringToVertex(ttmp,n);
    
    if(point.size()<s||point.size()<t){
	 cout<<"NA"<<endl;
	 continue;
    }

    vector<double> dist=dijkstra(s,graph,point);
    if(dist[t]==INF){
	 cout<<"NAA"<<endl;
	 continue;
    }
    cout<<dist[t]<<endl;

    vector<int> route=restore(s,t,graph,point,dist);
    for(int i=0;i<route.size();i++){
	 if(i)cout<<" ";
	 if(route[i]>=n){
	   cout<<"C"<<route[i]-n+1;
	 }else cout<<route[i]+1;
    }
    cout<<endl;
  }

  return 0;
}
