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
#define N 2000

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
Point findMinDist(Point p1,Point P2,Point P3);

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
    cout<<minp.x<<" "<<minp.y<<endl;
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
    if(flag==1)continue;
    int addn=point.size();
    for(int j=0;j<graph[node1].size();j++){
	 if(graph[node1][j].to==node2){
	   double cost=addp.dist(point[node1]);
	   graph[node1][j]={addn,cost};
	   graph[addn].push_back({node1,cost});
	   break;
	 }
    }
    for(int j=0;j<graph[node2].size();j++){
	 if(graph[node2][j].to==node1){
	   double cost=addp.dist(point[node2]);
	   graph[node2][j]={addn,cost};
	   graph[addn].push_back({node2,cost});
	   break;
	 }
    }
    stov[addp.name]=point.size();
    point.push_back(addp);
    double cost=addp.dist(minp);
    int sz=point.size();
    graph[sz].push_back({sz-1,cost});
    graph[sz-1].push_back({sz,cost});
    stov[minp.name]=point.size();
    point.push_back(minp);
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
