void ex_kmp(char *s, int *zs){
    int n = strlen(s);
    memset(zs, 0, sizeof(*zs) * n);
    for (int i = 1, l = 0, r = 0; i < n; ++ i){
        if (i <= r) zs[i] = std::min(zs[i - l], r - i + 1);
        while (i + zs[i] < n && s[i + zs[i]] == s[zs[i]]) ++ zs[i];
        if (i + zs[i] - 1 > r) l = i, r = i + zs[i] - 1;
    }
}
