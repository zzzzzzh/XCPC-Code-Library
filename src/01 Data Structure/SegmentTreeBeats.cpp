<TeX>
支持操作：
\begin{enumerate}
\item 区间取 min
\item 区间求 max
\item 区间求和
\end{enumerate}
</TeX>
#include <bits/stdc++.h>

using ll = long long;

struct Seg{
#define ls(x) ((x) << 1)
#define rs(x) ((x) << 1 | 1)

    static int MAX;
    const static int SIZE = 1 << 20;
    const static ll INF = 0x3f3f3f3f3f3f3f3f;

    ll max[SIZE << 1], smax[SIZE << 1], cnt[SIZE << 1];
    ll seg[SIZE << 1], lazy[SIZE << 1];

    void init(int MAX_VALUE){
        MAX = MAX_VALUE;
        memset(max, 0, sizeof(max[0]) * (MAX * 2));
        // 只有一种值时，smax应小于题目中出现的最小值，否则会导致到了叶子还在递归，因而RE
        memset(smax, -1, sizeof(smax[0]) * (MAX * 2));
        memset(seg, 0, sizeof(seg[0]) * (MAX * 2));
        memset(lazy, 0x3f, sizeof(lazy[0]) * (MAX * 2));
        cnt[1] = MAX;
        for (int i = 2; i < MAX << 1; ++ i){
            cnt[i] = cnt[i >> 1] >> 1;
        }
    }

    void update(int sit, ll value){
        if (value >= max[sit]) return;
        seg[sit] -= (max[sit] - value) * cnt[sit];
        max[sit] = value;
        lazy[sit] = std::min(lazy[sit], value);
    }

    void push(int sit){
        if (lazy[sit] == INF){
            return;
        }
        update(ls(sit), lazy[sit]);
        update(rs(sit), lazy[sit]);
        lazy[sit] = INF;
    }

    void pull(int sit){
        int lch = ls(sit), rch = rs(sit);
        if (max[lch] < max[rch]){
            std::swap(lch, rch);
        }
        max[sit] = max[lch];
        cnt[sit] = cnt[lch] + (max[lch] == max[rch] ? cnt[rch] : 0);
        smax[sit] = max[lch] == max[rch] ? std::max(smax[lch], smax[rch]) : std::max(smax[lch], max[rch]);
        seg[sit] = seg[lch] + seg[rch];
    }

    void add(int sit, int l, int r, int ql, int qr, ll value){
        if (ql > r || qr < l){
            return;
        }
        if (ql <= l && qr >= r && value > smax[sit]){
            update(sit, value);
            return;
        }
        push(sit);
        int mid = (l + r) / 2;
        add(ls(sit), l, mid, ql, qr, value);
        add(rs(sit), mid + 1, r, ql, qr, value);
        pull(sit);
    }

    void add(int l, int r, ll value){
        add(1, 0, MAX - 1, l, r, value);
    }

    ll query1(int sit, int l, int r, int ql, int qr){
        if (ql > r || qr < l){
            return 0;
        }
        if (ql <= l && qr >= r){
            return seg[sit];
        }
        push(sit);
        int mid = (l + r) / 2;
        return query1(ls(sit), l, mid, ql, qr) + query1(rs(sit), mid + 1, r, ql, qr);
    }

    ll query1(int ql, int qr){
        return query1(1, 0, MAX - 1, ql, qr);
    }

    ll query2(int sit, int l, int r, int ql, int qr){
        if (ql > r || qr < l){
            return 0;
        }
        if (ql <= l && qr >= r){
            return max[sit];
        }
        push(sit);
        int mid = (l + r) / 2;
        return std::max(query2(ls(sit), l, mid, ql, qr), query2(rs(sit), mid + 1, r, ql, qr));
    }

    ll query2(int ql, int qr){
        return query2(1, 0, MAX - 1, ql, qr);
    }
};

int Seg::MAX = 0;
