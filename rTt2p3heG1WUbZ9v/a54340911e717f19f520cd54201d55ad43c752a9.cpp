#pragma GCC optimize("Ofast,no-stack-protector,unroll-loops,fast-math")
#pragma GCC target("sse,sse2,sse3,ssse3,sse4.1,sse4.2,avx,avx2,popcnt,tune=native")

#include "bits/stdc++.h"

#define mem(x) memset((x), 0, sizeof((x)))
#define il __attribute__((always_inline))

using namespace std;

typedef long long ll;
typedef long double ld;
typedef unsigned long long ull;

#if __cplusplus > 201403L
#define r
#else
#define r register
#endif

#define c const

namespace _c
{
c double pi = acos(-1.0);
namespace min
{
c int i8 = -128;
c int i16 = -32768;
c int i = -2147483647 - 1;
c ll l = -9223372036854775807LL - 1;
} // namespace min

namespace max
{
c int i8 = 127;
c int i16 = 32767;
c int i = 2147483647;
c ll l = 9223372036854775807LL;
} // namespace max
} // namespace _c

namespace _f
{
template <typename T>
inline c T gcd(T m, T n)
{
    while (n != 0)
    {
        T t = m % n;
        m = n;
        n = t;
    }
    return m;
}

template <typename T>
inline c T abs(c T &a)
{
    return a > 0 ? a : -a;
}

template <typename T>
inline T pow(T a, T b)
{
    T res = 1;
    while (b > 0)
    {
        if (b & 1)
        {
            res = res * a;
        }
        a = a * a;
        b >>= 1;
    }
    return res;
}

template <typename T>
inline T pow(T a, T b, c T &m)
{
    a %= m;
    T res = 1;
    while (b > 0)
    {
        if (b & 1)
        {
            res = res * a % m;
        }
        a = a * a % m;
        b >>= 1;
    }
    return res % m;
}
} // namespace _f

namespace io
{
template <typename T>
inline T read()
{
    r T res = 0, neg = 1;
    char g = getchar();
    for (; !isdigit(g); g = getchar())
    {
        if (g == '-')
        {
            neg = -1;
        }
    }
    for (; isdigit(g); g = getchar())
    {
        res = res * 10 + g - '0';
    }
    return res * neg;
}
template <typename T>
inline void read(T &t)
{
    t = read<T>();
}
template <typename T>
inline void readln(c T first, c T last)
{
    for (r T it = first; it != last; it++)
    {
        read(*it);
    }
}

template <typename T>
inline void _write(T x)
{
    if (x < 0)
    {
        putchar('-');
        x = -x;
    }
    if (x > 9)
    {
        _write(x / 10);
    }
    putchar(x % 10 + '0');
}
template <typename T>
inline void write(c T &x, c char &sep = ' ')
{
    _write(x);
    putchar(sep);
}
template <typename T>
inline void writeln(c T &x)
{
    write(x, '\n');
}
template <typename T>
inline void writeln(c T first, c T last, c char &sep = ' ', c char &ends = '\n')
{
    for (r T it = first; it != last; it++)
    {
        write(*it, sep);
    }
    putchar(ends);
}

#if __cplusplus >= 201103L
template <typename T, typename... Args>
void read(T &x, Args &... args)
{
    read(x);
    read(args...);
}
#endif
} // namespace io
#undef c
#undef r

unordered_map<char, int> f;
int dx[] = {0, 1, 0, -1};
int dy[] = {1, 0, -1, 0};
int g[55][55];
int cnt, n, m, x_0, y_0, x_1, y_1, ans = -1;
struct cur
{
    int x, y, fx;
    const bool operator<(const cur &b) const
    {
        return x == b.x ? (y == b.y ? fx < b.fx : y < b.y) : x < b.x;
    }
};
cur init;
map<cur, int> vis;
bool check(int x, int y)
{
    int tx = x, ty = y;
    for (int i = 0; i < 4; i++)
    {
        tx += dx[i], ty += dy[i];
        if (tx <= 0 || tx > n || ty <= 0 || ty > m)
            return 0;
        if (tx > 0 && tx <= n && ty > 0 && ty <= m && g[tx][ty])
            return 0;
    }
    return 1;
}
inline void bfs()
{
    queue<cur> q;
    q.push(init);
    vis[init] = 1;
    while (!q.empty())
    {
        cur now = q.front();
        q.pop();
        int tx = now.x, ty = now.y;
        for (int i = 1; i <= 3; i++)
        {
            tx += dx[now.fx];
            ty += dy[now.fx];
            cur _t = now;
            _t.x = tx, _t.y = ty;
            if (!check(tx, ty))
            {
                break;
            }
            else if (!vis[_t])
            {
                vis[_t] = vis[now] + 1;
                if (_t.x == x_1 && _t.y == y_1)
                {
                    ans = vis[_t] - 1;
                    return;
                }
                q.push(_t);
            }
        }
        for (int i = -1; i <= 1; i++)
        {
            if (!i)
            {
                continue;
            }
            cur _t = now;
            _t.fx = (_t.fx + i + 4) % 4;
            if (!vis[_t])
            {
                q.push(_t);
                vis[_t] = vis[now] + 1;
            }
        }
    }
}
int main()
{
    io::read(n, m);
    f['E'] = 0;
    f['S'] = 1;
    f['W'] = 2;
    f['N'] = 3;
    for (int x = 1; x <= n; x++)
    {
        io::readln(g[x] + 1, g[x] + m + 1);
    }
    char fx;
    cin >> x_0 >> y_0 >> x_1 >> y_1 >> fx;
    init.x = x_0;
    init.y = y_0;
    init.fx = f[fx];
    if (x_0 == x_1 && y_0 == y_1)
    {
        io::writeln(0);
    }
    else if (!check(x_0, y_0))
    {
        io::writeln(-1);
    }
    else
    {
        bfs();
        io::writeln(ans);
    }
    return 0;
}