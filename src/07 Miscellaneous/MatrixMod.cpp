<TeX>Warning: 这份模板写完后就没用过了，讲道理矩阵题一般都能手搓。</TeX>

#include<bits/stdc++.h>
#define ll long long

template <typename T>
class matrix{
private:
	static const int MAXN = 100000;
	static T **ptrpool, *Tpool;
	static int ptrsize, Tsize;
	
	T **a;
	int n, m;
	T moder;
	
	T **newptrs(int size){
		if (ptrsize < size){
			ptrpool = new T *[ptrsize = std::max((int) MAXN, size)];
		}	
		return ptrpool + (ptrsize -= size);
	}
	
	T *newTs(int size){
		if (Tsize < size){
			Tpool = new T[Tsize = std::max((int) MAXN, size)]();
		}
		return Tpool + (Tsize -= size);
	}
	
	ll getone(int n)const{return 1;}
	__int128 getone(ll n)const{return 1;}
	
	T powermod(T a, T exp)const{
		T ret = 1;
		auto one = getone(a);
		for ( ; exp; exp >>= 1){
			if (exp & 1){
				ret = one * ret * a % moder;
			}
			a = one * a * a % moder;
		}
		return ret;
	}
	
	void apply(int n, int m){
		if (!n || !m) return;
		a = newptrs(n);
		for (int i = 0; i < n; ++ i){
			a[i] = newTs(m);
		}
		this -> n = n, this -> m = m;
	}
	
	void InitMatrix(const matrix <T> &p){
		apply(p.n, p.m);
		moder = p.moder;
		for (int i = 0; i < n; ++ i){
			memcpy(a[i], p.a[i], sizeof(T) * m);
		}
	}
	
	void InitMatrix(matrix <T> &&p){
		a = p.a, n = p.n, m = p.m, moder = p.moder;
	}
	
public:
	matrix <T>():n(0), m(0), moder(0){a = nullptr;}
	matrix <T>(int n, int m, T moder):moder(moder){apply(n, m);}
	matrix <T>(const matrix <T> &p){InitMatrix(p);}
	matrix <T>(matrix <T> &&p){InitMatrix(std::move(p));}
	T *operator [](int n){return a[n];}
	int getline(){return n;}
	int getcolumn(){return m;}
	T getmod(){return moder;}
	void setmod(T moder){this -> moder = moder;}
	void clear(){a = nullptr; n = m = moder = 0;}
	
	void resize(int n, int m){
		if (this -> n == n && this -> m == m) return;
		apply(n, m);
	}
	
	matrix <T> &operator = (const matrix <T> &p){
		if (&p != this) InitMatrix(p);
		return *this;
	}
	
	matrix <T> &operator = (matrix <T> &&p){
		InitMatrix(std::move(p));
		return *this;
	}
	
	bool operator == (const matrix <T> &p)const{
		if (n != p.n || m != p.m) return false;
		for (int i = 0; i < n; ++ i){
			for (int j = 0; j < m; ++ j){
				if (a[i][j] != p.a[i][j]) return false;
			}
		}
		return true;
	}
	
	matrix <T> operator + (const matrix <T> &p)const{
		if (n != p.n || m != p.m) assert(("Invalid matrix addition!", 0));
		matrix <T> ret(n, m, moder);
		for (int i = 0; i < n; ++ i){
			for (int j = 0; j < m; ++ j){
				ret.a[i][j] = a[i][j] + p.a[i][j];
				ret.a[i][j] -= ret.a[i][j] >= moder ? moder : 0;
			}
		}
		return ret;
	}
	
	matrix <T> operator - (const matrix <T> &p)const{
		if (n != p.n || m != p.m) assert(("Invalid matrix subtraction!", 0));
		matrix <T> ret(n, m, moder);
		for (int i = 0; i < n; ++ i){
			for (int j = 0; j < m; ++ j){
				ret.a[i][j] = a[i][j] - p.a[i][j];
				ret.a[i][j] += ret.a[i][j] < 0 ? moder : 0;
			}
		}
		return ret;
	}
	
	matrix <T> operator - ()const{
		matrix <T> ret(n, m, moder);
		for (int i = 0; i < n; ++ i){
			for (int j = 0; j < m; ++ j){
				ret.a[i][j] = ret.a[i][j] ? moder - ret.a[i][j] : 0;
			}
		}
		return ret;
	}
	
	matrix <T> operator * (const matrix &p)const{
		if (m != p.n) assert(("Invalid matrix multiplication!", 0));
		matrix <T> ret(n, p.m, moder);
		auto one = getone(moder);
		for (int k = 0; k < m; ++ k){
			for (int i = 0; i < n; ++ i){
				for (int j = 0; j < p.m; ++ j){
					ret.a[i][j] = (ret.a[i][j] + one * a[i][k] * p.a[k][j]) % moder;
				}
			}
		}
		return ret;
	}
	
	matrix <T> operator * (T &p)const{
		matrix <T> ret(n, m);
		auto one = getone(moder);
		for (int i = 0; i < n; ++ i){
			for (int j = 0; j < m; ++ j){
				ret.a[i][j] = one * p * a[i][j] % moder;
			}
		}
		return ret;
	}
	
	friend matrix <T> operator * (const T &p, const matrix <T> &mat){return mat * p;}
	matrix <T> &operator += (const matrix &p){*this = *this + p; return *this;}
	matrix <T> &operator -= (const matrix &p){*this = *this - p; return *this;}
	matrix <T> &operator *= (const T &p){*this = *this * p; return *this;}
	
