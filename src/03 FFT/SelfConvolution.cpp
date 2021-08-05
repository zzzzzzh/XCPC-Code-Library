<TeX>自卷积模板。例：$f(i)=\sum_{j=0}^{i-1}f(j)f(i-j-1)$</TeX>

const int N = 100010;

int f[N];

void solve(int l, int r){
    if (l == r){
        return;
    }
    int mid = (l + r) / 2;
    solve(l, mid);

    // 若 r-l-1<=mid，那么事实上与 f*g 一样，但是如果r-l>mid，那么此时r-l还没有被计算出来，
    // 可以证明这种情况只会在l=0时发生（假设下标不为负，但是这个也不重要）。
    // 需要稍微修改一下更新策略，对于一个区间 [L,R]，设它被分割成 [L,M] 和 (M,R]，
    // 给 (M,R] 中的元素贡献 [L,M] 和 [L,M] 的卷积，[L,M] 和 [0,L) 及 [0,L) 和 [L,M] 的卷积。
    // 这个更新策略的证明如下：考虑一个位置 x，以及分治的过程，那么 x 左边有若干个块。
    // 每个块内部确实恰好给 x 有 1 的贡献，而块之间也确实有 1 的贡献

    poly p(mid - l + 1);
    for (int i = l; i <= mid; ++ i){
        p[i - l] = f[i];
    }
    if (l + l + 1 <= r){
        poly q = p * p;
        for (int i = mid + 1; i <= r; ++ i){
            int pos = i - l - l;
            if (0 <= pos && pos <= q.len){
                add(f[i], q[pos]);
            }
        }
    }
    int right = std::min(l - 1, r - l - 1);
    if (right >= 0){
        poly q(right);
        for (int i = 0; i <= right; ++ i){
            q[i] = f[i];
        }
        q = p * q;
        for (int i = mid + 1; i <= r; ++ i){
            int pos = i - l;
            if (0 <= pos && pos <= q.len){
                add(f[i], q[pos]);
            }
        }
    }
    solve(mid + 1, r);
}
