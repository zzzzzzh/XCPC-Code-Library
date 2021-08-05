<TeX>
模式串长度为 $n$，第 $i$ 个位置可以匹配若干个字符，求长度为 $m$ 的文本串所有可以被匹配的子串。时间复杂度 $\mathcal{O}(nm/32)$，空间复杂度 $\mathcal{O}(n\Sigma/32)$
</TeX>
#include <bits/stdc++.h>

const int max_N = 1024;

const int max_L = (int) 5e6 + 21;

std::bitset<max_N> a[10], f;

int n, m;

char s[max_L];

void print(char *s, int n) {
    char c = s[n];
    s[n] = '\0', puts(s), s[n] = c;
}

int main() {
    while (~scanf("%d", &n)) {
        for (int i = 0; i < 10; ++i) {
            a[i].reset();
        }
        for (int i = 1, k, x; i <= n; ++i) {
            scanf("%d", &k);
            while (k--) {
                scanf("%d", &x);
                a[x].set(i);
            }
        }
        scanf("%s", s);
        m = strlen(s);
        f.reset();
        f.set(0);
        for (int i = 0; i < m; ++i) {
            int j = s[i] - '0';
            (f <<= 1) &= a[j];
            f.set(0);
            if (f.test(n)) {
                print(s + i - n + 1, n);
            }
        }
    }
    return 0;
}
