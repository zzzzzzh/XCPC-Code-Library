#include <bits/stdc++.h>

const int N = 2000010;

int p[N], d[N];

void gen_p(int n){
#if defined(DEBUG)
    for (int i = 0; i < n; ++ i){
        scanf("%d", &p[i]);
    }
#else
    int seed, a, b, moder;
    scanf("%d%d%d%d", &seed, &a, &b, &moder);
    for (int i = 0; i < n; ++ i){
        p[i] = i;
    }
    for (int i = 1; i < n; ++ i){
        std::swap(p[i], p[seed % (i + 1)]);
        seed = (1ll * a * seed + b) % moder;
    }
#endif
}

void gen_d(int n){
#if defined(DEBUG)
    for (int i = 0; i < n; ++ i){
        scanf("%d", &d[i]);
    }
#else
    int seed, a, b, moder;
    scanf("%d%d%d%d", &seed, &a, &b, &moder);
    for (int i = 0; i < n; ++ i){
        d[i] = seed % 10;
        seed = (1ll * a * seed + b) % moder;
    }
#endif
}

int n;
int e[N][2];
int ans[N], anscnt, inv[N];
bool vis[N];

void dfs(int u){
    int inv = d[u] < p[u] % 10;
    for (int i = 0; i < 2; ++ i){
        int x = i ^ inv;
        int v = e[u][x];
        if (v != -1){
            dfs(v);
            continue;
        }
        if (x){
            int cur = u + 1;
            while (cur < n && !vis[cur]){
                ans[anscnt ++] = cur ++;
            }
            ans[anscnt ++] = cur;
        }
        else{
            int cur = u;
            int prev = anscnt;
            while (cur > 0 && !vis[cur - 1]){
                ans[anscnt ++] = cur --;
            }
            ans[anscnt ++] = cur;
            std::reverse(ans + prev, ans + anscnt);
        }
    }
}

void solve(){
    anscnt = 0;
    scanf("%d", &n);
    for (int i = 0; i < n; ++ i){
        e[i][0] = e[i][1] = -1;
        vis[i] = false;
    }
    gen_p(n);
    gen_d(n);
    std::vector <int> stack;
    int root = -1;
    for (int i = 0; i < n; ++ i){
        if (d[i] == p[i] % 10){
            continue;
        }
        vis[i] = true;
        while (!stack.empty() && p[stack.back()] > p[i]){
            stack.pop_back();
        }
        if (stack.empty()){
            e[i][0] = root;
            root = i;
        }
        else{
            e[i][0] = e[stack.back()][1];
            e[stack.back()][1] = i;
        }
        stack.emplace_back(i);
    }
    if (root == -1){
        for (int i = 0; i <= n; ++ i){
            ans[anscnt ++] = i;
        }
    }
    else{
        dfs(root);
    }
    int ret = 0;
    for (int i = 0; i < anscnt; ++ i){
        inv[ans[i]] = i;
    }
#if defined(DEBUG)
    for (int i = 0; i < anscnt; ++ i){
        printf("%d%c", ans[i], " \n"[i == anscnt - 1]);
    }
#endif
    for (int i = anscnt - 1; i >= 0; -- i){
        ret = (1ll * ret * 10'000'019 + inv[i]) % 1'000'000'007;
    }
    printf("%d\n", ret);
}

int main(){
    int test;
    scanf("%d", &test);
    while (test --){
        solve();
    }
    return 0;
}