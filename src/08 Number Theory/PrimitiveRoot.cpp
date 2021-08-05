<TeX>
原根模板。checkroot() 返回 rt 是否是 a 的原根，solve() 返回 a 最小的原根，如 a 无原根，不保证结果正确。
原根数量为 $\varphi(\varphi(m))$ 个，分布基本随机。
</TeX>

#include<bits/stdc++.h>
#include"pollardrho.cpp"
#define ll long long

template <typename T>
class root{
private:
	T a, ans;
	std::vector <T> fact;
	
	void solve(){
		pollardrho <T> p(a - 1);
		fact = p.solve();
		std::sort(fact.begin(), fact.end());
		fact.erase(std::unique(fact.begin(), fact.end()), fact.end());
		for (T i = 1; i < a; ++ i){
			if (checkroot(i)){
				ans = i;
				return;
			}
		}
	}

public:
	root(){}
	
	root(T a):a(a){
		solve();
	}
	
	bool checkroot(T rt){
		for (auto u : fact){
			if (powermod(rt, a / u, a) == 1) return false;
		}
		return true;
	}
	
	T minroot(){
		return ans;
	}
};

int main(){
	srand((unsigned) time(NULL));
	printf("%d\n", root <int>(1000210433).minroot());
	return 0;
}