	template <typename K>
	matrix <T> power(K exp)const{
		if (n != m) assert(("Invalid matrix power!", 0));
		matrix <T> ret(matrixI(n, moder)), aux(*this);
		for ( ; exp; exp >>= 1){
			if (exp & 1){
				ret = ret * aux;
			}
			aux = aux * aux;
		}
		return ret;
	}
	
	template <typename K>
	matrix <T> powersum(K exp)const{
		if (n != m) assert(("Invalid matrix powersum!", 0));
		matrix <T> ret(matrixI(n, moder)), aux(*this);
		matrix <T> unit(ret);
		K exp1 = 1;
		for (++ exp; exp > 1; exp1 = exp1 << 1 | exp & 1, exp >>= 1)
			;
		for ( ; exp1 > 1; exp1 >>= 1){
			ret = ret * (aux + unit);
			aux = aux * aux;
			if (exp1 & 1){
				ret += aux;
				aux = aux * *this;
			}
	    }
	    return ret;
	}
	
	T det()const{
		if (n != m) assert(("Invalid matrix det!", 0));
		matrix aux(*this);
		T ret = 1;
		auto one = getone(a[0][0]);
		for (int i = 0; i < aux.n; ++ i){
			int sit = -1;
			for (int j = i; j < aux.n; ++ j){
				if (aux.a[j][i]){
					sit = j;
					break;
				}
			}
			if (!~sit) return 0;
			if (sit != i){
				for (int j = i; j < aux.n; ++ j){
					std::swap(aux.a[i][j], aux.a[sit][j]);
				}
				ret = ret ? moder - ret : 0;
			}
			for (int j = i + 1; j < n; ++ j){
				for (int now = j, pre = i; aux.a[pre][i]; std::swap(now, pre)){
					T coe = aux.a[now][i] / aux.a[pre][i];
					for (int k = i; k < n; ++ k){
						aux.a[now][k] = (aux.a[now][k] - one * coe * aux.a[pre][k] % moder + moder) % moder;
					}
				}
				if (!aux.a[i][i]){
					ret = ret ? moder - ret : 0;
					for (int k = i; k < n; ++ k){
						std::swap(aux.a[i][k], aux.a[j][k]);
					}
				}
			}
		}
		for (int i = 0; i < n; ++ i){
			ret = one * ret * aux.a[i][i] % moder;
		}
		return ret;
	}
	
	matrix <T> gauss(const matrix <T> &p)const{
		matrix aux1(*this), aux2(p);
		if (aux1.n != aux2.n) assert(("Invalid matrix equation!", 0));
		int cnt = 0;
		std::vector <int> aux;
		for (int i = 0; i < aux1.m; ++ i){
			int sit = -1;
			for (int j = cnt; j < aux1.n; ++ j){
				if (aux1.a[j][i]){
					sit = j;
					break;
				}
			}
			if (!~sit){
				aux.push_back(i);
				continue;
			}
			int inv = powermod(aux1.a[sit][i], moder - 2);
			for (int j = i; j < aux1.m; ++ j){
				std::swap(aux1.a[cnt][j], aux1.a[sit][j]);
				aux1.a[cnt][j] = 1ll * aux1.a[cnt][j] * inv % moder;
			}
			for (int j = 0; j < aux2.m; ++ j){
				std::swap(aux2.a[cnt][j], aux2.a[sit][j]);
				aux2.a[cnt][j] = 1ll * aux2.a[cnt][j] * inv % moder;
			}
			for (int j = 0; j < aux1.n; ++ j){
				if (j == cnt) continue;
				int x = (moder - aux1.a[j][i]) % moder;
				for (int k = i; k < aux1.m; ++ k){
					aux1.a[j][k] = (aux1.a[j][k] + 1ll * x * aux1.a[cnt][k]) % moder;
				}
				for (int k = 0; k < aux2.m; ++ k){
					aux2.a[j][k] = (aux2.a[j][k] + 1ll * x * aux2.a[cnt][k]) % moder;
				}
			}
			++ cnt;
		}
		for (int i = cnt; i < aux2.n; ++ i){
			for (int j = 0; j < aux2.m; ++ j){
				if (aux2.a[i][j]) return matrix <T>();
			}
		}
		int sz = aux.size();
		matrix ret(aux1.m, sz + aux2.m, moder);
		for (int i = 0; i < aux2.n; ++ i){
			for (int j = 0; j < aux2.m; ++ j){
				ret.a[i][j] = aux2.a[i][j];
			}
		}
		for (int i = 0; i < sz; ++ i){
			for (int j = 0; j < aux2.n; ++ j){
				if (j == aux[i]){
					ret.a[j][i + aux2.m] = 1;
				}
				else{
					ret.a[j][i + aux2.m] = (moder - aux1.a[j][aux[i]]) % moder;
				}
			}
		}
		return ret;
	}
	
	matrix <T> inv()const{
		if (n != m) assert(("Invalid matrix inv!", 0));
		matrix <T> aux(matrixI(n, moder));
		aux = gauss(aux);
		if (!aux.n || aux.n != aux.m) return matrix <T>();
		return aux;
	}
	
	static matrix <T> matrixI(int n, int moder){
		matrix <T> ret(n, n, moder);
		for (int i = 0; i < n; ++ i){
			ret.a[i][i] = 1;
		}
		return ret;
	}
	
	void write(){
		for (int i = 0; i < n; ++ i){
			for (int j = 0; j < m; ++ j){
				std::cout << a[i][j];
				putchar(" \n"[j == m - 1]);
			}
		}
	}
};

template <typename T> T **matrix <T>::ptrpool = nullptr;
template <typename T> T *matrix <T>::Tpool = nullptr;
template <typename T> int matrix <T>::ptrsize = 0;
template <typename T> int matrix <T>::Tsize = 0;

int main(){
	return 0;
}
