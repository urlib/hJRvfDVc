// #pragma GCC optimize("Ofast,no-stack-protector,unroll-loops,fast-math")
// #pragma GCC target("sse,sse2,sse3,ssse3,sse4.1,sse4.2,avx,avx2,popcnt,tune=native")

// #include "bits/stdc++.h"

// #define mem(x) memset((x), 0, sizeof((x)))
// #define il __attribute__((always_inline))

// using namespace std;

// typedef long long ll;
// typedef long double ld;
// typedef unsigned long long ull;

// #if __cplusplus > 201403L
// #define r
// #else
// #define r register
// #endif

// #define c const

// namespace _c
// {
// c double pi = acos(-1.0);
// namespace min
// {
// c int i8 = -128;
// c int i16 = -32768;
// c int i = -2147483647 - 1;
// c ll l = -9223372036854775807LL - 1;
// } // namespace min

// namespace max
// {
// c int i8 = 127;
// c int i16 = 32767;
// c int i = 2147483647;
// c ll l = 9223372036854775807LL;
// } // namespace max
// } // namespace _c

// namespace _f
// {
// template <typename T>
// inline c T gcd(T m, T n)
// {
//     while (n != 0)
//     {
//         T t = m % n;
//         m = n;
//         n = t;
//     }
//     return m;
// }

// template <typename T>
// inline c T abs(c T &a)
// {
//     return a > 0 ? a : -a;
// }

// template <typename T>
// inline T pow(T a, T b)
// {
//     T res = 1;
//     while (b > 0)
//     {
//         if (b & 1)
//         {
//             res = res * a;
//         }
//         a = a * a;
//         b >>= 1;
//     }
//     return res;
// }

// template <typename T>
// inline T pow(T a, T b, c T &m)
// {
//     a %= m;
//     T res = 1;
//     while (b > 0)
//     {
//         if (b & 1)
//         {
//             res = res * a % m;
//         }
//         a = a * a % m;
//         b >>= 1;
//     }
//     return res % m;
// }
// } // namespace _f

// namespace io
// {
// template <typename T>
// inline T read()
// {
//     r T res = 0, neg = 1;
//     char g = getchar();
//     for (; !isdigit(g); g = getchar())
//     {
//         if (g == '-')
//         {
//             neg = -1;
//         }
//     }
//     for (; isdigit(g); g = getchar())
//     {
//         res = res * 10 + g - '0';
//     }
//     return res * neg;
// }
// template <typename T>
// inline void read(T &t)
// {
//     t = read<T>();
// }
// template <typename T>
// inline void readln(c T first, c T last)
// {
//     for (r T it = first; it != last; it++)
//     {
//         read(*it);
//     }
// }

// template <typename T>
// inline void _write(T x)
// {
//     if (x < 0)
//     {
//         putchar('-');
//         x = -x;
//     }
//     if (x > 9)
//     {
//         _write(x / 10);
//     }
//     putchar(x % 10 + '0');
// }
// template <typename T>
// inline void write(c T &x, c char &sep = ' ')
// {
//     _write(x);
//     putchar(sep);
// }
// template <typename T>
// inline void writeln(c T &x)
// {
//     write(x, '\n');
// }
// template <typename T>
// inline void writeln(c T first, c T last, c char &sep = ' ', c char &ends = '\n')
// {
//     for (r T it = first; it != last; it++)
//     {
//         write(*it, sep);
//     }
//     putchar(ends);
// }

// #if __cplusplus >= 201103L
// template <typename T, typename... Args>
// void read(T &x, Args &... args)
// {
//     read(x);
//     read(args...);
// }
// #endif
// } // namespace io
// #undef c
// #undef r

// const int INF = 0x3f3f3f3f;
// const int N = 100 + 5;
// const int M = 100000 + 5;

// int f[N], hp[N], rp[N];
// int R[N][N];

// int find(int x)
// {
//     return x == f[x] ? x : f[x] = find(f[x]);
// }

// inline void uni(int x, int y)
// {
//     x = find(x), y = find(y);
//     if (x != y)
//     {
//         f[x] = y;
//     }
// }

// int head[M], dep[M];

// struct Edge
// {
//     int to, next, flow;
// } edge[M];
// int E = 1;

// inline void add_edge(int u, int v, int flow)
// {
//     edge[++E] = Edge{v, head[u], flow};
//     head[u] = E;
//     edge[++E] = Edge{u, head[v], 0};
//     head[v] = E;
// }

// int n, m, k;
// int s, t;

// inline bool bfs()
// {
//     memset(dep, 0, sizeof(dep));
//     queue<int> q;
//     dep[t] = 0;
//     q.emplace(s);
//     while (!q.empty())
//     {
//         int x = q.front();
//         q.pop();
//         if (x == t)
//         {
//             return 1;
//         }
//         for (int i = head[x]; i; i = edge[i].next)
//         {
//             if (edge[i].flow > 0 && !dep[edge[i].to])
//             {
//                 dep[edge[i].to] = dep[x] + 1;
//                 q.emplace(edge[i].to);
//             }
//         }
//     }

//     return 0;
// }

// int dfs(int x, int in)
// {
//     if (x == t)
//     {
//         return in;
//     }

//     int out = 0;
//     for (int i = head[x]; i; i = edge[i].next)
//     {
//         if (edge[i].flow > 0 && dep[edge[i].to] == dep[x] + 1)
//         {
//             int res = dfs(edge[i].to, std::min(edge[i].flow, in));
//             edge[i].flow -= res;
//             edge[i ^ 1].flow += res;
//             in -= res;
//             out += res;
//         }

