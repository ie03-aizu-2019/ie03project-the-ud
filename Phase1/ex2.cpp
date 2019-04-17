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

  vector<Pd> ans;
  for(int i=0;i<m;i++){
    for(int j=i+1;j<m;j++){
	 Pd t=intersect(pt[rd[i].fi],pt[rd[i].se],pt[rd[j].fi],pt[rd[j].se]);
	 if(t.fi==INF)continue;
	 ans.pb(t);
    }
  }

  sort(all(ans));
  for(int i=0;i<ans.size();i++){
    cout<<ans[i].fi<<" "<<ans[i].se<<endl;
  }
  
  return 0;
}
