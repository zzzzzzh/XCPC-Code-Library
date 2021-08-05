void manacher(char *s, char *t, int *p){
    int n = strlen(s);
    t[0] = '$', t[1] = '#';
    int cnt = 2;
    for (int i = 0; i < n; ++ i){
        t[cnt ++] = s[i];
        t[cnt ++] = '#';
    }
    t[cnt] = '\0';
    for (int i = 1, l = 0, r = -1; i < cnt; ++ i){
        int k = i > r ? 1 : std::min(p[l + r - i], r - i + 1);
        while (t[i + k] == t[i - k]){
            ++ k;
        }
        p[i] = k --;
        if (i + k > r){
            l = i - k, r = i + k;
        }
    }
}