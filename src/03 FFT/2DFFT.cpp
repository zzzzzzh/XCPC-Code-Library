<TeX>二维FFT模板，求两个二维多项式的乘积。真正比赛中请手写复数类，std::complex较慢。</TeX>

#include<bits/stdc++.h>
#include"fft.cpp"

const int MAX = 11;
const double eps = 1e-9;

void FFT_2D(std::complex <double> (*a)[1 << MAX], int lengthx, int lengthy, int type){
	for (int i = 0; i <= lengthx; ++ i){
		FFT(a[i], lengthy, type);
	}
	for (int i = 0, sz = std::max(lengthx, lengthy); i <= sz; ++ i){
		for (int j = i + 1; j < sz; ++ j){
			std::swap(a[i][j], a[j][i]);
		}
	}
	for (int i = 0; i <= lengthy; ++ i){
		FFT(a[i], lengthx, type);
	}
}

// a[i][j]表示二维多项式 a 的 (x^i)*(y^j) 项的系数
// lengthax 表示 x 的次数界，lengthay 表示 y 的次数界
// 计算结果返回到 a 中 
void mult(double (*a)[1 << MAX], int lengthax, int lengthay, double (*b)[1 << MAX], int lengthbx, int lengthby){
	static std::complex <double> aux[2][1 << MAX][1 << MAX];
	memset(aux, 0, sizeof(aux));
	int lengthx = 1, lengthy = 1, nx = lengthax + lengthbx, ny = lengthay + lengthby;
	for ( ; lengthx <= nx; lengthx <<= 1)
		;
	for ( ; lengthy <= ny; lengthy <<= 1)
		;
	for (int i = 0; i <= lengthax; ++ i){
		for (int j = 0; j <= lengthay; ++ j){
			aux[0][i][j] = a[i][j];
		}
	}
	for (int i = 0; i <= lengthbx; ++ i){
		for (int j = 0; j <= lengthby; ++ j){
			aux[1][i][j] = b[i][j];
		}
	}
	FFT_2D(aux[0], lengthx, lengthy, 1);
	FFT_2D(aux[1], lengthx, lengthy, 1);
	for (int i = 0; i < lengthy; ++ i){
		for (int j = 0; j < lengthx; ++ j){
			aux[0][i][j] *= aux[1][i][j];
		}
	}
	FFT_2D(aux[0], lengthy, lengthx, -1);
	for (int i = 0; i <= nx; ++ i){
		for (int j = 0; j <= ny; ++ j){
			a[i][j] = aux[0][i][j].real();
		}
	}
}

int main(){
	return 0;
}
