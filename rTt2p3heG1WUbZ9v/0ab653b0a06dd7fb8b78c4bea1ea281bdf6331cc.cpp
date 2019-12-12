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

const int N = 105;
int n, K, sum[N];
ll dp[N][N][N];
struct node
{
    int v, w, f;
} a[N];
inline bool cmpv(const node &a, const node &b)
{
    return a.v < b.v;
}
inline bool cmpw(const node &a, const node &b)
{
    return a.w < b.w;
}
inline int Sum(int l, int r)
{
    return sum[r] - sum[l - 1];
}
int main()
{
    io::read(n, K);
    for (int i = 1; i <= n; i++)
    {
        io::read(a[i].v);
    }
    for (int i = 1; i <= n; i++)
    {
        io::read(a[i].w);
    }
    for (int i = 1; i <= n; i++)
    {
        io::read(a[i].f);
    }
    sort(a + 1, a + n + 1, cmpw);
    for (int i = 1; i <= n; i++)
    {
        a[i].w = i;
    }
    sort(a + 1, a + n + 1, cmpv);
    for (int i = 1; i <= n; i++)
    {
        sum[i] = sum[i - 1] + a[i].f;
    }
    memset(dp, 0x3f, sizeof(dp));
    for (int k = 1; k <= n; k++)
    {
        for (int i = 1; i <= n + 1; i++)
        {
            dp[i][i - 1][k] = 0;
        }
    }
    for (int i = 1; i <= n; i++)
    {
        for (int l = 1, r = l + i - 1; r <= n; l++, r++)
        {
            for (int k = 1; k <= n; k++)
            {
                for (int x = l; x <= r; x++)
                {
                    dp[l][r][k] = std::min(dp[l][r][k], dp[l][x - 1][k] + dp[x + 1][r][k] + Sum(l, r) + K);
                    int v = a[x].w;
                    if (v >= k)
                    {
                        dp[l][r][k] = std::min(dp[l][r][k], dp[l][x - 1][v] + dp[x + 1][r][v] + Sum(l, r));
                    }
                }
            }
        }
    }
    io::writeln(dp[1][n][1]);
}