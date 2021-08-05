#include <bits/stdc++.h>

inline int lb(int x){return x & -x;}

int main(){
    int n = 5, m = 3;
    for (int i = (1 << m) - 1; i < (1 << n); ){
        printf("%d %u\n", i, __builtin_popcount(i));
        int x = lb(i);
        i += x;
        int y = lb(i) / x;
        i += y / 2 - 1;
    }
    return 0;
}
