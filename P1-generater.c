#include <stdio.h>
#include <stdlib.h>

int main(){
  int n,m,p=0,q; //初期条件, 2<=n<=1000, 1<=M<=500, p=0, 0<=q<=100
  int x,y; // 0≦x,y≦10000,
  int a,b; // a,bはn以下のランダム
  int c,d,e; //Cを含んだランダム
  int f;
  srand(1);


  n=rand()%998+2;
  m=rand()%500+1;
  q=rand()%100;
  f=rand()%100;

  printf("%d %d %d %d\n",n,m,p,q);
  for(int i=0;i<n;i++){
    x=rand()%10000;
    y=rand()%10000;
    printf("%d %d\n",x,y);
  }
  //return 0;


  for(int i=0;i<m;i++){
    a=rand()%n;
    b=rand()%n;
    printf("%d %d\n",a,b);
  }

  e=1;
  for(int i=0;i<q;i++){
    c=rand()%n;
    d=rand()%n;

    f=rand()%100;
    if(f%2){
      printf("c%d %d %d\n",c,d,e);
    }
    else{
      printf("%d %d %d\n",c,d,e);
    }

  }
}
