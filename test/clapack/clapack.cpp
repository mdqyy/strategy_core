#include <iostream>
#include <clapack/f2c.h>

extern"C"{
#include <clapack/clapack.h>
}
using namespace std;

int main(void) {
  integer M = 3;
  integer N = 1;
 
  real a[9] = {4,3,11,2,-1,0,-1,2,3};
  real b[3] = {2,10,8};

  integer lda;
  integer ldb;

  integer INFO;

  lda = M;
  ldb = M;
 
  integer ipiv[M];
 
  sgesv_(&M, &N, a, &lda,ipiv, b, &ldb, &INFO);
 
  if(0 == INFO) {
     for(int i=0;i<M;i++) {     
         cout<<b[i]<<endl;
     }     
  }
  else {
     cout<<"Failed."<<endl;
  }  

  return 0;
}

