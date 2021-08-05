void FMT(int *f, int len, int type) {
	for (int j = 0; j < len; ++j)
		for (int i = 0; i < 1 << len; ++i)
			if (i >> j & 1) {
				if (!type) f[i] = f[i] + f[i ^ 1 << j];
				else f[i] = f[i] - f[i ^ 1 << j];
			}
}
