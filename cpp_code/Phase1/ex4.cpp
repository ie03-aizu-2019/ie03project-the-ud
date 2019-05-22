#include<iostream>
#include<algorithm>
#include<vector>
#include<math.h>
#include<queue>
#include<string>
#include<map>
using namespace std;
#define INF 1e9
#define EPS 1e-8
#define N 30000

struct Point{
  double x,y;
  string name;  
  
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

struct Road{
  int b,e;
};

struct Edge{
  int to;
  double cost;
};

struct Path{
  double dist;
  vector<int> route;
};

Point findIntersection(Point p1,Point p2,Point p3,Point p4);
typedef pair<int,int> P;
typedef pair<Point,P> PP;//(頂点,(辺番号1,辺番号2))
vector<vector<Edge> > makeGraph(int n,int m,vector<Point> point,vector<Road> road,vector<PP> c);
vector<PP> makeIntersection(int m,vector<Road> road,vector<Point> point);
typedef pair<double,int> Pdi;//(最短距離、ノード番号)
Path dijkstra(int s,int t,int sz,vector<vector<Edge> >& graph);
vector<int> restore(int s,int t,vector<vector<Edge> >& graph,vector<double> dist);

int main(){
  int n,m,p,q;
  cin>>n>>m>>p>>q;
  
  vector<Point> point;
  vector<Road> road;
  map<string,int> stov;
  
  for(int i=0;i<n;i++){
    double x,y;
    cin>>x>>y;
    Point pt={x,y,to_string(i+1)};
    stov[pt.name]=point.size();
    point.push_back(pt);
  }
  
  for(int i=0;i<m;i++){
    int b,e;
    cin>>b>>e;
    b--,e--;//頂点は0-indexedで管理
    road.push_back({b,e});
  }

  vector<PP> intersection=makeIntersection(m,road,point);

  //pointに交差点を追加する
  for(int i=0;i<intersection.size();i++){
   Point pt = intersection[i].first;
   pt.name="C"+to_string(i+1);
   stov[pt.name]=point.size();
   point.push_back(pt);
  }
  
  vector<vector<Edge> > graph=makeGraph(n,m,point,road,intersection);
  
  for(int i=0;i<q;i++){
    string stmp,ttmp;
    int s,t,k;
    cin>>stmp>>ttmp>>k;

    if(stov.count(stmp)==0||stov.count(ttmp)==0){
	 cout<<"NA"<<endl;
	 continue;
    }
    
    s=stov[stmp];
    t=stov[ttmp];
   
    Path path=dijkstra(s,t,(int)point.size(),graph);
    if(path.dist==INF){
	 cout<<"NA"<<endl;
	 continue;
    }
    
    cout<<path.dist<<endl;
    for(int i=0;i<path.route.size();i++){
	 if(i)cout<<" ";
	 int pn=path.route[i];
	 cout<<point[pn].name;
    }
    cout<<endl;
  }

  return 0;
}

Point findIntersection(Point p1,Point p2,Point p3,Point p4){
  Point intersection = {INF,INF,"no name"};
  double ta=(p3.x-p4.x)*(p1.y-p3.y)+(p3.y-p4.y)*(p3.x-p1.x);
  double tb=(p3.x-p4.x)*(p2.y-p3.y)+(p3.y-p4.y)*(p3.x-p2.x);
  double tc=(p1.x-p2.x)*(p3.y-p1.y)+(p1.y-p2.y)*(p1.x-p3.x);
  double td=(p1.x-p2.x)*(p4.y-p1.y)+(p1.y-p2.y)*(p1.x-p4.x);
  //交差判定 交差していなければINFを返す
  if(tc*td>=0||ta*tb>=0)return intersection;

  //交差点のｘ座標が決まっている場合
  if(p1.x==p2.x){
    double x=p1.x;
    double y=x*(p3.y-p4.y)/(p3.x-p4.x)+p3.y-p3.x*(p3.y-p4.y)/(p3.x-p4.x);
    intersection.x=x;
    intersection.y=y;
    return intersection;
  } 
  if(p3.x==p4.x){
    double x=p3.x;
    double y=x*(p1.y-p2.y)/(p1.x-p2.x)+p1.y-p1.x*(p1.y-p2.y)/(p1.x-p2.x);
    intersection.x=x;
    intersection.y=y;
    return intersection;
  }

  //交差点のｘ座標が決まってない場合
  double ka=(p1.y-p2.y)/(p1.x-p2.x);
  double kb=p1.y-p1.x*(p1.y-p2.y)/(p1.x-p2.x);
  double kc=(p3.y-p4.y)/(p3.x-p4.x);
  double kd=p3.y-p3.x*(p3.y-p4.y)/(p3.x-p4.x);
  double x=(kb-kd)/(kc-ka);
  double y=x*(p1.y-p2.y)/(p1.x-p2.x)+p1.y-p1.x*(p1.y-p2.y)/(p1.x-p2.x);
  intersection.x=x;
  intersection.y=y;
  return intersection;
}


Path dijkstra(int s,int t,int sz,vector<vector<Edge> >& graph){
  vector<double> dist(sz,INF);//dist[i]:=stratから頂点iに対する最短距離
  dist[s]=0;
  priority_queue<Pdi,vector<Pdi>,greater<Pdi> > que;
  que.push(Pdi(0,s));
  while(que.size()>0){
    Pdi tmp=que.top();
    que.pop();
    int v=tmp.second;
    if(dist[v]<tmp.first)continue;
    for(int i=0;i<graph[v].size();i++){
	 int to=graph[v][i].to;
	 double cost=graph[v][i].cost;
	 if(dist[to]>dist[v]+cost){
	   dist[to]=dist[v]+cost;
	   que.push(Pdi(dist[to],to));
	 }
    }
  }
  if(dist[t]==INF)return {INF,vector<int>()};
  Path path={dist[t],restore(s,t,graph,dist)};
  return path;
}

vector<vector<Edge> > makeGraph(int n,int m,vector<Point> point,vector<Road> road,vector<PP> c){
  vector<vector<Edge> > graph(N);

  //交差点をx軸の負の方向から見ていき線分を分割する
  for(int i=0;i<c.size();i++){
    int edgeNum1=c[i].second.first;
    int edgeNum2=c[i].second.second;
    int np1=road[edgeNum1].b;
    int np2=road[edgeNum1].e;
    Point p1=point[np1];
    Point p2=point[np2];

    if(p1<p2){
	 //交差点の乗った線分に対し左(下)側の端点と交差点をグラフに追加
	 double cost=p1.dist(point[n+i]);
	 graph[np1].push_back({n+i,cost});
	 graph[n+i].push_back({np1,cost});
	 //左(下)側の端点を交差点に更新
	 road[edgeNum1].b=n+i;	 
    }else{
	 double cost=p2.dist(point[n+i]);
	 graph[np2].push_back({n+i,cost});
	 graph[n+i].push_back({np2,cost});
	 road[edgeNum1].e=n+i;	 
    }

    np1=road[edgeNum2].b;
    np2=road[edgeNum2].e;
    p1=point[np1];
    p2=point[np2];

    if(p1<p2){
	 double cost=p1.dist(point[n+i]);
	 graph[np1].push_back({n+i,cost});
	 graph[n+i].push_back({np1,cost});
	 road[edgeNum2].b=n+i;	 
    }else{
	 double cost=p2.dist(point[n+i]);
	 graph[np2].push_back({n+i,cost});
	 graph[n+i].push_back({np2,cost});
	 road[edgeNum2].e=n+i;	 
    }   
  }
  //残った線分をグラフに追加
  for(int i=0;i<m;i++){
    int pn1=road[i].b,pn2=road[i].e;
    double cost=point[pn1].dist(point[pn2]);
    graph[pn1].push_back({pn2,cost});
    graph[pn2].push_back({pn1,cost});
  }
  return graph;
}

vector<PP> makeIntersection(int m,vector<Road> road,vector<Point> point){
  vector<PP> intersection;
  for(int i=0;i<m;i++){
    for(int j=i+1;j<m;j++){
	 int a=road[i].b;
	 int b=road[i].e;
	 int c=road[j].b;
	 int d=road[j].e;
	 Point tmp=findIntersection(point[a],point[b],point[c],point[d]);
	 if(tmp.x==INF)continue;
	 intersection.push_back(PP(tmp,P(i,j)));
    }
  }
  sort(intersection.begin(),intersection.end());
  return intersection;
}

//最短経路の頂点番号を返す関数
vector<int> restore(int s,int t,vector<vector<Edge> >& graph,vector<double> dist){
  vector<int> route;
  vector<vector<int> > graph2(N);//s-t間の最短経路のみを残した有向グラフ
  queue<int> que;
  int used[N]={};//未探索の頂点は０探索済みなら１
  que.push(t);
  
  //tからBFSをしてgraph2を構築
  while(que.size()){
    int v=que.front();
    que.pop();
    if(used[v])continue;
    used[v]=1;
    for(int i=0;i<graph[v].size();i++){
	 int to=graph[v][i].to;
	 double cost=graph[v][i].cost;
	 if(dist[to]>dist[v])continue;
	 if(abs(dist[v]-dist[to]-cost)<EPS){
	   que.push(to);
	   graph2[to].push_back(v);
	 }
    }
  }
  
  //sから辞書順に頂点を追加
  int node=s;
  while(1){
    route.push_back(node);
    if(node==t)break;
    int mi=N+1;
    for(int i=0;i<graph2[node].size();i++){
	 mi=min(mi,graph2[node][i]);
    }
    node=mi;
  }
  return route;
}
