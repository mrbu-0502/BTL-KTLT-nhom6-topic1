#include <iostream>
#include <string>

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
        string a;
        while (true) {
            cin >> a;
            x = 0; y = 0; m = 0;
            int c = 0, n = 1;
            for (int b = 0; b < a.size(); b++) {
                if (a[b] == '/') {
                    c++;
                } else if (a[b] >= '0' && a[b] <= '9') {
                    if (c == 0) x = x * 10 + (a[b] - '0');
                    if (c == 1) y = y * 10 + (a[b] - '0');
                    if (c == 2) m = m * 10 + (a[b] - '0');
                } else {
                    n = 0;
                }
            }
            if (c == 2 && n == 1) break;
            cout << "sai dinh dang, moi ban nhap lai code\n";
        }
    }

    void output() {
        if (x < 10) cout << "0";
        cout << x << "/";
        if (y < 10) cout << "0";
        cout << y << "/" << m << "\n";
    }
};

int main() {
    date c;
    c.input();
    if (c.valid()) {
        c.output();
    } else {
        cout << "0\n";
    }
    return 0;
}
