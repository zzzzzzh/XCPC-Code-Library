struct KMP{
    #define MAXM (1000010)
    int m;
    char p[MAXM];
    int fail[MAXM];

    void makeFail(char *p){
        memset(this -> p, 0, sizeof(this -> p));

        m = strlen(p);
        for(int i = 0; i < m; ++ i){
            this -> p[i] = p[i];
        }

        fail[0] = -1;
        for(int i = 1, j = -1; i <= m; ++ i){
            while(j >= 0 && p[j] != p[i - 1]) j = fail[j];
            fail[i] = ++ j;
        }
    }

    ll searchForm(char *t){
        int n = strlen(t);
        ll ret = 0;
        for(int i = 0, j = 0; i < n; ++ i){
            while(j >= 0 && p[j] != t[i]) j = fail[j];
            if(++ j == m){
            	ret += f[i];
                j = fail[j];
            }
        }
        return ret;
    }
    #undef MAXM
}kmp;