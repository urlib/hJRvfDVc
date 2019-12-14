#include <cstdio>
#include <string>
#include <cmath>
using namespace std;
const int maxn = 200005, maxm = 505;
int n, m, Q, num[maxn], lnk[maxn], sum[maxn], outt[maxn]; //sum表示它要弹几次才能出它所在的这个块，outt表示它弹出这个块后到哪个点
struct dyt
{
    int l, r;
} a[maxn];
inline int read()
{
    int x = 0;
    char ch = getchar();
    while (ch < '0' || ch > '9')
        ch = getchar();
    while (ch >= '0' && ch <= '9')
        x = x * 10 + ch - 48, ch = getchar();
    return x;
}
void doit(int l, int r)
{ //构造题解中说的两个量
    for (int i = r; i >= l; i--)
        if (i + num[i] > a[lnk[i]].r)
            sum[i] = 1, outt[i] = i + num[i];
        else
            sum[i] = sum[i + num[i]] + 1, outt[i] = outt[i + num[i]];
}
int main()
{
    n = read();
    m = sqrt(n);
    if (m * m < n)
        m++;
    int s = 0;
    for (int i = 1; i <= n; i++)
        num[i] = read();
    for (int i = 1; i <= n; i += m)
        a[++s].l = i, a[s].r = i + m - 1;
    if (s < m)
        a[++s].l = s * m + 1, a[s].r = n;
    s = 1;
    for (int i = 1; i <= n; i++)
    {
        if (i > a[s].r)
            s++;
        lnk[i] = s;
    }
    doit(1, n);
    Q = read();
    while (Q--)
    {
        int x = read(), y = read() + 1;
        if (x == 1)
        {
            int ans = sum[y], x = outt[y];
            for (int i = lnk[y]; i <= m && x <= n; i++)
                ans += sum[x], x = outt[x];
            printf("%d\n", ans);
        }
        else
        {
            int z = read();
            num[y] = z;
            doit(a[lnk[y]].l, a[lnk[y]].r);
        }
    }
    return 0;
}