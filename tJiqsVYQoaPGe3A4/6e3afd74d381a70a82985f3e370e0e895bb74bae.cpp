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

const int N = 50000 + 5;

int n, m, k;
int a[N], ans[N];

int l = 1, r, sum, s[N], size, id[N];

ll _l[N], _r[N];

struct line
{
    int l, r, num;
    const bool operator<(const line &b) const
    {
        return id[r] == id[b.r] ? l < b.l : r < b.r;
    }
} q[N];

int main()
{
    io::read(n, m);
    size = n / sqrt(m * 2 / 3);

    for (int i = 1; i <= n; i++)
    {
        io::read(a[i]);
        id[i] = (i - 1) / size + 1;
    }

    for (int i = 1; i <= m; i++)
    {
        io::read(q[i].l, q[i].r);
        q[i].num = i;
        _l[i] = q[i].l;
        _r[i] = q[i].r;
    }

    sort(q + 1, q + m + 1);

    for (int i = 1; i <= m; i++)
    {
        while (l < q[i].l)
        {
            sum -= s[a[l]] * (s[a[l]] - 1) / 2;
            s[a[l]]--;
            sum += s[a[l]] * (s[a[l]] - 1) / 2;
            l++;
        }

        while (l > q[i].l)
        {
            l--;
            sum -= s[a[l]] * (s[a[l]] - 1) / 2;
            s[a[l]]++;
            sum += s[a[l]] * (s[a[l]] - 1) / 2;
        }

        while (r < q[i].r)
        {
            r++;
            sum -= s[a[r]] * (s[a[r]] - 1) / 2;
            s[a[r]]++;
            sum += s[a[r]] * (s[a[r]] - 1) / 2;
        }

        while (r > q[i].r)
        {
            sum -= s[a[r]] * (s[a[r]] - 1) / 2;
            s[a[r]]--;
            sum += s[a[r]] * (s[a[r]] - 1) / 2;
            r--;
        }

        ans[q[i].num] = sum;
    }
    for (int i = 1; i <= m; i++)
    {
        int _gcd = _f::gcd((ll)ans[i], (ll)(_r[i] - _l[i] + 1) * (_r[i] - _l[i]) / 2);

        if (_l[i] == _r[i])
        {
            puts("0/1");
        }
        else
        {
            io::write(ans[i] / _gcd, '/');
            io::writeln((ll)((_r[i] - _l[i] + 1) * (_r[i] - _l[i]) / 2) / _gcd);
        }
    }
    return 0;
}