<TeX>栈中维护一个严格递增的右链。小根堆 $(a_i, i)$, 根是 \text{st[1]}。</TeX>
for (int i = 1; i <= n; ++i) {
	while (top && a[st[top]] >= a[i]) ch[i][0] = st[top--];
	if (top) ch[st[top]][1] = i;
	st[++top] = i;
}
