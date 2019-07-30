import java.util.*;
import java.lang.Math;
import java.util.ArrayList;
import java.util.ArrayDeque;
import java.util.Arrays;
import java.util.List;
import java.util.Queue;
import java.util.PriorityQueue;
import java.util.Scanner;

class Point implements Comparable<Point> {
  public double x, y;
  public String name;
  public Point(double x, double y, String name) {
    this.x = x;
    this.y = y;
    this.name = name;
  }
  double dist(Point p){
    return Math.sqrt((x-p.x)*(x-p.x)+(y-p.y)*(y-p.y));
  }
  public int compareTo(Point o) {
    if ( this.x < o.x ) return -1;
    if ( this.x > o.x ) return 1;
    if ( this.y < o.y ) return -1;
    if ( this.y > o.y ) return 1;
    return 0;
  }
}

class Road {
  public int b, e;
  public Road(int b, int e) {
    this.b = b;
    this.e = e;
  }
}

class Edge {
  public int to;
  public double cost;
  public Edge(int to, double cost) {
    this.to = to;
    this.cost = cost;
  }
}

class P implements Comparable<P> {
  public int first, second;
  public P(int first, int second) {
    this.first = first;
    this.second = second;
  }
  public int compareTo(P o) {
    if ( this.first < o.first ) return -1;
    if ( this.first > o.first ) return 1;
    if ( this.second < o.second ) return -1;
    if ( this.second > o.second ) return 1;
    return 0;
  }
}

class PP implements Comparable<PP> {
  public Point first;
  public P second;
  public PP(Point first, P second) {
    this.first = first;
    this.second = second;
  }
  public int compareTo(PP o) {
    return first.compareTo(o.first);
  }
}

class Pdi implements Comparable<Pdi> {
  public double first;
  public int second;
  public Pdi(double first, int second) {
    this.first = first;
    this.second = second;
  }
  public int compareTo(Pdi o) {
    if ( this.first < o.first ) return -1;
    if ( this.first > o.first ) return 1;
    if ( this.second < o.second ) return -1;
    if ( this.second > o.second ) return 1;
    return 0;
  }
}

class Path implements Comparable<Path> {
  public double dist;
  public ArrayList<Integer> route;
  public int compareTo(Path o) {
    if ( this.dist < o.dist ) return -1;
    if ( this.dist > o.dist ) return 1;
    return 0;
  }
  public Path(double dist, ArrayList<Integer> route) {
    this.dist = dist;
    this.route = route;
  }
}

