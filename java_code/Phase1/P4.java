import java.util.*;
import java.lang.Math;


class Point implements Comparable<Point>{
  double x,y;
  String name;

  public Point(double x,double y,String name){
    this.x = x;
    this.y = y;
    this.name = name;
  }

  public int compareTo(Point p){
    if(p.getX() == this.getX() ) return (int)(this.getY()-p.getY());
    return (int)(this.getX()-p.getX());
  }

  public static Point findIntersection(Point p1,Point p2,Point p3,Point p4){
    final double INF = 1e9;
    Point judge = new Point(0,0,"judge");

    double ta=(p3.x-p4.x)*(p1.y-p3.y)+(p3.y-p4.y)*(p3.x-p1.x);
    double tb=(p3.x-p4.x)*(p2.y-p3.y)+(p3.y-p4.y)*(p3.x-p2.x);
    double tc=(p1.x-p2.x)*(p3.y-p1.y)+(p1.y-p2.y)*(p1.x-p3.x);
    double td=(p1.x-p2.x)*(p4.y-p1.y)+(p1.y-p2.y)*(p1.x-p4.x);
    //交差判定 交差していなければINFを返す
    if(tc*td>=0||ta*tb>=0){
      judge.x = INF;
      judge.y = INF;
      return judge;
    }

    //交差点のｘ座標が決まっている場合
    if(p1.x==p2.x){
      double x=p1.x;
      double y=x*(p3.y-p4.y)/(p3.x-p4.x)+p3.y-p3.x*(p3.y-p4.y)/(p3.x-p4.x);
      judge.x = x;
      judge.y = y;
      return judge;
    }
    if(p3.x==p4.x){
      double x=p3.x;
      double y=x*(p1.y-p2.y)/(p1.x-p2.x)+p1.y-p1.x*(p1.y-p2.y)/(p1.x-p2.x);
      judge.x = x;
      judge.y = y;
      return judge;
    }

    //交差点のｘ座標が決まってない場合
    double ka=(p1.y-p2.y)/(p1.x-p2.x);
    double kb=p1.y-p1.x*(p1.y-p2.y)/(p1.x-p2.x);
    double kc=(p3.y-p4.y)/(p3.x-p4.x);
    double kd=p3.y-p3.x*(p3.y-p4.y)/(p3.x-p4.x);
    double x=(kb-kd)/(kc-ka);
    double y=x*(p1.y-p2.y)/(p1.x-p2.x)+p1.y-p1.x*(p1.y-p2.y)/(p1.x-p2.x);

    judge.x=x;
    judge.y=y;

    return judge;
  }
  double getX(){
    return x;
  }
  double getY(){
    return y;
  }
  String getName(){
    return name;
  }
  double getDist(Point p){
    return Math.sqrt((x-p.x)*(x-p.x)+(y-p.y)*(y-p.y));
  }

}

class Edge{
  int x,y;
  public Edge(int x,int y){
    this.x = x;
    this.y = y;
  }

  int getB(){
    return x;
  }

  int getE(){
    return y;
  }
}

class EdgeS{
  int to;
  double cost;
  public EdgeS(int to, double cost){
    this.to=to;
    this.cost=cost;
  }
}

class Pdi{
  double dist;
  int n;
  public Pdi(double dist,int n){
    this.dist=dist;
    this.n=n;
  }
}

class Path{
  double dist;
  int route;
  public Path(double dist, int route){
    this.dist=dist;
    this.route=route;
  }

  public static Path dijkstra(int s,int t,List <List<EdgeS> > graph,List<Point> points){
    List<Double> dist = new ArrayList<Double>();
    dist.set(s,0.0);
    Queue<  > que;
  }
}

class PP{
  Point p;
  int x,y;
  final public static double INF = 1e9;


  public PP(Point p,int x,int y){
    this.p = p;
    this.x=x;
    this.y=y;
  }

