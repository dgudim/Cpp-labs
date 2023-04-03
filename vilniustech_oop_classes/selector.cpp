#include <iomanip>
#include <iostream>
#include <limits.h>
#include <map>
#include <math.h>
#include <set>
#include <vector>

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

    bool isValid() { return !(a + b <= c || a + c <= b || b + c <= a); }

    bool isObtuse() {
        double longestSize = max(max(a, b), c);
        double shortSidesSquare;
        if (longestSize == a) {
            shortSidesSquare = b * b + c * c;
        } else if (longestSize == b) {
            shortSidesSquare = a * a + c * c;
        } else {
            shortSidesSquare = b * b + a * a;
        }
        return shortSidesSquare < longestSize * longestSize;
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

bool collinear(int x1, int y1, int x2, int y2, int x3, int y3) {
    return (y1 - y2) * (x1 - x3) == (y1 - y3) * (x1 - x2);
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

int _8_triangles_obtuse() {
    int n, x1, x2, x3, y1, y2, y3, n_obtuse = 0, n_valid = 0;
    cin >> n;
    for (int i = 0; i < n; i++) {
        cin >> x1 >> x2 >> x3 >> y1 >> y2 >> y3;
        if (collinear(x1, y1, x2, y2, x3, y3)) {
            continue;
        }
        double side1 = distanceBetweenPoints(x1, y1, x2, y2);
        double side2 = distanceBetweenPoints(x2, y2, x3, y3);
        double side3 = distanceBetweenPoints(x3, y3, x1, y1);
        Triangle tr(side1, side2, side3);
        if (tr.isValid()) {
            n_valid++;
            n_obtuse += tr.isObtuse();
        }
    }
    if (n_valid == 0) {
        cout << "NA";
    } else {
        cout << n_obtuse;
    }
    return 0;
}

void printTime(long time) {
    int hours, minutes, seconds;
    hours = time / 3600;
    time -= hours * 3600;
    minutes = time / 60;
    time -= minutes * 60;
    seconds = time;
    cout << setw(2) << setfill('0') << hours << ":" << setw(2) << setfill('0') << minutes
         << ":" << setw(2) << setfill('0') << seconds << "\n";
}

long HourMinSecToSec(int hours, int minutes, int seconds) {
    return (hours * 60 + minutes) * 60 + seconds;
}

int _9_time_division() {
    int hours, minutes, seconds, n_divisions;
    long time;
    cin >> n_divisions >> hours >> minutes >> seconds;
    time = HourMinSecToSec(hours, minutes, seconds);

    for (int i = 0; i < n_divisions; i++) {
        time /= 3;
        printTime(time);
    }

    return 0;
}

int _10_max_time_sum() {
    int hours, minutes, seconds, n;
    cin >> n;
    long max_time = 0;
    vector<long> times(n);
    for (int i = 0; i < n; i++) {
        cin >> hours >> minutes >> seconds;
        long time = HourMinSecToSec(hours, minutes, seconds);
        times[i] = time;
    }
    for (int i = 0; i < times.size() - 1; i++) {
        for (int i2 = i + 1; i2 < times.size(); i2++) {
            long time = times[i] + times[i2];
            time -= (time / (24L * 3600)) * 24L * 3600;
            if (time > max_time) {
                max_time = time;
            }
        }
    }
    printTime(max_time);
    return 0;
}

int _11_time_delta() {
    int hours, minutes, seconds, n;
    cin >> n;
    long min_delta = INT_MAX;
    vector<long> times(n);
    for (int i = 0; i < n; i++) {
        cin >> hours >> minutes >> seconds;
        long time = HourMinSecToSec(hours, minutes, seconds);
        times[i] = time;
    }
    const long _24H = 24 * 3600;
    for (int i = 0; i < times.size() - 1; i++) {
        for (int i2 = i + 1; i2 < times.size(); i2++) {
            long delta = abs(times[i] - times[i2]);

            // handle time overflows
            delta = min(delta, times[i2] - times[i] + _24H);
            delta = min(delta, times[i] - times[i2] + _24H);

            if (delta < min_delta) {
                min_delta = delta;
            }
        }
    }
    printTime(min_delta);
    return 0;
}

int _15_exam_marks() {
    int n, n_passed;
    string best_student;
    double max_average = 0;
    cin >> n;
    for (int i = 0; i < n; i++) {
        string name;
        int n1, n2, n3, n4, n5;
        cin >> name >> n1 >> n2 >> n3 >> n4 >> n5;
        double average = (n1 + n2 + n3 + n4 + n5) / 5.0;
        if (average > max_average) {
            max_average = average;
            best_student = name;
        }
        if (average > 9) {
            n_passed++;
        }
    }
    cout << n_passed << "\n" << best_student;
    return 0;
}

int _16_certificate_of_honor() {
    int n, n_marks, n_passed, mark;
    cin >> n;
    for (int i = 0; i < n; i++) {
        cin >> n_marks;
        bool passed = true;
        double accumulator = 0;
        for (int m = 0; m < n_marks; m++) {
            cin >> mark;
            if (mark < 8) {
                passed = false;
            }
            if (passed) {
                accumulator += mark;
            }
        }
        if(passed && accumulator / n_marks > 8.5) {
            n_passed ++;
        }
    }
    cout << n_passed;
    return 0;
}

int main() {
    int n;
    cin >> n;
    map<string, int> mapp;
    set<int> score_set;
    set<int> disqual_set;
    for (int i = 0; i < n; i++) {
        string name;
        int m1, m2, m3;
        cin >> name >> m1 >> m2 >> m3;
        int score = m1 * 5 + m2 * 3 + m3 * 15;
        mapp.insert(pair(name, score));
        if (!score_set.insert(score).second) {
            disqual_set.insert(score);
        }
    }
    string max_name;
    int max_score = -1;
    for (auto &[name, score] : mapp) {
        if (score > max_score && !disqual_set.contains(score)) {
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

int main2() {
    return 0;
}