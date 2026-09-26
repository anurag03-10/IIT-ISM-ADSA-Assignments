#include <iostream>
#include <vector>
#include <cmath>
#include <random>
#include <chrono>
#include <iomanip>
#include <algorithm>
#include <string>
using namespace std;

struct Point { double x, y; string name; };
struct Segment { Point a, b; string name; };

int sign(double x) { if (x > 0) return 1; if (x < 0) return -1; return 0; }

double cross(const Point &o, const Point &a, const Point &b) {
    return (a.x - o.x) * (b.y - o.y) - (a.y - o.y) * (b.x - o.x);
}

bool onSegment(const Point &p, const Point &q, const Point &r) {
    // q on pr?
    return q.x <= max(p.x, r.x) + 1e-9 && q.x + 1e-9 >= min(p.x, r.x)
        && q.y <= max(p.y, r.y) + 1e-9 && q.y + 1e-9 >= min(p.y, r.y);
}

int orientation(const Point &p, const Point &q, const Point &r) {
    double val = cross(p,q,r);
    if (fabs(val) < 1e-9) return 0; // collinear
    return (val > 0)? 1: 2; // 1: counterclockwise, 2: clockwise
}

bool segmentsIntersect(const Segment &s1, const Segment &s2, string &type, Point &ip) {
    Point p1 = s1.a, q1 = s1.b, p2 = s2.a, q2 = s2.b;
    int o1 = orientation(p1, q1, p2);
    int o2 = orientation(p1, q1, q2);
    int o3 = orientation(p2, q2, p1);
    int o4 = orientation(p2, q2, q1);

    // General case
    if (o1 != o2 && o3 != o4) {
        type = "Proper crossing";
        // compute intersection point of two lines
        double A1 = q1.y - p1.y;
        double B1 = p1.x - q1.x;
        double C1 = A1 * p1.x + B1 * p1.y;

        double A2 = q2.y - p2.y;
        double B2 = p2.x - q2.x;
        double C2 = A2 * p2.x + B2 * p2.y;

        double det = A1 * B2 - A2 * B1;
        if (fabs(det) < 1e-12) return true; // nearly parallel
        ip.x = (B2 * C1 - B1 * C2) / det;
        ip.y = (A1 * C2 - A2 * C1) / det;
        return true;
    }

    // Special Cases
    // p2 on p1q1
    if (o1 == 0 && onSegment(p1, p2, q1)) { type = "Touching at endpoint or collinear overlap"; ip = p2; return true; }
    if (o2 == 0 && onSegment(p1, q2, q1)) { type = "Touching at endpoint or collinear overlap"; ip = q2; return true; }
    if (o3 == 0 && onSegment(p2, p1, q2)) { type = "Touching at endpoint or collinear overlap"; ip = p1; return true; }
    if (o4 == 0 && onSegment(p2, q1, q2)) { type = "Touching at endpoint or collinear overlap"; ip = q1; return true; }

    // Check collinear overlapping
    if (o1 == 0 && o2 == 0 && o3 == 0 && o4 == 0) {
        // check overlap
        auto between = [&](double a,double b,double c){ return max(min(a,b), min(b,c)) <= min(max(a,b), max(b,c)) + 1e-9; };
        bool overlap = (onSegment(p1,p2,q1) || onSegment(p1,q2,q1) || onSegment(p2,p1,q2) || onSegment(p2,q1,q2));
        if (overlap) { type = "Overlapping/Collinear"; ip = {0,0, string()}; return true; }
    }

    type = "No intersection";
    return false;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cout << "Enter number of operational locations: ";
    int n; if (!(cin >> n)) return 0;
    vector<Point> pts; pts.reserve(n);
    for (int i = 0; i < n; ++i) {
        double x,y; cin >> x >> y;
        pts.push_back({x,y, string("L") + to_string(i+1)});
    }

    cout << "Enter monitoring rectangle (x1 y1 x2 y2): ";
    double x1,y1,x2,y2; cin >> x1 >> y1 >> x2 >> y2;
    double rx1 = min(x1,x2), rx2 = max(x1,x2), ry1 = min(y1,y2), ry2 = max(y1,y2);

    cout << "Enter number of truck routes: ";
    int m; cin >> m;
    vector<Segment> segs; segs.reserve(m);
    for (int i = 0; i < m; ++i) {
        double sx,sy, ex,ey; cin >> sx >> sy >> ex >> ey;
        Segment s; s.a = {sx,sy, string("R") + to_string(i+1)}; s.b = {ex,ey, string("R") + to_string(i+1)}; s.name = string("R") + to_string(i+1);
        segs.push_back(s);
    }

    // Query points inside rectangle
    int examined = 0; vector<Point> inside;
    for (auto &p : pts) {
        ++examined;
        if (p.x >= rx1 - 1e-9 && p.x <= rx2 + 1e-9 && p.y >= ry1 - 1e-9 && p.y <= ry2 + 1e-9) inside.push_back(p);
    }

    cout << "Points inside monitoring region:\n";
    for (auto &p : inside) cout << p.name << " (" << p.x << "," << p.y << ")\n";
    cout << "Points examined: " << examined << "\n\n";

    int intersections = 0;
    cout << "Route intersection report:\n";
    for (int i = 0; i < m; ++i) {
        for (int j = i+1; j < m; ++j) {
            string type; Point ip; bool inter = segmentsIntersect(segs[i], segs[j], type, ip);
            cout << segs[i].name << " and " << segs[j].name << " : ";
            if (!inter || type == "No intersection") cout << "No intersection";
            else {
                cout << type;
                if (type == "Proper crossing" || type.find("Touching")!=string::npos) {
                    cout << " at (" << fixed << setprecision(3) << ip.x << "," << ip.y << ")";
                }
                if (type == "Overlapping/Collinear") cout << "";
                ++intersections;
            }
            cout << '\n';
        }
    }

    cout << "\nPoints inside monitoring region = " << inside.size() << "\n";
    cout << "Number of route intersections   = " << intersections << "\n";

    // Experimental extension: prompt user for random experiment
    cout << "\nRun experimental random test? (y/n): "; char ch; cin >> ch;
    if (ch == 'y' || ch == 'Y') {
        int P,S; cout << "Enter number of random points and segments: "; cin >> P >> S;
        double bound; cout << "Enter coordinate bound (points in [0,bound]): "; cin >> bound;
        vector<Point> rpts; vector<Segment> rsegs;
        std::mt19937_64 rng(chrono::high_resolution_clock::now().time_since_epoch().count());
        uniform_real_distribution<double> dist(0.0, bound);
        for (int i=0;i<P;++i) rpts.push_back({dist(rng), dist(rng), string("p")+to_string(i+1)});
        for (int i=0;i<S;++i) {
            Point a={dist(rng),dist(rng),""}, b={dist(rng),dist(rng),""};
            rsegs.push_back({a,b,string("s")+to_string(i+1)});
        }
        // rectangle center quarter
        double qx1 = bound*0.25, qy1 = bound*0.25, qx2 = bound*0.75, qy2 = bound*0.75;

        auto t1 = chrono::high_resolution_clock::now();
        int cnt=0; for (auto &p:rpts) if (p.x>=qx1 && p.x<=qx2 && p.y>=qy1 && p.y<=qy2) ++cnt;
        auto t2 = chrono::high_resolution_clock::now();
        chrono::duration<double> dt_points = t2 - t1;

        auto t3 = chrono::high_resolution_clock::now();
        int cnti=0;
        for (int i=0;i<S;++i) for (int j=i+1;j<S;++j) { string type; Point ip; bool inter = segmentsIntersect(rsegs[i], rsegs[j], type, ip); if (inter && type!="No intersection") ++cnti; }
        auto t4 = chrono::high_resolution_clock::now();
        chrono::duration<double> dt_segs = t4 - t3;

        cout << "\nExperimental result:\n";
        cout << "Points found inside rect: " << cnt << ", time = " << dt_points.count() << " s\n";
        cout << "Segment intersections found: " << cnti << ", time = " << dt_segs.count() << " s\n";
    }

    return 0;
}
