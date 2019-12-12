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

char s[105];
int find(int l, int r)
{
    int pos = -1, count = 0;
    for (int i = l; i < r; i++)
    {
        if (s[i] == '(')
            count++;
        else if (s[i] == ')')
            count--;
        else if (count > 0)
            continue;
        else if (s[i] == '+' || s[i] == '-')
            pos = i;
        else if (s[i] == '*' || s[i] == '/')
        {
            if (pos == -1 || s[pos] == '^')
                pos = i;
        }
        else if (s[i] == '^')
        {
            if (pos == -1)
                pos = i;
        }
    }
    return pos;
}

double pp(int l, int r)
{
    double a = 0, b = 0;
    int oo = l;
    while (s[oo] != '.' && oo <= r)
    {
        a *= 10;
        a += s[oo] - 48;
        oo++;
    }
    for (int i = r; i >= oo + 1; i--)
    {
        b += s[i] - 48;
        b /= 10;
    }
    return a + b;
}

double calc(double e, double f, char c)
{
    switch (c)
    {
    case '+':
    {
        return e + f;
    }
    case '-':
    {
        return e - f;
    }
    case '*':
    {
        return e * f;
    }
    case '/':
    {
        return e / f;
    }
    case '^':
    {
        return pow(e, f);
    }
    }
}

double solve(int l, int r)
{
    int pos = find(l, r);
    if (pos != -1)
    {
        double left = solve(l, pos - 1);
        double right = solve(pos + 1, r);
        return calc(left, right, s[pos]);
    }
    else
    {
        if (s[l] != '(')
        {
            return pp(l, r);
        }
        else
            return solve(l + 1, r - 1);
    }
}

int main()
{
    cin >> s;
    cout << fixed << setprecision(2) << solve(0, strlen(s) - 1);
    return 0;
}