#include<bits/stdc++.h>
using namespace std;
#define fi first
#define se second
#define mp make_pair
#define pb push_back
#define all(x) x.begin(),x.end()
#define dbg(x) cout<<#x<<":"<<x<<endl
#define INF 1e9
#define EPS 1e-8
typedef pair<int,int> Pi;
typedef pair<double,double> Pd;
typedef pair<Pd,Pi> PP;
typedef pair<double,int> Pid;

double dist(Pd p1,Pd p2){
  return sqrt((p1.fi-p2.fi)*(p1.fi-p2.fi)+(p1.se-p2.se)*(p1.se-p2.se));
}

vector<double> dijkstra(int s,vector<int> g[],vector<Pd> pt){
  vector<double> d(pt.size(),INF);
  d[s]=0;
  priority_queue<Pid,vector<Pid>,greater<Pid> > que;
  que.push(Pid(0,s));
  while(que.size()){
    Pid p=que.top();
    que.pop();
    int v=p.se;
    //cout<<v<<endl;
    if(d[v]<p.fi)continue;
    for(int i=0;i<g[v].size();i++){
	 int to=g[v][i];
	 if(d[to]>d[v]+dist(pt[v],pt[to])){
	   d[to]=d[v]+dist(pt[v],pt[to]);
	   que.push(Pid(d[to],to));
	 }
    }
  }
  return d;
}

Pd intersect(Pd p1,Pd p2,Pd p3,Pd p4){
  double ta=(p3.fi-p4.fi)*(p1.se-p3.se)+(p3.se-p4.se)*(p3.fi-p1.fi);
  double tb=(p3.fi-p4.fi)*(p2.se-p3.se)+(p3.se-p4.se)*(p3.fi-p2.fi);
  double tc=(p1.fi-p2.fi)*(p3.se-p1.se)+(p1.se-p2.se)*(p1.fi-p3.fi);
  double td=(p1.fi-p2.fi)*(p4.se-p1.se)+(p1.se-p2.se)*(p1.fi-p4.fi);
  if(tc*td>=0||ta*tb>=0)return Pd(INF,INF);
  if(p1.fi==p2.fi){
    double x=p1.fi;
    double y=x*(p3.se-p4.se)/(p3.fi-p4.fi)+p3.se-p3.fi*(p3.se-p4.se)/(p3.fi-p4.fi);
    return Pd(x,y);
  }
  if(p3.fi==p4.fi){
    double x=p3.fi;
    double y=x*(p1.se-p2.se)/(p1.fi-p2.fi)+p1.se-p1.fi*(p1.se-p2.se)/(p1.fi-p2.fi);
    return Pd(x,y);
  }
  double ka=(p1.se-p2.se)/(p1.fi-p2.fi);
  double kb=p1.se-p1.fi*(p1.se-p2.se)/(p1.fi-p2.fi);
  double kc=(p3.se-p4.se)/(p3.fi-p4.fi);
  double kd=p3.se-p3.fi*(p3.se-p4.se)/(p3.fi-p4.fi);
  double x=(kb-kd)/(kc-ka);
  double y=x*(p1.se-p2.se)/(p1.fi-p2.fi)+p1.se-p1.fi*(p1.se-p2.se)/(p1.fi-p2.fi);
  return Pd(x,y);
}

int main(){
  int n,m,p,q;
  cin>>n>>m>>p>>q;
  vector<Pd> pt(n); 
  vector<Pi> rd(m); 
  for(int i=0;i<n;i++){
    double x,y;
    cin>>x>>y;
    pt[i]=Pd(x,y);
  }
  for(int i=0;i<m;i++){
    int b,e;
    cin>>b>>e;
    b--,e--;
    rd[i]=Pi(b,e);
  }

  vector<PP> c;
  for(int i=0;i<m;i++){
    for(int j=i+1;j<m;j++){
	 Pd t=intersect(pt[rd[i].fi],pt[rd[i].se],pt[rd[j].fi],pt[rd[j].se]);
	 if(t.fi==INF)continue;
	 Pi k=Pi(i,j);
	 c.pb(PP(t,k));
    }
  }
  sort(all(c));

  vector<int> g[251111]; 
  for(int i=0;i<c.size();i++){
    pt.pb(c[i].fi);
    Pd p1=pt[rd[c[i].se.fi].fi];
    Pd p2=pt[rd[c[i].se.fi].se];
    int np1=rd[c[i].se.fi].fi;
    int np2=rd[c[i].se.fi].se;
    if(pt[rd[c[i].se.fi].fi]>pt[rd[c[i].se.fi].se]){
	 swap(p1,p2);
	 swap(np1,np2);
    }
    g[np1].pb(n+i);
    g[n+i].pb(np1);
    if(pt[rd[c[i].se.fi].fi]==p1)rd[c[i].se.fi].fi=n+i;
    else rd[c[i].se.fi].se=n+i;

    p1=pt[rd[c[i].se.se].fi];
    p2=pt[rd[c[i].se.se].se];
    np1=rd[c[i].se.se].fi;
    np2=rd[c[i].se.se].se;
    if(pt[rd[c[i].se.se].fi]>pt[rd[c[i].se.se].se]){
	 swap(p1,p2);
	 swap(np1,np2);
    }
    g[np1].pb(n+i);
    g[n+i].pb(np1);
    if(pt[rd[c[i].se.se].fi]==p1)rd[c[i].se.se].fi=n+i;
    else rd[c[i].se.se].se=n+i;
  }

  for(int i=0;i<m;i++){
    g[rd[i].fi].pb(rd[i].se);
    g[rd[i].se].pb(rd[i].fi);
  }

  for(int i=0;i<q;i++){
    string ss,ts;
    int s=-1,t=-1,k;
    cin>>ss>>ts>>k;
    if(ss[0]=='C')s+=n;
    for(int i=0;i<ss.size();i++){
	 if(ss[ss.size()-1-i]=='C')continue;
	 s+=(ss[ss.size()-1-i]-'0')*pow(10,i);
    }
    if(ts[0]=='C')t+=n;
    for(int i=0;i<ts.size();i++){
	 if(ts[ts.size()-1-i]=='C')continue;
	 t+=(ts[ts.size()-1-i]-'0')*pow(10,i);
    }
    //    cout<<s<<" "<<t<<endl;
    if(pt.size()<s||pt.size()<t){
	 cout<<"NA"<<endl;
	 continue;
    }
  
    vector<double> d=dijkstra(s,g,pt);
    if(d[t]==INF)cout<<"NA"<<endl;
    else cout<<d[t]<<endl;
  }
  
  return 0;
}
