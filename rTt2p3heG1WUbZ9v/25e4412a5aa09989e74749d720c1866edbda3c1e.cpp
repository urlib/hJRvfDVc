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

const int B = 20 + 5;
const int N = 5000 + 5;
const int M = 60000 + 5;
const int INF = 2e9;

int head[N], cur[N], dep[N], to[M], nxt[M], wgt[M], E = 0;
int a[N][B], siz[B];

int n, m;

inline void add_edge(int u, int v, int w)
{
    to[E] = v;
    nxt[E] = head[u];
    wgt[E] = w;
    head[u] = E++;
}

inline void init(int l, int r)
{
    E = 0;
    memset(head, -1, sizeof(head));

    for (int i = 1; i <= n; i++)
    {
        add_edge(0, i, 1);
        add_edge(i, 0, 0);
    }

    for (int i = 1; i <= n; i++)
    {
        for (int j = l; j <= r; j++)
        {
            add_edge(i, a[i][j] + n, 1);
            add_edge(a[i][j] + n, i, 0);
        }
    }

    for (int i = 1; i <= m; i++)
    {
        add_edge(i + n, n + m + 1, siz[i]);
        add_edge(n + m + 1, i + n, 0);
    }
}

inline bool bfs(int s, int t)
{
    memset(dep, -1, sizeof(dep));
    dep[s] = 0;

    memcpy(cur, head, (n + m + 2) * sizeof(int));

    queue<int> q;
    q.emplace(s);

    while (!q.empty())
    {
        int u = q.front();
        q.pop();
        for (int i = head[u]; i != -1; i = nxt[i])
        {
            int v = to[i];
            if (dep[v] == -1 && wgt[i])
            {
                dep[v] = dep[u] + 1;
                q.emplace(v);
            }
        }
    }
    return dep[t] != -1;
}

int dfs(int u, int t, int in)
{
    if (!in || u == t)
    {
        return in;
    }

    int out = 0;
    for (int i = cur[u]; i != -1; i = nxt[i])
    {
        cur[u] = i;
        int v = to[i];
        if (dep[v] == dep[u] + 1)
        {
            int f = dfs(v, t, std::min(in, wgt[i]));
            if (f)
            {
                wgt[i] -= f;
                wgt[i ^ 1] += f;
                in -= f;
                out += f;
                if (!in)
                {
                    break;
                }
            }
        }
    }
    return out;
}

inline int dinic(int s, int t)
{
    int ans = 0;
    while (bfs(s, t))
    {
        ans += dfs(s, t, INF);
    }
    return ans;
}

inline bool _check(int l, int r)
{
    init(l, r);
    return dinic(0, m + n + 1) == n;
}

inline bool check(int x)
{
    for (int i = 1; i + x <= m; i++)
    {
        if (_check(i, i + x))
        {
            return 1;
        }
    }
    return 0;
}

int main()
{
    io::read(n, m);
    for (int i = 1; i <= n; i++)
    {
        io::readln(a[i] + 1, a[i] + m + 1);
    }

    io::readln(siz + 1, siz + m + 1);

    int l = 0, r = m - 1, ans = 0;
    while (l <= r)
    {
        int mid = (l + r) >> 1;
        if (check(mid))
        {
            ans = mid;
            r = mid - 1;
        }
        else
        {
            l = mid + 1;
        }
    }
    io::writeln(ans + 1);
}