#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 1000
#define M 500
#define XY 10000

int main(){
// プログラム実行ごとに異なるパターンが使える
  unsigned int now = (unsigned int)time( 0 );
  srand(now);

  int n,m,p=0,q=rand()%100; //初期条件, 2<=n<=1000, 1<=M<=500, p=0, 0<=q<=100
  int x,y; // 0≦x,y≦10000,
  int a,b; // a,bはn以下のランダム
  int c,d,e; //Cを含んだランダム
  // プログラム実行ごとに異なるパターンが使える



  n=rand()%(N-1)+2; //999+2
  m=rand()%M+1; //500+1
  printf("%d %d %d %d\n",n,m,p,q);
  for(int i=0;i<n;i++){
    x=rand()%XY;
    y=rand()%XY;
    printf("%d %d\n",x,y);
  }
  //return 0;


  for(int i=0;i<m;i++){
    a=rand()%(n-1)+1;
    b=rand()%(n-1)+1;
    printf("%d %d\n",a,b);
  }


  for(int i=0;i<q;i++){
    c=rand()%(n-1)+1;
    d=rand()%(n-1)+1;
    e=1;

    printf("%d %d %d\n",c,d,e);
  }
  return 0;
}
