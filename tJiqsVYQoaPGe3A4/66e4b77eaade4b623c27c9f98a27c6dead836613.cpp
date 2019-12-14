#include <bits/stdc++.h>
using namespace std;
struct line
{
    int l, r, num;
} q[50010];
int n, m, k, a[50010], ans[50010], l = 1, r, sum, s[50010], size, id[50010];

long long l1[50010], r1[50010];
bool cmp(line x, line y)
{
    if (id[x.r] == id[y.r])
        return x.l < y.l;
    else
        return x.r < y.r;
}
int main()
{
    scanf("%d%d", &n, &m);
    size = n / sqrt(m * 2 / 3);
    for (int i = 1; i <= n; i++)
        scanf("%d", &a[i]), id[i] = (i - 1) / size + 1;
    for (int i = 1; i <= m; i++)
        scanf("%d%d", &q[i].l, &q[i].r), q[i].num = i, l1[i] = q[i].l, r1[i] = q[i].r;
    sort(q + 1, q + m + 1, cmp);
    for (int i = 1; i <= m; i++)
    {
        while (l < q[i].l)
            sum -= s[a[l]] * (s[a[l]] - 1) / 2, s[a[l]]--, sum += s[a[l]] * (s[a[l]] - 1) / 2, l++;
        while (l > q[i].l)
            l--, sum -= s[a[l]] * (s[a[l]] - 1) / 2, s[a[l]]++, sum += s[a[l]] * (s[a[l]] - 1) / 2;
        while (r < q[i].r)
            r++, sum -= s[a[r]] * (s[a[r]] - 1) / 2, s[a[r]]++, sum += s[a[r]] * (s[a[r]] - 1) / 2;
        while (r > q[i].r)
            sum -= s[a[r]] * (s[a[r]] - 1) / 2, s[a[r]]--, sum += s[a[r]] * (s[a[r]] - 1) / 2, r--;
        ans[q[i].num] = sum;
    }
    for (int i = 1, x; i <= m; i++)
    {
        x = __gcd((long long)ans[i], (long long)(r1[i] - l1[i] + 1) * (r1[i] - l1[i]) / 2);

        if (l1[i] == r1[i])
            printf("0/1\n");
        else
            printf("%d/%lld\n", ans[i] / x, (long long)((r1[i] - l1[i] + 1) * (r1[i] - l1[i]) / 2) / x);
    }
    return 0;
}