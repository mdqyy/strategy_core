#include <iostream>

extern"C"{
#include <lapacke/lapacke.h>
}
using namespace std;

int main(void) {
  /// Advanced algebra
  /// Peking Univ. p103.6
  lapack_int n, nrhs, lda, ldb, info;
	lapack_int *ipiv;
  n = 4, nrhs = 1, lda = n, ldb = nrhs;

  float a[4*4] = {9, -2, 0, -1, 
                 -2, 10, -1, 0, 
                  0, -1, 13, -4,
                 -1, 0, 10, -4 
                 };
  float b[4] = {100,100,100,100};

  lda = n;
  ldb = nrhs;
  ipiv = new lapack_int[n];
 
  info = LAPACKE_sgesv(LAPACK_ROW_MAJOR, n, nrhs, a, lda, ipiv, b, ldb);

  if(0 == info) {
     for(int i=0;i<n;i++) {     
        cout<<b[i]<<endl;
     }     
  }
  else {
     cout<<"Failed."<<endl;
  }  

  return 0;
}