  public static List<PP> makeIntersection(int m,List<Edge> roads,List<Point> points){
    List<PP> intersection = new ArrayList<PP>();
    for(int i=0;i<m;i++){
      for(int j=i+1;j<m;j++){

        int a = roads.get(i).getB();
        int b = roads.get(i).getE();
        int c = roads.get(j).getB();
        int d = roads.get(j).getE();

        Point tmp=new Point(0,0,"tmp");
        tmp = Point.findIntersection(points.get(a),points.get(b),points.get(c),points.get(d));
        if(tmp.x==INF)continue;
        //intersection.push_back(PP(tmp,P(i,j)));
        PP tmpp = new PP(tmp,i,j);
        intersection.add(tmpp);
      }
    }
    return intersection;
  }
public static List< List<EdgeS> > makeGraph(int n,int m,List<Point> points,List<Edge> roads,List<PP> c){
  List< List<EdgeS> > graph = new ArrayList< List<EdgeS> >();

  for(int i=0;i<c.size();i++){
    int edgeNum1=c.get(i).getX();//x座標
    int edgeNum2=c.get(i).getY();//y座標
    int np1=roads.get(edgeNum1).getB();
    int np2=roads.get(edgeNum1).getE();
    Point p1=points.get(np1);
    Point p2=points.get(np2);


    if(p1.getX()<p2.getX()){
      //交差点の乗った線分に対し左(下)側の端点と交差点をグラフに追加
      double cost=p1.getDist(points.get(n+i));
      EdgeS eds1 = new EdgeS(n+i,cost);
      graph.get(np1).add(eds1);
      EdgeS eds2 = new EdgeS(np1,cost);
      graph.get(n+i).add(eds2);
      //左(下)側の端点を交差点に更新
      //Edge ed = roads.get(edgeNum1);
      int etmp = roads.get(edgeNum1).getE();
      Edge enter_ed = new Edge(n+i,etmp);
      roads.set(edgeNum1,enter_ed);
    }else{
      double cost=p2.getDist(points.get(n+i));
      EdgeS eds1 = new EdgeS(n+i,cost);
      graph.get(np2).add(eds1);
      EdgeS eds2 = new EdgeS(np2,cost);
      graph.get(n+i).add(eds2);

      int btmp = roads.get(edgeNum1).getB();
      Edge enter_ed = new Edge(n+i,btmp);
      roads.set(edgeNum1,enter_ed);
    }

    np1=roads.get(edgeNum2).getB();
    np2=roads.get(edgeNum2).getE();
    p1=points.get(np1);
    p2=points.get(np2);

    if(p1.getX()<p2.getX()){
      //交差点の乗った線分に対し左(下)側の端点と交差点をグラフに追加
      double cost=p1.getDist(points.get(n+i));
      EdgeS eds1 = new EdgeS(n+i,cost);
      graph.get(np1).add(eds1);
      EdgeS eds2 = new EdgeS(np1,cost);
      graph.get(n+i).add(eds2);
      //左(下)側の端点を交差点に更新
      //Edge ed = roads.get(edgeNum1);
      int etmp = roads.get(edgeNum2).getE();
      Edge enter_ed = new Edge(n+i,etmp);
      roads.set(edgeNum2,enter_ed);
    }else{
      double cost=p2.getDist(points.get(n+i));
      EdgeS eds1 = new EdgeS(n+i,cost);
      graph.get(np2).add(eds1);
      EdgeS eds2 = new EdgeS(np2,cost);
      graph.get(n+i).add(eds2);

      int btmp = roads.get(edgeNum2).getB();
      Edge enter_ed = new Edge(n+i,btmp);
      roads.set(edgeNum2,enter_ed);
    }
  }

  for(int i=0;i<m;i++){
    int pn1=roads.get(i).getB(),pn2=roads.get(i).getE();
    double cost=points.get(pn1).getDist(points.get(pn2));
    EdgeS eds1 = new EdgeS(pn2,cost);
    graph.get(pn1).add(eds1);
    EdgeS eds2 = new EdgeS(pn1,cost);
    graph.get(pn2).add(eds2);
  }
return graph;

}
int getX(){
  return x;
}
int getY(){
  return y;
}

Point getPoint(){
    return p;
  }
}
public class P4{
  public static void main(String[] args){
    int n,m,p,q;
    final double INF = 1e9;

   Scanner scanner = new Scanner(System.in);
    n = scanner.nextInt();
    m = scanner.nextInt();
    p = scanner.nextInt();
    q = scanner.nextInt();

    List<Point> points = new ArrayList<Point>();
    List<Edge> roads = new ArrayList<Edge>();
    Map <String,Integer> stov = new HashMap<>();
    Point enter;
    double x,y;
    for(int i=0;i<n;i++){
      x = scanner.nextDouble();
      y = scanner.nextDouble();

      enter = new Point(x,y,String.valueOf(i+1));
      stov.put(enter.getName(),points.size());
      points.add(enter);
      //System.out.println(points.size());
      //System.out.println(points.get(i).getY());
    }

    for(int i=0;i<m;i++){
      int b,e;
      b = scanner.nextInt();
      e = scanner.nextInt();
      Edge enter2 = new Edge(b,e);
      b--;
      e--;
      roads.add(enter2);
    }

    List<PP> intersection=new ArrayList<PP>();
    intersection=PP.makeIntersection(m,roads,points);
    List<Point> ans = new ArrayList<Point>();

    for(int i=0;i<intersection.size();i++){
       Point pt = intersection.get(i).getPoint();
       pt.name="C"+String.valueOf(i+1);
       stov.put(pt.getName(),points.size());
       points.add(pt);
     }

     List< List<EdgeS> > graph = new ArrayList< List<EdgeS> >();
     graph = PP.makeGraph(n,m,points,roads,intersection);
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

       Path path=dijkstra(s,t,graph,points);
       if(path.dist==INF){
         //cout<<"NA"<<endl;
         continue;
       }
     }

     for(int i=0;i<m;i++){
       for(int j=i+1;j<m;j++){
         int a = roads.get(i).getB();
         int b = roads.get(i).getE();
         int c = roads.get(j).getB();
         int d = roads.get(j).getE();

         Point tmp = Point.findIntersection(points.get(a-1),points.get(b-1),points.get(c-1),points.get(d-1));
         if(tmp.x==INF) continue;
         ans.add(tmp);

       }
     }

     Collections.sort(ans);


     for(Point pt : ans){
       System.out.println(pt.getX() + pt.getY());
     }

   }
 }
