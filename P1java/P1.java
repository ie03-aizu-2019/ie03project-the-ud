import java.util.*;
//final double INF = 1e9;

class Point {
  double x,y;

  public Point(double x,double y){
    this.x = x;
    this.y = y;
  }



  public Point findIntersection(Point p1,Point p2,Point p3,Point p4){
    final double INF = 1e9;
    Point judge = new Point(0,0);

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


}// end of class Point

class Edge{
  double x,y;
  public Edge(double x,double y){
    this.x = x;
    this.y = y;
  }
}

public class P1{
  public static void main(String[] args){
    int n,m,p,q;

   Scanner scanner = new Scanner(System.in);
    n = scanner.nextInt();
    m = scanner.nextInt();
    p = scanner.nextInt();
    q = scanner.nextInt();

    List<Point> points = new ArrayList<Point>();
    List<Edge> roads = new ArrayList<Edge>();

    double x,y;
    for(int i=0;i<n;i++){

      x = scanner.nextDouble();
      y = scanner.nextDouble();
      Point enter = new Point(x,y);

      points.add(enter);
    }

    for(int i=0;i<m;i++){
      double b,e;
      b = scanner.nextDouble();
      e = scanner.nextDouble();
      Edge enter2 = new Edge(b,e);

      roads.add(enter2);
    }

    List<Point> ans = new ArrayList<Point>();
  }
}
