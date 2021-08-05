#include <bits/stdc++.h>

const int N = 510;
const int K = 1000010;

int boy_match[N], girl_match[N];
int boy[N][N], girl[N][N];
int rk[N];
char input[K];

/*
 * require: ** boy and girl are different **
 *          boy[i][j] is the j-th lovest girl of boy i
 *          girl[i][j] is the i-th girl's loveness of boy j
 * return: boy_match[i] is the matching girl of boy i
 */
void Gale_Shapley(int n){
    memset(boy_match, 0, sizeof(boy_match));
    memset(girl_match, 0, sizeof(girl_match));
    for (int i = 1; i <= n; ++ i){
        rk[i] = 1;
    }
    for ( ; ; ){
        bool flag = false;
        for (int i = 1; i <= n; ++ i){
            if (!boy_match[i]){
                int x = boy[i][rk[i] ++];
                if (!girl_match[x]){
                    boy_match[i] = x;
                    girl_match[x] = i;
                }
                else if (girl[x][i] > girl[x][girl_match[x]]){
                    boy_match[girl_match[x]] = 0;
                    boy_match[i] = x;
                    girl_match[x] = i;
                }
                flag = true;
            }
        }
        if (!flag){
            break;
        }
    }
}

std::map <std::string, int> boy_map;
std::string boy_name[N];
std::map <std::string, int> girl_map;
std::string girl_name[N];

int get_boy(const std::string &str){
    if (boy_map.count(str)){
        return boy_map[str];
    }
    int sz = boy_map.size();
    ++ sz;
    boy_name[sz] = str;
    return boy_map[str] = sz;
}

int get_girl(const std::string &str){
    if (girl_map.count(str)){
        return girl_map[str];
    }
    int sz = girl_map.size();
    ++ sz;
    girl_name[sz] = str;
    return girl_map[str] = sz;
}

std::string read_string(){
    scanf("%s", input);
    return input;
}

int main(){
    int n;
    while (scanf("%d", &n) != EOF){
        boy_map.clear();
        girl_map.clear();
        for (int i = 1; i <= n; ++ i){
            int boy_id = get_boy(read_string());
            for (int j = 1; j <= n; ++ j){
                int girl_id = get_girl(read_string());
                boy[boy_id][j] = girl_id;
            }
        }
        for (int i = 1; i <= n; ++ i){
            int girl_id = get_girl(read_string());
            for (int j = 1; j <= n; ++ j){
                int boy_id = get_boy(read_string());
                girl[girl_id][boy_id] = n - j;
            }
        }
        Gale_Shapley(n);
        for (int i = 1; i <= n; ++ i){
            printf("%s %s\n", boy_name[i].c_str(), girl_name[boy_match[i]].c_str());
        }
        puts("");
    }
    return 0;
}
