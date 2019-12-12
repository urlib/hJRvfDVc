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
inline c T max(c T &a, c T &b)
{
    return a > b ? a : b;
}

template <typename T>
inline c T min(c T &a, c T &b)
{
    return a < b ? a : b;
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

unordered_map<int, int> hush, ans;

inline bool in_range(int x, int y)
{
    return x >= 1 && x <= 3 && y >= 1 && y <= 3;
}

const int target = 123804765;
int n;

static int fx, fy, nx, ny;
static int cur, now;

static int a[5][5];
const static int dx[] = {1, -1, 0, 0}, dy[] = {0, 0, 1, -1};

int main()
{
    io::read(n);
    if (n == target)
    {
        io::writeln(0);
        return 0;
    }

    queue<int> q;
    q.push(target);
    q.push(n);

    ans[target] = 1;
    ans[n] = 0;
    hush[target] = 2;
    hush[n] = 1;

    while (!q.empty())
    {
        cur = q.front();
        q.pop();
        now = cur;

        for (int i = 3; i >= 1; i--)
        {
            for (int j = 3; j >= 1; j--)
            {
                a[i][j] = now % 10;
                now /= 10;
                if (a[i][j] == 0)
                {
                    fx = i;
                    fy = j;
                }
            }
        }

        for (int i = 0; i < 4; i++)
        {
            nx = fx + dx[i], ny = fy + dy[i];
            if (!in_range(nx, ny))
            {
                continue;
            }
            now = 0;
            swap(a[fx][fy], a[nx][ny]);
            for (int k = 1; k <= 3; k++)
            {
                for (int j = 1; j <= 3; j++)
                {
                    now = now * 10 + a[k][j];
                }
            }
            if (hush[now] == hush[cur])
            {
                swap(a[fx][fy], a[nx][ny]);
                continue;
            }
            else if (hush[now] + hush[cur] == 3)
            {
                io::writeln(ans[now] + ans[cur]);
                return 0;
            }
            ans[now] = ans[cur] + 1;
            hush[now] = hush[cur];
            q.push(now);
            swap(a[fx][fy], a[nx][ny]);
        }
    }
}