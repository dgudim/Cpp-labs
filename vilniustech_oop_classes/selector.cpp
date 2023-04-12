#include <algorithm>
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

class MushroomPicker {
  private:
    string name;
    int oregano, boletus, jungle, points;

  public:
    MushroomPicker(string &name, int oregano, int boletus, int jungle)
        : name(name), oregano(oregano), boletus(boletus), jungle(jungle) {
        this->points = oregano * 5 + boletus * 3 + jungle * 15;
    }

    string getName() const { return name; }

    int getPoints() const { return points; }
};

class HexagonalTube {

    double height, innerRadius, hexRadius;

    constexpr static double PIOnCrack = 355 / 113.0;

  public:
    HexagonalTube(double height, double innerRadius, double hexRadius)
        : height(height), innerRadius(innerRadius), hexRadius(hexRadius) {}

    double getSurfaceArea() {
        return (hexRadius * 6 + 2 * PIOnCrack * innerRadius) * height + // walls and inner hole
               3 * sqrt(3) * hexRadius * hexRadius -
               PIOnCrack * innerRadius * innerRadius * 2; // caps x2
    }

    double getMetalVolume() {
        double outerHexArea = 3 * sqrt(3) / 2 * hexRadius * hexRadius;
        double innerCircleVolume = PIOnCrack * innerRadius * innerRadius;
        return (outerHexArea - innerCircleVolume) * height;
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
        if (passed && accumulator / n_marks > 8.5) {
            n_passed++;
        }
    }
    cout << n_passed;
    return 0;
}

bool compareMushroomPickers(const MushroomPicker &a, const MushroomPicker &b) {
    return a.getPoints() > b.getPoints();
}

int _19_mushroom_pickers() {
    int n, a, b, c;
    cin >> n;
    vector<MushroomPicker> pickers;
    for (int i = 0; i < n; i++) {
        string name;
        cin >> name >> a >> b >> c;
        pickers.push_back(MushroomPicker(name, a, b, c));
    }

    sort(pickers.begin(), pickers.end(), compareMushroomPickers);

    bool allDisqualified = true;
    for (int i = 0; i < n; i++) {
        if ((pickers[i - 1].getPoints() != pickers[i].getPoints() || i == 0) &&
            (pickers[i].getPoints() != pickers[i + 1].getPoints() || i == n - 1)) {
            cout << pickers[i].getName();
            allDisqualified = false;
            break;
        }
    }

    if (allDisqualified) {
        cout << "all disqualified" << std::endl;
    }

    return 0;
}

int _18_hexagonal_tube() {
    int n, len, innerDi, hexDi;
    double surfaceArea = 0;
    cin >> n;
    for (int i = 0; i < n; i++) {
        cin >> len >> innerDi >> hexDi;
        HexagonalTube tube(len, innerDi / 2.0, hexDi / 2.0);
        cout << (i + 1) << " volume: " << fixed << setprecision(3) << tube.getMetalVolume()
             << "\n";
        surfaceArea += tube.getSurfaceArea();
    }
    cout << "Sum of the surface areas: " << surfaceArea;
    return 0;
}

int _17_doors_and_tin() {
    double a, b;
    int n, height, width;
    cin >> a >> b >> n;
    double diagonal = sqrt(a * a + b * b);
    int passed = 0;
    for (int i = 0; i < n; i++) {
        cin >> height >> width;
        passed += (min(height, width) <= max(max(a, b), diagonal));
    }
    cout << passed;
    return 0;
}

int _12_scores() {
    int n, _2p, _2p_good, _3p, _3p_good, pen, pen_good;
    cin >> n;
    int maxScore = INT_MIN, maxPlayer = INT_MIN;
    for (int i = 0; i < n; i++) {
        cin >> _2p >> _2p_good >> _3p >> _3p_good >> pen >> pen_good;
        int score = _2p_good * 3 - _2p + _3p_good * 4 - _3p + pen_good * 2 - pen;
        if (score > maxScore) {
            maxScore = score;
            maxPlayer = i + 1;
        }
    }
    cout << maxPlayer;
    return 0;
}

int _13_scores_accuracy() {
    double n, _2p, _2p_good, _3p, _3p_good, pen, pen_good;
    cin >> n;
    double maxAccuracy = -1;
    int maxScore = INT_MIN;
    for (int i = 0; i < n; i++) {
        cin >> _2p >> _2p_good >> _3p >> _3p_good >> pen >> pen_good;
        double all = _2p + _3p + pen;
        double accuracy = all == 0 ? 0 : ((_2p_good + _3p_good + pen_good) / all);
        int score = _2p_good * 2 + _3p_good * 3 + pen_good;
        if (accuracy > maxAccuracy) {
            maxScore = score;
            maxAccuracy = accuracy;
        }
    }
    cout << maxScore;
    return 0;
}

int main() {
    double n, _2p, _2p_good, _3p, _3p_good, pen, pen_good;
    cin >> n;
    double maxAccuracy = -1;
    int maxScore = INT_MIN;
    for (int i = 0; i < n; i++) {
        cin >> _2p >> _2p_good >> _3p >> _3p_good >> pen >> pen_good;
        double all = _2p + _3p + pen;
        double accuracy = all == 0 ? 0 : ((_2p_good + _3p_good + pen_good) / all);
        int score = _2p_good * 2 + _3p_good * 3 + pen_good;
        if (score > maxScore) {
            maxScore = score;
            maxAccuracy = accuracy;
        }
    }
    cout << fixed << setprecision(2) << maxAccuracy * 100;
    return 0;
}