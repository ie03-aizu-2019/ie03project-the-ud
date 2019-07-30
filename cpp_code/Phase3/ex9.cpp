#include<iostream>
#include<algorithm>
#include<vector>
#include<math.h>
#include<queue>
#include<string>
#include<map>
#include<set>
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
    return abs(x-right.x)<EPS&&abs(y-right.y)<EPS;
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

typedef pair<int,int> P;
typedef pair<Point,P> PP;//(頂点,(辺番号1,辺番号2))
typedef pair<double,int> Pdi;//(最短距離,ノード番号)


struct Path{
  double dist;
  vector<int> route;
  bool operator<(const Path& right)const{
    return dist==right.dist?route<right.route:dist<right.dist;
  }
  bool operator>(const Path& right)const{
    return dist==right.dist?route>right.route:dist>right.dist;
  }
  bool operator==(const Path& right)const{
    return dist==right.dist&&route==right.route;
  }
};

Point findIntersection(Point p1,Point p2,Point p3,Point p4);
vector<vector<Edge> > makeGraph(int n,int m,vector<Point> point,vector<Road> road,vector<PP> c);
vector<PP> makeIntersection(vector<Road>& road,vector<Point> point);
Path dijkstra(int s,int t,int sz,vector<vector<Edge> >& graph);
vector<int> restore(int s,int t,vector<vector<Edge> >& graph,vector<double> dist);
vector<Path> yensAlgorithm(int k,int s,int t,int sz,vector<vector<Edge> >& graph);
bool findVec(vector<int>& v1,vector<int>& v2);
Point findMinDist(Point p1,Point p2,Point p3);
void addPoint(Point addp,int i,vector<Point>& point,vector<vector<Edge> >& graph,map<string,int>& stov);

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
  
  vector<PP> intersection=makeIntersection(road,point);
  
  //pointに交差点を追加する
  int intersectionCnt=1;
  for(int i=0;i<intersection.size();i++){
   Point pt = intersection[i].first;
   if(i>0&&intersection[i-1].first==pt)continue;
   pt.name="C"+to_string(intersectionCnt++);
   stov[pt.name]=point.size();
   point.push_back(pt);
  }
  
  vector<vector<Edge> > graph=makeGraph(n,m,point,road,intersection);

  for(int i=0;i<p;i++){
    Point addp;
    cin>>addp.x>>addp.y;
    addPoint(addp,i,point,graph,stov);
  }
  
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

    vector<Path> paths=yensAlgorithm(k,s,t,(int)point.size(),graph);
    if(paths.size()==0)cout<<"NA"<<endl;
    for(int j=0;j<paths.size();j++){
	 cout<<paths[j].dist<<endl;

	 for(int l=0;l<paths[j].route.size();l++){
	   if(l)cout<<" ";
	   cout<<point[paths[j].route[l]].name;
	 }
	 cout<<endl;

    }
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
  int icnt=0;
  for(int i=0;i<c.size();i++){
    int edgeNum1=c[i].second.first;
    int edgeNum2=c[i].second.second;
    int np1=road[edgeNum1].b;
    int np2=road[edgeNum1].e;
    Point p1=point[np1];
    Point p2=point[np2];
    
    if(p1<p2){
	 //交差点の乗った線分に対し左(下)側の端点と交差点をグラフに追加
	 double cost=p1.dist(point[n+icnt]);
	 graph[np1].push_back({n+icnt,cost});
	 graph[n+icnt].push_back({np1,cost});
	 //左(下)側の端点を交差点に更新
	 road[edgeNum1].b=n+icnt;	 
    }else{
	 double cost=p2.dist(point[n+icnt]);
	 graph[np2].push_back({n+icnt,cost});
	 graph[n+icnt].push_back({np2,cost});
	 road[edgeNum1].e=n+icnt;	 
    }

    np1=road[edgeNum2].b;
    np2=road[edgeNum2].e;
    p1=point[np1];
    p2=point[np2];

    if(p1<p2){
	 double cost=p1.dist(point[n+icnt]);
	 graph[np1].push_back({n+icnt,cost});
	 graph[n+icnt].push_back({np1,cost});
	 road[edgeNum2].b=n+icnt;	 
    }else{
	 double cost=p2.dist(point[n+icnt]);
	 graph[np2].push_back({n+icnt,cost});
	 graph[n+icnt].push_back({np2,cost});
	 road[edgeNum2].e=n+icnt;	 
    }
    if(i==0||!(c[i-1].first==c[i].first))icnt++;
  }
  //残った線分をグラフに追加
  for(int i=0;i<road.size();i++){
    int pn1=road[i].b,pn2=road[i].e;
    double cost=point[pn1].dist(point[pn2]);
    graph[pn1].push_back({pn2,cost});
    graph[pn2].push_back({pn1,cost});
  }
  return graph;
}

