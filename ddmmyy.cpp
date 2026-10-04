#include <iostream>

using namespace std;

class date {
private:
    int x, y, m;

public:
    date(int x = 1, int y = 1, int m = 2000) : x(x), y(y), m(m) {}

    bool leap() {
        return (m % 400 == 0) || (m % 4 == 0 && m % 100 != 0);
    }

    bool valid() {
        if (m < 1 || y < 1 || y > 12 || x < 1) return false;
        int n[13] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
        if (y == 2 && leap()) return x <= 29;
        return x <= n[y];
    }

    void input() {
        char c1, c2;
        cin >> x >> c1 >> y >> c2 >> m;
    }

    void output() {
        if (x < 10) cout << "0";
        cout << x << "/";
        if (y < 10) cout << "0";
        cout << y << "/" << m << "\n";
    }
};

int main() {
    date d;
    d.input();
    if (d.valid()) {
        d.output();
    } else {
        cout << "0\n";
    }
    return 0;
}
