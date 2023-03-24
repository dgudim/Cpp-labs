#include <iostream>
#include <math.h>
#include <iomanip>

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
    Triangle(double a, double b, double c, bool print_detroy = false): a(a), b(b), c(c), print_detroy(print_detroy) {

    }

    Triangle(bool print_detroy = false) : Triangle(6, 6, 6, print_detroy) {
        
    }

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
    if(mode == 1) {
        cout << Rect(w, h).getArea() + Rect().getArea();
    } else {
        cout << Rect(w, h).getPerimeter() + Rect().getPerimeter();
    }
    return 0;
}

int main() {
    int a, b, c;
    cin >> a >> b >> c;
    cout << ((Triangle(a, b, c, true).getArea() > Triangle(true).getArea()) ? "first" : "second");
    return 0;
}