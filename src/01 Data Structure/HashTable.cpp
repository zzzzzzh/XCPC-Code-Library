struct HashT {
    std::pair<int, pii> data[23333];
    int head[65536], vis[65536], time, nxt[23333], tot;
    inline void clear() { // 必须先clear再使用
        tot = 0, ++time;
    }
    inline pii &operator[](int s) {
        int p = s & 65535;
        if (vis[p] != time) {
            vis[p] = time;
            head[p] = -1;
        }
        int ret = -1;
        for (int i = head[p]; ~i; i = nxt[i])
            if (data[i].first == s) {
                ret = i;
                break;
            }
        if (ret == -1) {
            data[tot] = {s, {INT_MAX, 0}};
            nxt[tot] = head[p];
            ret = head[p] = tot++;
        }
        return data[ret].second;
    }
    inline std::pair<int, pii> *begin() {
        return data;
    }
    inline std::pair<int, pii> *end() {
        return data + tot;
    }
};