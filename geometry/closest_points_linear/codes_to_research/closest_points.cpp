#include <bits/stdc++.h>

using namespace std;

struct Point {
    long long x, y;

    Point(long long x, long long y) : x(x), y(y) {}
};

long long distanceSquare(Point a, Point b) {
    return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y);
}

double findClosestPointsDistance(vector<Point> points) {
    assert(points.size() > 1);
    mt19937 rnd(1234);
    shuffle(points.begin(), points.end(), rnd);
    long long anssq = distanceSquare(points[0], points[1]);
    long long ans = sqrt(anssq);
    if (anssq == 0) {
        return 0;
    }
    unordered_map<long long, vector<Point>> grid;

    auto trueDivision = [&](long long a, long long b) -> long long { // avoiding troubles with negative numbers
        long long reminder = a % b;
        if (reminder < 0) {
            reminder += b;
        }
        return (a - reminder) / b;
    };
    auto getPointHash = [&](Point p) -> long long {
        return trueDivision(p.x, ans) * 10000007 + trueDivision(p.y, ans);
    };
    grid[getPointHash(points[0])].push_back(points[0]);
    grid[getPointHash(points[1])].push_back(points[1]);

    for (size_t i = 2; i < points.size(); i++) {
        long long newanssq = anssq;
        
        for (int dx = -1; dx <= 1; dx++) {
            for (int dy = -1; dy <= 1; dy++) {
                for (Point q : grid[getPointHash(Point(points[i].x + dx * ans, points[i].y + dy * ans))]) {
                    newanssq = min(newanssq, distanceSquare(points[i], q));
                }
            }
        }

        if (newanssq < anssq) {
            anssq = newanssq;
            ans = sqrt(newanssq);
            if (anssq == 0) {
                return 0;
            }
            grid.clear();
            for (size_t j = 0; j < i; j++) {
                grid[getPointHash(points[j])].push_back(points[j]);
            }
        }
        grid[getPointHash(points[i])].push_back(points[i]);
    }
    return sqrt(anssq);
}

int main() {
    int n;
    cin >> n;
    vector<Point> points;
    for (int i = 0; i < n; i++) {
        int x, y;
        cin >> x >> y;
        points.push_back(Point(x, y));
    }
    cout << findClosestPointsDistance(points) << endl;
}