//         if (!in)
//         {
//             return out;
//         }
//     }

//     return out;
// }

// int main()
// {
//     io::read(n, m, k);
//     s = 0, t = M - 2;

//     for (int i = 1; i <= n + 2; i++)
//     {
//         f[i] = i;
//     }

//     for (int i = 1; i <= m; i++)
//     {
//         io::read(hp[i], rp[i]);

//         for (int j = 0; j < rp[i]; j++)
//         {
// #define x R[i][j]
//             io::read(x);
//             if (x == 0)
//             {
//                 x = n + 1;
//             }
//             else if (x == -1)
//             {
//                 x = n + 2;
//             }
// #undef x
//             if (j != 0)
//             {
//                 uni(R[i][j - 1], R[i][j]);
//             }
//         }
//     }

//     if (find(n + 1) != find(n + 2))
//     {
//         io::writeln(0);
//         return 0;
//     }

//     int mx = 0;

//     for (int ans = 1;; ans++)
//     {
//         add_edge(s, (ans - 1 + 1) * (n + 1), INF);
//         for (int i = 1; i <= m; i++)
//         {
//             int x = (ans - 1) % rp[i], y = ans % rp[i];
//             x = (R[i][x] == n + 2 ? t : (ans - 1) * (n + 1) + R[i][x]);
//             y = (R[i][y] == n + 2 ? t : ans * (n + 1) + R[i][y]);
//             add_edge(x, y, hp[i]);
//         }

//         while (bfs())
//         {
//             mx += dfs(s, INF);
//         }

//         if (mx >= k)
//         {
//             io::writeln(ans);
//             return 0;
//         }

//         for (int i = 1; i <= n + 1; i++)
//         {
//             add_edge((ans - 1) * (n + 1) + i, ans * (n + 1) + i, INF);
//         }
//     }
// }

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

const int INF = 0x3f3f3f3f;
const int N = 105;
const int M = 1000005;
int n, m, k, E = 1;
int s, t;
int ans, mx;

int f[N], R[N][N], hp[N], rp[N];
int nxt[M], to[M], head[M], flow[M], dep[M];

int find(int x)
{
    return x == f[x] ? x : f[x] = find(f[x]);
}
inline void merge(int x, int y)
{
    x = find(x), y = find(y);
    if (x != y)
    {
        f[x] = y;
    }
}

inline void _add_edge(int u, int v, int w)
{
    to[++E] = v;
    nxt[E] = head[u];
    flow[E] = w;
    head[u] = E;
}
inline void add_edge(int u, int v, int w)
{
    _add_edge(u, v, w);
    _add_edge(v, u, 0);
}

inline bool bfs()
{
    for (int i = 1; i <= ans * (n + 1); i++)
    {
        dep[i] = 0;
    }
    queue<int> q;
    q.emplace(s);
    dep[t] = 0;
    while (!q.empty())
    {
        int x = q.front();
        q.pop();
        if (x == t)
        {
            return 1;
        }
        for (int i = head[x]; i; i = nxt[i])
        {
            if (flow[i] > 0 && !dep[to[i]])
            {
                dep[to[i]] = dep[x] + 1;
                q.emplace(to[i]);
            }
        }
    }
    return 0;
}
int dfs(int x, int in)
{
    if (x == t)
    {
        return in;
    }
    int out = 0;
    for (int i = head[x]; i; i = nxt[i])
    {
        if (flow[i] > 0 && dep[to[i]] == dep[x] + 1)
        {
            int res = dfs(to[i], std::min(flow[i], in));
            in -= res;
            out += res;
            flow[i] -= res;
            flow[i ^ 1] += res;
            if (!in)
            {
                return out;
            }
        }
    }
    return out;
}

int main()
{
    int x, y;
    io::read(n, m, k);
    s = 0, t = M - 2;
    for (int i = 1; i <= n + 2; i++)
    {
        f[i] = i;
    }
    for (int i = 1; i <= m; i++)
    {
        io::read(hp[i], rp[i]);
        for (int j = 0; j < rp[i]; ++j)
        {
            scanf("%d", &R[i][j]);
            if (R[i][j] == 0)
            {
                R[i][j] = n + 1;
            }
            if (R[i][j] == -1)
            {
                R[i][j] = n + 2;
            }
            if (j != 0)
            {
                merge(R[i][j - 1], R[i][j]);
            }
        }
    }
    if (find(n + 1) != find(n + 2))
    {
        io::writeln(0);
        return 0;
    }
    for (ans = 1;; ++ans)
    {
        add_edge(s, (ans - 1) * (n + 1) + n + 1, INF);
        for (int i = 1; i <= m; i++)
        {
            x = (ans - 1) % rp[i], y = ans % rp[i];
            if (R[i][x] == n + 2)
            {
                x = t;
            }
            else
            {
                x = (ans - 1) * (n + 1) + R[i][x];
            }
            if (R[i][y] == n + 2)
            {
                y = t;
            }
            else
            {
                y = ans * (n + 1) + R[i][y];
            }
            add_edge(x, y, hp[i]);
        }
        while (bfs())
        {
            mx += dfs(s, INF);
        }
        if (mx >= k)
        {
            io::writeln(ans);
            return 0;
        }

        for (int i = 1; i <= n + 1; i++)
        {
            add_edge((ans - 1) * (n + 1) + i, ans * (n + 1) + i, INF);
        }
    }
    return 0;
}