public class P3 {
  private static Point findIntersection(Point p1, Point p2, Point p3, Point p4) {
    int INF = (int)1e9;
    double EPS = 0.0000000001;
    Point intersection = new Point(INF,INF,"no name");
    double ta=(p3.x-p4.x)*(p1.y-p3.y)+(p3.y-p4.y)*(p3.x-p1.x);
    double tb=(p3.x-p4.x)*(p2.y-p3.y)+(p3.y-p4.y)*(p3.x-p2.x);
    double tc=(p1.x-p2.x)*(p3.y-p1.y)+(p1.y-p2.y)*(p1.x-p3.x);
    double td=(p1.x-p2.x)*(p4.y-p1.y)+(p1.y-p2.y)*(p1.x-p4.x);
    //交差判定 交差していなければINFを返す
    if(tc*td>=0||ta*tb>=0)return intersection;

    //交差点のｘ座標が決まっている場合
    if(Math.abs(p1.x-p2.x) < EPS){
      double x=p1.x;
      double y=x*(p3.y-p4.y)/(p3.x-p4.x)+p3.y-p3.x*(p3.y-p4.y)/(p3.x-p4.x);
      intersection.x=x;
      intersection.y=y;
      return intersection;
    }
    if(Math.abs(p3.x-p4.x) < EPS){
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

  //最短経路の頂点番号を返す関数
  private static ArrayList<Integer> restore(int s,int t,ArrayList<ArrayList<Edge> > graph,ArrayList<Double> dist){
    int N = 30000;
    double EPS = 0.0000000001;
    ArrayList<Integer> route = new ArrayList<Integer>();
    ArrayList<ArrayList<Integer> > graph2 = new ArrayList<ArrayList<Integer> >();//s-t間の最短経路のみを残した有向グラフ
    for ( int i = 0; i < N; i++ ) {
      graph2.add(new ArrayList<Integer>());
    }
    Queue<Integer> que = new ArrayDeque<Integer>();
    int[] used = new int[N];//未探索の頂点は０探索済みなら１
    Arrays.fill(used, 0);
    que.add(t);

    //tからBFSをしてgraph2を構築
    while(!que.isEmpty()){
      int v=que.poll();
      if(used[v] != 0)continue;
      used[v]=1;
      for(int i=0;i<graph.get(v).size();i++){
        int to=graph.get(v).get(i).to;
        double cost=graph.get(v).get(i).cost;
        if(dist.get(to)>dist.get(v))continue;
        if(Math.abs(dist.get(v)-dist.get(to)-cost)<EPS){
          que.add(to);
          graph2.get(to).add(v);
        }
      }
    }

    //sから辞書順に頂点を追加
    int node=s;
    while(true){
      route.add(node);
      if(node==t)break;
      int mi=N+1;
      for(int i=0;i<graph2.get(node).size();i++){
        mi=Math.min(mi,graph2.get(node).get(i));
      }
      node=mi;
    }
    return route;
  }

  private static Path dijkstra(int s,int t,int sz,ArrayList<ArrayList<Edge> > graph){
    Double INF = 1000000000.0;
    ArrayList<Double> dist = new ArrayList<Double>(Collections.nCopies(sz, INF));

    dist.set(s, 0.0);

    PriorityQueue<Pdi> que = new PriorityQueue<Pdi>();
    que.add(new Pdi(0, s));
    while( !que.isEmpty() ){
      Pdi tmp = que.poll();
      int v=tmp.second;
      if(dist.get(v)<tmp.first)continue;
      for(int i=0;i<graph.get(v).size();i++){
        int to=graph.get(v).get(i).to;
        double cost=graph.get(v).get(i).cost;
        if(dist.get(to)>dist.get(v)+cost){
          dist.set(to, dist.get(v)+cost);
          que.add(new Pdi(dist.get(to),to));
        }
      }
    }

    ArrayList<Integer> res = new ArrayList<Integer>();
    if(dist.get(t)>=INF-1) return new Path(INF, res);
    Path path = new Path(dist.get(t),restore(s,t,graph,dist));
    return path;
  }

  private static ArrayList<ArrayList<Edge> > makeGraph(int n,int m,ArrayList<Point> point,ArrayList<Road> road,ArrayList<PP> c){
    int N = 30000;
    ArrayList<ArrayList<Edge> > graph = new ArrayList<ArrayList<Edge> >();//s-t間の最短経路のみを残した有向グラフ
    for ( int i = 0; i < N; i++ ) {
      graph.add(new ArrayList<Edge>());
    }
    //交差点をx軸の負の方向から見ていき線分を分割する
    for(int i=0;i<c.size();i++){
      int edgeNum1=c.get(i).second.first;
      int edgeNum2=c.get(i).second.second;
      int np1=road.get(edgeNum1).b;
      int np2=road.get(edgeNum1).e;
      Point p1=point.get(np1);
      Point p2=point.get(np2);

      if(p1.compareTo(p2) == -1){
        //交差点の乗った線分に対し左(下)側の端点と交差点をグラフに追加
        double cost=p1.dist(point.get(n+i));
        graph.get(np1).add(new Edge(n+i,cost));
        graph.get(n+i).add(new Edge(np1,cost));
        //左(下)側の端点を交差点に更新
        road.get(edgeNum1).b=n+i;
      }else{
        double cost=p2.dist(point.get(n+i));
        graph.get(np2).add(new Edge(n+i,cost));
        graph.get(n+i).add(new Edge(np2,cost));
        road.get(edgeNum1).e=n+i;
      }

      np1=road.get(edgeNum2).b;
      np2=road.get(edgeNum2).e;
      p1=point.get(np1);
      p2=point.get(np2);

      if(p1.compareTo(p2) == -1){
        double cost=p1.dist(point.get(n+i));
        graph.get(np1).add(new Edge(n+i,cost));
        graph.get(n+i).add(new Edge(np1,cost));
        road.get(edgeNum2).b=n+i;
      }else{
        double cost=p2.dist(point.get(n+i));
        graph.get(np2).add(new Edge(n+i,cost));
        graph.get(n+i).add(new Edge(np2,cost));
        road.get(edgeNum2).e=n+i;
      }
    }
    //残った線分をグラフに追加
    for(int i=0;i<road.size();i++){
      int pn1=road.get(i).b,pn2=road.get(i).e;
      double cost=point.get(pn1).dist(point.get(pn2));
      graph.get(pn1).add(new Edge(pn2,cost));
      graph.get(pn2).add(new Edge(pn1,cost));
    }
    return graph;
  }

  private static ArrayList<PP> makeIntersection(ArrayList<Road> road,ArrayList<Point> point){
    int INF = 1000000000;
    ArrayList<PP> intersection = new ArrayList<PP>();
    for(int i=0;i<road.size();i++){
      for(int j=0;j<point.size();j++){
        int b=road.get(i).b;
        int e=road.get(i).e;
        if(b==j||e==j)continue;
        if(point.get(e).compareTo(point.get(b)) == -1) {
          // swap(b,e);
          int tmp = b;
          b = e;
          e = tmp;
        }
        if(point.get(j).compareTo(point.get(b)) == -1||point.get(e).compareTo(point.get(j)) == -1)continue;
        double ax=point.get(b).x-point.get(j).x;
        double ay=point.get(b).y-point.get(j).y;
        double bx=point.get(j).x-point.get(e).x;
        double by=point.get(j).y-point.get(e).y;
        if(ax*by==ay*bx){
          road.add(new Road(j,b));
          road.get(i).b=j;
        }
      }
    }
    for(int i=0;i<road.size();i++){
      for(int j=i+1;j<road.size();j++){
        int a=road.get(i).b;
        int b=road.get(i).e;
        int c=road.get(j).b;
        int d=road.get(j).e;
        if(a==c||a==d||b==c||b==d)continue;
        Point tmp=findIntersection(point.get(a),point.get(b),point.get(c),point.get(d));
        if(tmp.x>=INF-1)continue;
        intersection.add(new PP(tmp,new P(i,j)));
      }
    }
    Collections.sort(intersection);
    return intersection;
  }

  public static void main(String[] args) {
    int n,m,p,q;
    final double INF = 1e9;

    Scanner scanner = new Scanner(System.in);
    n = scanner.nextInt();
    m = scanner.nextInt();
    p = scanner.nextInt();
    q = scanner.nextInt();

    ArrayList<Point> points = new ArrayList<Point>();
    ArrayList<Road> roads = new ArrayList<Road>();
    Map <String,Integer> stov = new HashMap<>();
    Point enter;
    double x,y;
    for(int i=0;i<n;i++){
      x = scanner.nextDouble();
      y = scanner.nextDouble();

      enter = new Point(x,y,String.valueOf(i+1));
      stov.put(enter.name,points.size());
      points.add(enter);
      //System.out.println(points.size());
      //System.out.println(points.get(i).getY());
    }

    for(int i=0;i<m;i++){
      int b,e;
      b = scanner.nextInt();
      e = scanner.nextInt();
      Road enter2 = new Road(b-1,e-1);
      roads.add(enter2);
    }

    ArrayList<PP> intersection=new ArrayList<PP>();
    intersection=makeIntersection(roads,points);
    ArrayList<Point> ans = new ArrayList<Point>();

    for(int i=0;i<intersection.size();i++){
      Point pt = intersection.get(i).first;
      //System.out.println(pt.x + " " + pt.y);
      pt.name="C"+String.valueOf(i+1);
      stov.put(pt.name,points.size());
      points.add(pt);
    }

    ArrayList< ArrayList<Edge> > graph = new ArrayList< ArrayList<Edge> >();
    graph = makeGraph(n,m,points,roads,intersection);
    /*making P4*/
    for(int i=0;i<q;i++){
      String stmp,ttmp;
      int s,t,k;
      stmp = scanner.next();
      ttmp = scanner.next();
      k = scanner.nextInt();

      if(stov.containsKey(stmp)==false ||stov.containsKey(ttmp)==false){
        System.out.println("NA");
        continue;
      }

      s=stov.get(stmp);
      t=stov.get(ttmp);

      Path path=dijkstra(s,t,points.size(), graph);
      if(path.dist==INF){
        System.out.println("NA");
        continue;
      }
      System.out.println(path.dist);
      /*for(int j=0;j<path.route.size();j++){
        if(j>0)System.out.print(" ");
        // cout<<point[path.route[i]].name;
        System.out.print(points.get(path.route.get(j)).name);
      }*/
      //System.out.println("");
    }

    for(int i=0;i<m;i++){
      for(int j=i+1;j<m;j++){
        int a = roads.get(i).b;
        int b = roads.get(i).e;
        int c = roads.get(j).b;
        int d = roads.get(j).e;

        Point tmp = findIntersection(points.get(a-1),points.get(b-1),points.get(c-1),points.get(d-1));
        if(tmp.x==INF) continue;
        ans.add(tmp);

      }
    }

    Collections.sort(ans);


    for(Point pt : ans){
      System.out.println(pt.x + pt.y);
    }

  }
}
