#include <iostream>
#include <math.h>
#include <iomanip>

using namespace std;

class Rect {
  private:
    int width, height;

  public:
    Rect(int w, int h) : width(w), height(h) {}

    int getPerimeter() { return width * 2 + height * 2; }

    int getArea() { return width * height; }
};

class Triangle {
  private:
    double a, b, c;

    public:
    Triangle(double a, double b, double c): a(a), b(b), c(c) {

    }

    double getArea() {
        double s = (a + b + c) / 2;
        return sqrt(s * (s - a) * (s - b) * (s - c));
    }
};

int perimeter1_rectangles() {
    int w, h;
    cin >> w >> h;
    cout << Rect(w, h).getPerimeter();
    return 0;
}

int area2_rectangles() {
    int n, w, h, area = 0;
    cin >> n;
    for (int i = 0; i < n; i++) {
        cin >> w >> h;
        area += Rect(w, h).getArea();
    }
    cout << area;
    return 0;
}

int area3_tringles() {
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

int main() {
    
    return 0;
}