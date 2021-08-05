// Rikka with Sequence
#include <bits/stdc++.h>

template <typename T>
inline void read(T& x) {
  int c;
  while (!isdigit(c = getchar())) {
  }
  x = c - '0';
  while (isdigit(c = getchar())) {
    x = x * 10 + c - '0';
  }
}

using LL = long long;

using PII = std::pair<int, int>;

const int max_N = (int) 2e5 + 21;
const int max_M = max_N * 12;

struct node {
  LL sum;
  int cnt, ls, rs;
} tree[max_M];

inline int get_val(int x) {
  int ls = tree[x].ls;
  int rs = tree[x].rs;
  return tree[x].sum - tree[ls].sum - tree[rs].sum;
}

inline void update(int x, int val) {
  int ls = tree[x].ls;
  int rs = tree[x].rs;
  tree[x].sum = tree[ls].sum + val + tree[rs].sum;
  tree[x].cnt = tree[ls].cnt + 1 + tree[rs].cnt;
}

int tot, n, m, A1[max_N], A[max_N], cnt, rt1, rt2;

int build(const int* a, int l, int r) {
  if (l > r) {
    return 0;
  }
  int ret = ++tot;
  int mid = (l + r) >> 1;
  tree[ret].ls = build(a, l, mid - 1);
  tree[ret].rs = build(a, mid + 1, r);
  update(ret, a[mid]);
  return ret;
}

int merge(int x, int y) {
  if (!x || !y) {
    return x + y;
  }
  int ret = ++tot;
  tree[ret].sum = tree[x].sum + tree[y].sum;
  tree[ret].cnt = tree[x].cnt + tree[y].cnt;
  if (1LL * tree[x].cnt * RAND_MAX < 1LL * rand() * tree[ret].cnt) {
    tree[ret].ls = tree[x].ls;
    tree[ret].rs = merge(tree[x].rs, y);
  } else {
    tree[ret].ls = merge(x, tree[y].ls);
    tree[ret].rs = tree[y].rs;
  }
  return ret;
}

PII split(int x, int k) {
  assert(0 <= k && k <= tree[x].cnt);
  if (!k) {
    return {0, x};
  }
  if (k == tree[x].cnt) {
    return {x, 0};
  }
  PII ret;
  int y = ++tot;
  int rk = tree[x].cnt - tree[tree[x].rs].cnt;
  if (k >= rk) {
    ret = split(tree[x].rs, k - rk);
    tree[y].ls = tree[x].ls;
    tree[y].rs = ret.first;
    ret.first = y;
  } else {
    ret = split(tree[x].ls, k);
    tree[y].ls = ret.second;
    tree[y].rs = tree[x].rs;
    ret.second = y;
  }
  update(y, get_val(x));
  return ret;
}

int get_range(int rt, int l, int r) {
  auto tmp1 = split(rt, l - 1);
  auto tmp2 = split(tmp1.second, r - l + 1);
  return tmp2.first;
}

LL query(int l, int r) {
  return tree[get_range(rt2, l, r)].sum;
}

void modify(int l, int r, int k) {
  int base = get_range(rt2, l - k, l - 1);
  while (tree[base].cnt < r - l + 1) {
    base = merge(base, base);
  }
  base = get_range(base, 1, r - l + 1);
  auto tmp1 = split(rt2, l - 1);
  auto tmp2 = split(tmp1.second, r - l + 1);
  rt2 = merge(tmp1.first, merge(base, tmp2.second));
}

void modify(int l, int r) {
  int base = get_range(rt1, l, r);
  auto tmp1 = split(rt2, l - 1);
  auto tmp2 = split(tmp1.second, r - l + 1);
  rt2 = merge(tmp1.first, merge(base, tmp2.second));
}

void display(int x) {
  if (!x) {
    return;
  }
  display(tree[x].ls);
  A[++cnt] = get_val(x);
  display(tree[x].rs);
}

void rebuild() {
  cnt = 0;
  display(rt2);
  tot = n;
  rt2 = build(A, 1, n);
}

int main() {
  read(n);
  read(m);
  for (int i = 1; i <= n; ++i) {
    read(A1[i]);
  }
  rt1 = rt2 = build(A1, 1, n);
  assert(tot == n);
  for (int i = 1, opt, l, r, k; i <= m; ++i) {
    read(opt);
    read(l);
    read(r);
    if (opt == 1) {
      printf("%lld\n", query(l, r));
    } else if (opt == 2) {
      read(k);
      modify(l, r, k);
    } else {
      modify(l, r);
    }
    if (max_M - tot < 2000) {
      rebuild();
    }
  }
}