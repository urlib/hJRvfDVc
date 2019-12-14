#include "bits/stdc++.h"
std::mt19937 R;

int main(int argc, char *argv[])
{
    R.seed(time(0));
    for (int i = 1; i < argc; i++)
    {
        int X = 0;
        for (int j = 0; j < strlen(argv[i]); j++)
        {
            X = X * 10 + argv[i][j] - '0';
        }
        for (int j = 0; j < X; j++)
        {
            char c = ' ';
            while (!isalnum(c))
            {
                c = R() % 128;
            }
            putchar(c);
        }
        putchar('\n');
    }
}