vector<PP> makeIntersection(vector<Road>& road,vector<Point> point){
  vector<PP> intersection;
  for(int i=0;i<road.size();i++){
    for(int j=0;j<point.size();j++){
	 int b=road[i].b;
	 int e=road[i].e;
	 if(b==j||e==j)continue;
	 if(point[e]<point[b])swap(b,e);
	 if(point[j]<point[b]||point[e]<point[j])continue;
	 int ax=point[b].x-point[j].x;
	 int ay=point[b].y-point[j].y;
	 int bx=point[j].x-point[e].x;
	 int by=point[j].y-point[e].y;
	 if(ax*by==ay*bx){
	   road.push_back({j,b});
	   road[i].b=j;
	 }
    }
  }
  for(int i=0;i<road.size();i++){
    for(int j=i+1;j<road.size();j++){
	 int a=road[i].b;
	 int b=road[i].e;
	 int c=road[j].b;
	 int d=road[j].e;
	 if(a==c||a==d||b==c||b==d)continue;
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

bool findVec(vector<int>& v1,vector<int>& v2){
  if(v1.size()>=v2.size())return false;
  for(int i=0;i<v1.size();i++){
    if(v1[i]!=v2[i])return false;
  }
  return true;
}

vector<Path> yensAlgorithm(int k,int s,int t,int sz,vector<vector<Edge> >& graph){
  vector<Path> kShortestPath;
  set<Path> usedPath;
  priority_queue<Path,vector<Path>,greater<Path> > que;
  
  Path tmp =dijkstra(s,t,sz,graph);
  if(tmp.dist==INF)return kShortestPath;
  que.push(tmp);
  usedPath.insert(tmp);
  for(int cn=0;cn<k;cn++){
    if(que.empty())break;
    double dist=que.top().dist;
    vector<int> route=que.top().route;
    que.pop();
    kShortestPath.push_back({dist,route});

    vector<vector<Edge> > tmpGraph=graph;
    double spurDist=0;
    vector<int> spurRoute;
    for(int i=0;i<route.size()-1;i++){
	 int spurNode=route[i];
	 spurRoute.push_back(route[i]);
	 set<int> outNode;
	 
	 for(int j=0;j<kShortestPath.size();j++){
	   if(findVec(spurRoute,kShortestPath[j].route)){
		outNode.insert(kShortestPath[j].route[i+1]);
	   }
	 }
	 
	 for(int j=0;j<tmpGraph[spurNode].size();j++){
	   if(outNode.count(tmpGraph[spurNode][j].to)){
		tmpGraph[spurNode][j].cost=INF;
	   }
	 }
	 
	 Path tmp=dijkstra(spurNode,t,sz,tmpGraph);
	 if(tmp.dist!=INF){
	   tmp.route.insert(tmp.route.begin(),spurRoute.begin(),spurRoute.end()-1);
	   tmp.dist+=spurDist;
	   if(usedPath.count(tmp)==0){
		que.push(tmp);
		usedPath.insert(tmp);
	   }
	 }
	 
	 for(int j=0;j<tmpGraph[spurNode].size();j++){
		tmpGraph[spurNode][j].cost=INF;
		if(graph[spurNode][j].to==route[i+1])
		  spurDist+=graph[spurNode][j].cost;
	 }
    }
  }
  return kShortestPath;
}

void addPoint(Point addp,int i,vector<Point>& point,vector<vector<Edge> >& graph,map<string,int>& stov){
  addp.name=(string)("P"+to_string(i+1));
  Point minp={INF,INF,(string)("PC"+to_string(i+1))};
  int node1,node2;
  for(int j=0;j<point.size();j++){
    for(int k=0;k<graph[j].size();k++){
	 int to=graph[j][k].to;
	 Point p=findMinDist(addp,point[j],point[to]);
	 if(addp.dist(p)<addp.dist(minp)){
	   minp.x=p.x;
	   minp.y=p.y;
	   node1=j;
	   node2=to;
	 }
    }
  }
  int flag=0;
  for(int j=0;j<point.size();j++){
    if(minp==point[j]){
	 int sz=point.size();
	 double cost=addp.dist(point[j]);
	 graph[sz].push_back({j,cost});
	 graph[j].push_back({sz,cost});
	 stov[addp.name]=sz;
	 point.push_back(addp);
	 flag=1;
	 break;
    }
  }
  if(flag==1)return;
  int addn=point.size();
  for(int j=0;j<graph[node1].size();j++){
    if(graph[node1][j].to==node2){
	 double cost=minp.dist(point[node1]);
	 graph[node1][j]={addn,cost};
	 graph[addn].push_back({node1,cost});
	 break;
    }
  }
  for(int j=0;j<graph[node2].size();j++){
    if(graph[node2][j].to==node1){
	 double cost=minp.dist(point[node2]);
	 graph[node2][j]={addn,cost};
	 graph[addn].push_back({node2,cost});
	 break;
    }
  }
  stov[minp.name]=point.size();
  point.push_back(minp);
  double cost=addp.dist(minp);
  int sz=point.size();
  graph[sz].push_back({sz-1,cost});
  graph[sz-1].push_back({sz,cost});
  stov[addp.name]=point.size();
  point.push_back(addp);
}

Point findMinDist(Point p1,Point p2,Point p3){
  double a=p3.x-p2.x;
  double b=p3.y-p2.y;
  double a2=a*a;
  double b2=b*b;
  double r=a2+b2;
  double t=-(a*(p2.x-p1.x)+b*(p2.y-p1.y))/r;
  if(t<=0)return p2;
  if(t>=1)return p3;
  return (Point){(p3.x-p2.x)*t+p2.x,(p3.y-p2.y)*t+p2.y,""};
}
