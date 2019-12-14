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

const int N = 133333 + 5;
const int M = 1e6 + 5;
int a[N], p[M], ans[N], block_size;
struct _change
{
    int pla, pre, suc;
} change[N];

struct request
{
    int l, r, t, belong;

    const bool operator<(const request &b) const
    {
        return l / block_size == b.l / block_size ? (r / block_size == b.r / block_size ? t < b.t : r / block_size < b.r / block_size)
                                                  : l / block_size < b.l / block_size;
    }
} query[N];

int n, m;
int query_id = 0, change_id = 0, l = 1, r = 0, t = 0, num = 0;

int main()
{
    io::read(n, m);
    io::readln(a + 1, a + n + 1);

    for (int i = 1; i <= m; i++)
    {
        char c = getchar();
        while (!isalpha(c))
        {
            c = getchar();
        }

        if (c == 'R')
        {
            ++change_id;
            io::read(change[change_id].pla, change[change_id].suc);
            change[change_id].pre = a[change[change_id].pla];
            a[change[change_id].pla] = change[change_id].suc;
        }
        else
        {
            query[++query_id] = (request){io::read<int>(), io::read<int>(), change_id, query_id};
        }
    }

    block_size = ceil(exp((log(n) + log(change_id)) / 3));

    for (int i = change_id; i >= 1; i--)
    {
        a[change[i].pla] = change[i].pre;
    }

    sort(query + 1, query + query_id + 1);

    for (int i = 1; i <= m; i++)
    {
        while (query[i].l < l)
        {
            num += !p[a[--l]]++;
        }

        while (query[i].l > l)
        {
            num -= !--p[a[l++]];
        }

        while (query[i].r > r)
        {
            num += !p[a[++r]]++;
        }

        while (query[i].r < r)
        {
            num -= !--p[a[r--]];
        }

        while (query[i].t < t)
        {
            int pla = change[t].pla;
            if (l <= pla && pla <= r)
            {
                num -= !--p[a[pla]];
            }
            a[pla] = change[t--].pre;
            if (l <= pla && pla <= r)
            {
                num += !p[a[pla]]++;
            }
        };

        while (query[i].t > t)
        {
            int pla = change[++t].pla;
            if (l <= pla && pla <= r)
            {
                num -= !--p[a[pla]];
            }
            a[pla] = change[t].suc;
            if (l <= pla && pla <= r)
            {
                num += !p[a[pla]]++;
            }
        };

        ans[query[i].belong] = num;
    }
    io::writeln(ans + 1, ans + query_id + 1, '\n');
    return 0;
}