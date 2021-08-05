#include <bits/stdc++.h>

const int N = 2100;
using ll = long long;

inline int lb(int x){return x & -x;}

ll c[4][N][N];

void add(ll (*c)[N], int sit1, int sit2, ll value){
    for (int i = sit1; i < N; i += lb(i)){
        for (int j = sit2; j < N; j += lb(j)){
            c[i][j] += value;
        }
    }
}

void add(int x1, int x2, int y1, int y2, ll value){
    int x[2] = {x1, x2 + 1};
    int y[2] = {y1, y2 + 1};
    for (int i = 0; i < 2; ++ i){
        for (int j = 0; j < 2; ++ j){
            ll base = (i ^ j) ? -value : value;
            add(c[0], x[i], y[j], base);
            add(c[1], x[i], y[j], base * x[i]);
            add(c[2], x[i], y[j], base * y[j]);
            add(c[3], x[i], y[j], base * x[i] * y[j]);
        }
    }
}

ll query(ll (*c)[N], int sit1, int sit2){
    ll ret = 0;
    for (int i = sit1; i; i -= lb(i)){
        for (int j = sit2; j; j -= lb(j)){
            ret += c[i][j];
        }
    }
    return ret;
}

ll query_lu(int x, int y){
    ll ans = (x + 1) * (y + 1) * query(c[0], x, y);
    ans -= (y + 1) * query(c[1], x, y);
    ans -= (x + 1) * query(c[2], x, y);
    ans += query(c[3], x, y);
    return ans;
}

ll query(int x1, int x2, int y1, int y2){
    ll ans = query_lu(x2, y2);
    ans -= query_lu(x1 - 1, y2);
    ans -= query_lu(x2, y1 - 1);
    ans += query_lu(x1 - 1, y1 - 1);
    return ans;
}

int main(){
    int n, m;
    scanf("%d%d", &n, &m);
    int type;
    while (scanf("%d", &type) != EOF){
        if (type == 1){
            int x1, y1, x2, y2;
            int value;
            scanf("%d%d%d%d%d", &x1, &y1, &x2, &y2, &value);
            add(x1, x2, y1, y2, value);
        }
        else{
            int x1, y1, x2, y2;
            scanf("%d%d%d%d", &x1, &y1, &x2, &y2);
            printf("%lld\n", query(x1, x2, y1, y2));
        }
    }
    return 0;
}