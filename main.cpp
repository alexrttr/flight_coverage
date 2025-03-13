#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <cmath>
#include <iomanip>

using namespace std;

const double VIEW_DIST = 50.0;
const double TOLERANCE = 0.001;  // 1 meter precision
const double EPS = 1e-9;

struct Flight {
    double A, B, C;          // Normalized line equation
    double min_x, max_x;     // Expanded bounding box
    double min_y, max_y;
};

struct Rectangle {
    double left, bottom, right, top;
};

vector<pair<double, double>> get_key_points(int L) {
    return {
        {0,0}, {L,0}, {0,L}, {L,L},          // Corners
        {L/2.0, L/2.0},                      // Center
        {0, L/2.0}, {L/2.0, 0},              // Edge midpoints
        {L, L/2.0}, {L/2.0, L},              // Edge midpoints
        {L/4.0, 3*L/4.0}, {3*L/4.0, L/4.0},  // Diagonals
        {L/4.0, L/4.0}, {3*L/4.0, 3*L/4.0}   // Quarter points
    };
}

bool quick_check(const vector<Flight>& flights, int L, double& x, double& y) {
    for (auto& p : get_key_points(L)) {
        bool covered = false;
        for (const auto& f : flights) {
            double dist = fabs(f.A * p.first + f.B * p.second + f.C);
            if (dist <= VIEW_DIST + EPS) {
                covered = true;
                break;
            }
        }
        if (!covered) {
            x = p.first;
            y = p.second;
            return true;
        }
    }
    return false;
}

bool is_rectangle_covered(const Rectangle& rect, const vector<Flight>& flights) {
    for (const auto& f : flights) {
        if (rect.right < f.min_x || rect.left > f.max_x ||
            rect.top < f.min_y || rect.bottom > f.max_y) continue;

        bool covers = true;
        for (double x : {rect.left, rect.right}) {
            for (double y : {rect.bottom, rect.top}) {
                if (fabs(f.A * x + f.B * y + f.C) > VIEW_DIST + EPS) {
                    covers = false;
                    break;
                }
            }
            if (!covers) break;
        }
        if (covers) return true;
    }
    return false;
}

bool find_uncovered(const vector<Flight>& flights, int L, double& x, double& y) {
    // First check strategic points
    if (quick_check(flights, L, x, y)) return true;

    queue<Rectangle> q;
    q.push({0.0, 0.0, (double)L, (double)L});

    while (!q.empty()) {
        auto rect = q.front();
        q.pop();

        if (is_rectangle_covered(rect, flights)) continue;

        double w = rect.right - rect.left;
        double h = rect.top - rect.bottom;
        if (w <= TOLERANCE && h <= TOLERANCE) {
            x = (rect.left + rect.right) / 2;
            y = (rect.bottom + rect.top) / 2;
            return true;
        }

        double mx = (rect.left + rect.right) / 2;
        double my = (rect.bottom + rect.top) / 2;
        
        q.push({rect.left, rect.bottom, mx, my});
        q.push({mx, rect.bottom, rect.right, my});
        q.push({rect.left, my, mx, rect.top});
        q.push({mx, my, rect.right, rect.top});
    }
    return false;
}

bool read_input(const string& filename, int& L, vector<Flight>& flights) {
    ifstream fin(filename);
    if (!fin) return false;

    double L_val;
    if (!(fin >> L_val) || L_val <= 0 || L_val > 1000) return false;
    L = (int)L_val;
    if (L_val != L) return false;

    int N;
    if (!(fin >> N) || N < 1 || N > 100) return false;

    flights.resize(N);
    for (auto& f : flights) {
        double x0, y0, x1, y1;
        if (!(fin >> x0 >> y0 >> x1 >> y1)) return false;
        
        double dx = x1 - x0;
        double dy = y1 - y0;
        double len = hypot(dx, dy);
        if (len < EPS) return false;

        f.A = dy/len;
        f.B = -dx/len;
        f.C = -(f.A*x0 + f.B*y0);

        // Precompute 50km buffer
        f.min_x = min(x0, x1) - 50.0;
        f.max_x = max(x0, x1) + 50.0;
        f.min_y = min(y0, y1) - 50.0;
        f.max_y = max(y0, y1) + 50.0;
    }
    return true;
}

int main() {
    int L;
    vector<Flight> flights;
    
    if (!read_input("INPUT", L, flights)) {
        ofstream("OUTPUT") << "ERROR\n";
        return 0;
    }

    double x, y;
    ofstream fout("OUTPUT");
    if (find_uncovered(flights, L, x, y)) {
        fout << fixed << setprecision(3) << x << " " << y << "\n";
    } else {
        fout << "OK\n";
    }
}

