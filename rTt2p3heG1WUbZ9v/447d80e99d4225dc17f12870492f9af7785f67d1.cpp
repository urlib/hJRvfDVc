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

struct robot
{
    int x, y;//坐标值
    int direction;//方向 
    int step;
};

queue<robot> q;
bool vis[51][51][4];
bool map[52][52];
int dx[4]={0,1,0,-1},dy[4]={1,0,-1,0};
int main()
{
    int n,m;
    int sx,sy,ex,ey,sd;//起始点和终点坐标 
    char sdirection;//起始方向 
    scanf("%d%d",&n,&m);
    for(int i=1;i<=n;i++)
    {
        for(int j=1;j<=m;j++)
        {
            scanf("%d",&map[i][j]);
            if(map[i][j])
            {
                map[i-1][j-1]=1;
                map[i-1][j]=1;
                map[i][j-1]=1;
            }
        }
    }
    //预处理 
    scanf("%d%d%d%d %c",&sx,&sy,&ex,&ey,&sdirection);
    if(sdirection=='E')sd=0;
    else if(sdirection=='S')sd=1;
    else if(sdirection=='W')sd=2;
    else sd=3;//将方向转化为数字便于判重 
    if(sx>=n || sx<1 || sy>=m || sy<1 || map[sx][sy]){printf("-1");return 0;}
    robot temp;
    temp.x=sx;temp.y=sy;temp.direction=sd;temp.step=0;//从起点开始，将起点状态塞进队列
    vis[sx][sy][sd]=true;//别忘了标记起始点对应的方向 
    q.push(temp);
    while(!q.empty()){
        //之后就是bfs了，枚举5种操作 并判断状态是否已被访问过，注意行走的时候的判定障碍问题 
        temp=q.front();
        q.pop();
        int nx=temp.x,ny=temp.y;
        if(nx==ex && ny==ey){printf("%d",temp.step);return 0;}//有了结果立即输出 
        for(int i=1;i<=3;i++){
            //走1步~走3步
            nx+=dx[temp.direction];ny+=dy[temp.direction];
            //哇！此题巨坑，机器人直径1.6m... 
            if(nx<1 || nx>=n || ny<1 || ny>=m || map[nx][ny])break; //重点，不能走到边界，也不能走到障碍点所影响的点，否则就不能继续走了，跳出循环 
            else if(!vis[nx][ny][temp.direction]){
                vis[nx][ny][temp.direction]=true;
                robot cnew;cnew.x=nx;cnew.y=ny;cnew.direction=temp.direction;
                cnew.step=temp.step+1; 
                q.push(cnew);
                //printf("%d %d - %d %d step:%d\n",temp.x,temp.y,cnew.x,cnew.y,cnew.step);
            }
        }
        //转向
        robot cnew;cnew.x=temp.x;cnew.y=temp.y;cnew.step=temp.step+1;
        cnew.direction=temp.direction-1;
        if(cnew.direction==-1)cnew.direction=3;//如果我从东向西转，那么应该到北 
        if(!vis[cnew.x][cnew.y][cnew.direction]){vis[cnew.x][cnew.y][cnew.direction]=true;q.push(cnew);}
        cnew.direction=temp.direction+1;
        if(cnew.direction==4)cnew.direction=0;//如果我从北向东转，那么应该到东 
        if(!vis[cnew.x][cnew.y][cnew.direction]){vis[cnew.x][cnew.y][cnew.direction]=true;q.push(cnew);}
    }
    printf("-1");//无解 
    return 0;
} 
