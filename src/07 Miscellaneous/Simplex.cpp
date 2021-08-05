// maximize: cx, subject to: Ax <= b
// x >= 0
// A[0..n+m-1][0..n-1] = (m * n)
//                       (-In)
// c[0..n-1]
// b[0..n+m-1] = (m * 1)
//               (n * [0])
typedef std::vector<double> VD;
VD simplex(std::vector<VD> A, VD b, VD c) {
	int n = A.size(), m = A[0].size() + 1, r = n, s = m - 1;
	std::vector<VD> D(n + 2, VD(m + 1, 0));
	std::vector<int> ix(n + m);
	for (int i = 0; i < n + m; ++i) ix[i] = i;
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < m - 1; ++j) D[i][j] = -A[i][j];
		D[i][m - 1] = 1; D[i][m] = b[i];
		if (D[r][m] > D[i][m]) r = i;
	}
	for (int j = 0; j < m - 1; ++j) D[n][j] = c[j];
	D[n + 1][m - 1] = -1;
	for (double d;;) {
		if (r < n) {
			std::swap(ix[s], ix[r + m]);
			D[r][s] = 1.0 / D[r][s];
			std::vector<int> speedUp;
			for (int j = 0; j <= m; ++j) if (j != s) {
				D[r][j] *= -D[r][s];
				if (D[r][j]) speedUp.push_back(j);
			}
			for (int i = 0; i <= n + 1; ++i) if (i != r) {
				for (auto j : speedUp) D[i][j] += D[r][j]*D[i][s];
				D[i][s] *= D[r][s];
			}
		}
		r = s = -1;
		for (int j = 0; j < m; ++j)
			if (s < 0 || ix[s] > ix[j])
				if (D[n + 1][j] > eps 
				|| (D[n + 1][j] > -eps && D[n][j] > eps)) s = j;
		if (s < 0) break;
		for (int i = 0; i < n; ++i) if (D[i][s] < -eps)
			if (r < 0 
			|| (d = D[r][m]/D[r][s] - D[i][m]/D[i][s]) < -eps 
			|| (d < eps && ix[r + m] > ix[i + m]))
				r = i;
		if (r < 0) return VD(); // unbounded
	}
	if (D[n + 1][m] < -eps) return VD(); // infeasible
	VD x(m - 1);
	for (int i = m; i < n + m; ++i) if (ix[i] < m - 1) 
		x[ix[i]] = D[i - m][m];
	printf("%.0f\n", D[n][m]);
	return x;
}

