<TeX>$N$ 应当设为 $2$ 的幂，以免考虑数组溢出问题</TeX>

int search(int rt, int num){
    int l_empty = lb(rt) - c[rt];
    if (rt & 1){
        return rt + (l_empty < num);
    }
    if (l_empty >= num){
        return search(rt - lb(rt) / 2, num);
    }
    return search(rt + lb(rt) / 2, num - l_empty);
}
