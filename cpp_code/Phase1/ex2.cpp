#include<iostream>
#include<algorithm>
#include<vector>
using namespace std;
#define INF 1e9
#define EPS 1e-8

struct Point{
  double x,y;
  bool operator<(const Point& right)const{
    return x==right.x?y<right.y:x<right.x;
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

  vector<Point> ans;

  //ansに交差点を追加
  for(int i=0;i<m;i++){
    for(int j=i+1;j<m;j++){
	 int a=road[i].b;
	 int b=road[i].e;
	 int c=road[j].b;
	 int d=road[j].e;
	 Point tmp=findIntersection(point[a],point[b],point[c],point[d]);
	 if(tmp.x==INF)continue;
	 ans.push_back(tmp);
    }
  }

  sort(ans.begin(),ans.end());
  
  for(int i=0;i<ans.size();i++){
    cout<<ans[i].x<<" "<<ans[i].y<<endl;
  }

  return 0;
}
