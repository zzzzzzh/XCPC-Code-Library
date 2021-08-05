const db eps = 1e-5;
db simpson(db l, db r, db (*f)(db)){
	db mid = (l + r) * 0.5;
	return (f(l) + 4 * f(mid) + f(r)) * (r - l) / 6;
}
db asr(db l, db r, db eps, db value, db (*f) (db)){
	db mid = (l + r) * 0.5;
	db left = simpson(l, mid, f);
    db right = simpson(mid, r, f);
	if (std::abs(left + right - value) < 15 * eps)
		return left + right + (left + right - value) / 15;
	return asr(l, mid, eps / 2, left, f) 
         + asr(mid, r, eps / 2, right, f);
}
db calc(db l, db r, db (*f) (db), db eps) {
	return asr(l, r, eps, simpson(l, r, f), f);
}
