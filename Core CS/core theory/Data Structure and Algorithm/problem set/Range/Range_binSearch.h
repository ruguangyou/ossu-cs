
int binSearch(int lo, int hi, int* R, int const& e) {
	while (lo < hi) {
		int mi = (lo + hi) >> 1;
		(e < R[mi]) ? hi = mi : lo = mi + 1;
	}
	return --lo;
}