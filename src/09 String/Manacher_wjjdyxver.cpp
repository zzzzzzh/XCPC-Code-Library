<TeX> S starts at 0. </TeX>
<TeX> type = 0 -> odd, type = 1 -> even </TeX>
<TeX> palindromic centre of 'abba' is 1 </TeX>
namespace Manacher { 
	int r[2][size]; //palindromic radius
	template<typename T>
	void manacher(T S, int len, int type) {
		register int i, j, k;
		int *R = r[type];
		for(i=j=0; i<len; i+=k, j=std::max(j-k,0)){
			while(i-j>=0&&i+j+type<len&&S[i-j]==S[i+j+type])++j;
			R[i] = j;
			for (k = 1; k < j && R[i - k] != R[i] - k; ++k)
				R[i + k] = std::min(R[i - k], R[i] - k);
		}}}
