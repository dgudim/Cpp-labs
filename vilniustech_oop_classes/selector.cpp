#include <iomanip>
#include <iostream>
#include <map>
#include <math.h>
#include <set>

using namespace std;

class Rect {
  private:
    int width, height;

  public:
    Rect(int w = 6, int h = 9) : width(w), height(h) {}

    int getPerimeter() { return width * 2 + height * 2; }

    int getArea() { return width * height; }

    ~Rect() {}
};

class Triangle {
  private:
    double a, b, c;
    bool print_detroy;

  public:
    Triangle(double a, double b, double c, bool print_detroy = false)
        : a(a), b(b), c(c), print_detroy(print_detroy) {}

    Triangle(bool print_detroy = false) : Triangle(6, 6, 6, print_detroy) {}

    double getArea() {
        double s = (a + b + c) / 2;
        return sqrt(s * (s - a) * (s - b) * (s - c));
    }

    ~Triangle() {
        if (print_detroy) {
            cout << "destroy\n";
        }
    }
};

double distanceBetweenPoints(double x, double y, double x1, double y1) {
    return sqrt((x - x1) * (x - x1) + (y - y1) * (y - y1));
}

class Circle {
  private:
    double x, y, r;

  public:
    Circle(double x, double y, double r) : x(x), y(y), r(r) {}

    bool hasPoint(double p_x, double p_y) {
        return distanceBetweenPoints(x, y, p_x, p_y) <= r;
    }

    bool intersects(const Circle &c) {
        double distance = distanceBetweenPoints(x, y, c.x, c.y);
        return distance <= (r + c.r) && distance >= r - c.r && distance >= c.r - r;
    }
};

int _1_perimeter_rectangles() {
    int w, h;
    cin >> w >> h;
    cout << Rect(w, h).getPerimeter();
    return 0;
}

int _2_area_rectangles() {
    int n, w, h, area = 0;
    cin >> n;
    for (int i = 0; i < n; i++) {
        cin >> w >> h;
        area += Rect(w, h).getArea();
    }
    cout << area;
    return 0;
}

int _3_area_tringles() {
    int n, a, b, c;
    double area = 0;
    cin >> n;
    for (int i = 0; i < n; i++) {
        cin >> a >> b >> c;
        area += Triangle(a, b, c).getArea();
    }
    cout << fixed << setprecision(2) << area;
    return 0;
}

int _4_perimeter_area_rectangles() {
    int mode, w, h;
    cin >> mode >> w >> h;
    if (mode == 1) {
        cout << Rect(w, h).getArea() + Rect().getArea();
    } else {
        cout << Rect(w, h).getPerimeter() + Rect().getPerimeter();
    }
    return 0;
}

int _5_area_triangles_comparison() {
    int a, b, c;
    cin >> a >> b >> c;
    Triangle tr1(a, b, c, true);
    Triangle tr2(true);

    int area1 = tr1.getArea();
    int area2 = tr2.getArea();

    cout << ((area1 > area2) ? "first\n" : (area1 == area2 ? "equal\n" : "second\n"));
    return 0;
}

int _6_circle_point_in_area() {
    int n, p = 0, x, y, d;
    cin >> n;
    for (int i = 0; i < n; i++) {
        cin >> x >> y >> d;
        p += Circle(x, y, d / 2.0).hasPoint(0, 0);
    }
    cout << p;
    return 0;
}

int _7_cicrle_intersection() {
    Circle c(6, 6, 6);
    int n, p = 0, x, y, d;
    cin >> n;
    for (int i = 0; i < n; i++) {
        cin >> x >> y >> d;
        p += Circle(x, y, d / 2.0).intersects(c);
    }
    cout << p;
    return 0;
}

int main() {
    int n;
    cin >> n;
    map<string, int> mapp;
    set<int> score_set;
    set<string> disqual_set;
    for (int i = 0; i < n; i++) {
        string name;
        int m1, m2, m3;
        cin >> name >> m1 >> m2 >> m3;
        int score = m1 * 5 + m2 * 3 + m3 * 15;
        mapp.insert(pair(name, score));
        if (!score_set.insert(score).second) {
            disqual_set.insert(name);
        }
    }
    string max_name;
    int max_score = -1;
    for (auto &[name, score] : mapp) {
        if (score > max_score && !disqual_set.contains(name)) {
            max_name = name;
            max_score = score;
        }
    }
    if (max_score == -1) {
        cout << "all disqualified";
    } else {
        cout << max_name;
    }
    return 0;
}