//==============================================================================
/*!
*  \file LogisticReg_main.cpp
*
*  \brief A running case of Logistic Regression.
*
*  \author chenqinyu
*          contact:
*              superthierry14@gmail.com
*  \date 2013-07
*
*  \par
*/
//==============================================================================
/*
#include <strategy_core/supervised/LogisticReg.h>
#include <string>
#include <iostream>

using namespace std;

int main (int argc, char **argv) {
  int flag = 0;
//for (INT i = 0; i < 10000000; i++) { /// Test memory-leaking
  const char *log_conf_path = "";
  //const char *log_conf_path = "/home/cqy/work/strategy_core/etc/LogisticReg/l4c.cfg";
  const char *conf_file_path = "";
  const char *trainingset_path = "/home/cqy/work/strategy_core/data/logistic.sample";
  const char *encoding_type = "utf-8";
  LogisticReg::init(log_conf_path);
  L4C_INFO("Logistic regression training starts!");

  ///TODO: get_opt()
  LogisticReg::LrPara *lrpara = new LogisticReg::LrPara(conf_file_path, encoding_type);
  ///Conf conf = Conf(conf_file_path, encoding_type);
  ///conf.parse(lrpara);
  LogisticReg::TrainingSet *ts = new LogisticReg::TrainingSet(trainingset_path);

  L4C_INFO("Logistic regression preprocessing starts!");
  if (!LogisticReg::preprocess(ts)) {
    L4C_FATAL("Logistic regression preprocessing failed!");
    flag = -1;
    goto end;
  }
  L4C_INFO("Logistic regression preprocessing finished!");

  L4C_INFO("Logistic regression training starts!");
  if (!LogisticReg::train(ts, lrpara)) {
    L4C_FATAL("Logistic regression training failed!");
    flag = -1;
    goto end;
  }
  L4C_INFO("Logistic regression training finished!");

end:
  delete ts;
  ts = NULL;
  delete lrpara;
  lrpara = NULL;
//}
  return flag;
}
*/
/*
   LAPACKE Example : Calling DGELS using col-major order
   =====================================================

   The program computes the solution to the system of linear
   equations with a square matrix A and multiple
   right-hand sides B, where A is the coefficient matrix
   and b is the right-hand side matrix:

   Description
   ===========

   In this example, we wish solve the least squares problem min_x || B - Ax ||
   for two right-hand sides using the LAPACK routine DGELS. For input we will
   use the 5-by-3 matrix

         ( 1  1  1 )
         ( 2  3  4 )
     A = ( 3  5  2 )
         ( 4  2  5 )
         ( 5  4  3 )
    and the 5-by-2 matrix

         ( -10 -3 )
         (  12 14 )
     B = (  14 12 )
         (  16 16 )
         (  18 16 )
    We will first store the input matrix as a static C two-dimensional array,
    which is stored in col-major order, and let LAPACKE handle the work space
    array allocation. The LAPACK base name for this function is gels, and we
    will use double precision (d), so the LAPACKE function name is LAPACKE_dgels.

    lda=5 and ldb=5. The output for each right hand side is stored in b as
    consecutive vectors of length 3. The correct answer for this problem is
    the 3-by-2 matrix

         ( 2 1 )
         ( 1 1 )
         ( 1 2 )

    A complete C program for this example is given below. Note that when the arrays
     are passed to the LAPACK routine, they must be dereferenced, since LAPACK is
      expecting arrays of type double *, not double **.


   LAPACKE Interface
   =================

   LAPACKE_dgels (col-major, high-level) Example Program Results

  -- LAPACKE Example routine (version 3.5.0) --
  -- LAPACK is a software package provided by Univ. of Tennessee,    --
  -- Univ. of California Berkeley, Univ. of Colorado Denver and NAG Ltd..--
     February 2012

*/
/* Calling DGELS using col-major order */

/* Includes */
#include <lapacke/lapacke.h>

/* Main program */
int main (int argc, const char * argv[]) {
   /* Locals */
   double A[5][3] = {1,2,3,4,5,1,3,5,2,4,1,4,2,5,3};
   double b[5][2] = {-10,12,14,16,18,-3,14,12,16,16};
   lapack_int info,m,n,lda,ldb,nrhs;
   int i,j;

   /* Initialization */
   m = 5;
   n = 3;
   nrhs = 2;
   lda = 5;
   ldb = 5;

   /* Solve least squares problem*/
   info = LAPACKE_dgels(LAPACK_COL_MAJOR, 'N', m, n, nrhs, *A, lda, *b, ldb);

   /* Print Solution */
   exit(0);
} /* End of LAPACKE_dgels Example */
