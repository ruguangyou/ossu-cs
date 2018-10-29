
void merge(int lo, int mi, int hi, int* R) {
	int* A = R + lo;
	int lb = mi-lo; int* B = new int[lb];
	for (int i = 0; i < lb; B[i] = A[i++]);
	int lc = hi - mi; int* C = R + mi;
	/*
	for (int i = 0, j = 0, k = 0; (j < lb) || (k < lc); ) {
		if ( (j < lb) && ( (k >= lc) || (B[j] <= C[k]) ) ) A[i++] = B[j++];
		if ( (k < lc) && ( (j >= lb) || (B[j] >  C[k]) ) ) A[i++] = C[k++];
	}
	*/
	for (int i = 0, j = 0, k = 0; (j < lb); ) {
		if ( (k >= lc) || (B[j] <= C[k]) ) A[i++] = B[j++];
		if ( (k <  lc) && (B[j] >  C[k]) ) A[i++] = C[k++];
	}
	
	delete [] B;
}