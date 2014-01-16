//==============================================================================
/*!
 *  \file LogisticReg.h
 *
 *  \brief LogisticReg.h
 *
 *  \author chenqinyu
 *          contact:
 *              superthierry14@gmail.com
 *  \date 2013-07
 *
 *  \par
 */
//==============================================================================

#ifndef STRATEGY_CORE_SUPERVISED_LOGISTICREG_H_
#define STRATEGY_CORE_SUPERVISED_LOGISTICREG_H_

#include <strategy_core/lin_alg/dense.h>
#include <strategy_core/lin_alg/sparse.h>
#include <strategy_core/common/algorithm.h>
#include <string>

using namespace std;

namespace LogisticReg {
#define VectorType sparse::SparseVector
#define Ytype_t INT
#define Ytype_p REAL

class SampleSet {
 public:
  sparse::CrossList *features; /// X
  Ytype_t *y; /// Y
  INT sample_num;
  INT feature_num;
  SampleSet(const INT sample_num, const INT feature_num);
  ~SampleSet();
};

class TrainingSet {
 public:
  ///header info
  INT sample_num;
  INT feature_num;

  char *line;
  INT size;
  const char *training_set_path;
  FILE *opf;
  SampleSet *sample_set;

  TrainingSet(const char *training_set_path);
  ~TrainingSet();
  bool load_header(void);
  bool load_sample(void);
};

class LrModel:algorithm::Model {
 public:
  REAL target;  /// Value of target function.
  dense::DenseRealVector *weight_vector;
  SampleSet *ss;
  dense::DenseRealVector *gradient_vector;
  REAL *sum_xcol; /// intermediate result: sum(x_ij, i, 1, sample_num)
                  /// init when loading whole data set.
  REAL *sum_nz_xcol; /// intermediate result: sum(x_ij, i, 1, sample_num) | yi != 0
                  /// init when loading whole data set.
  REAL *ewx;      /// intermediate result: exp(inner(W, Xi))
                  /// init when doing every step.
  REAL logit;     /// logit = ewx/(1 + ewx)
  LrModel(const SampleSet *ss);
  ~LrModel();
  void cal_target();/// The target funtion is the likelihood function of
                    /// logisitic regression.
  void cal_gradient();
  void cal_logit();
  REAL get_ewx(const INT row); /// row: [0, sample_num - 1]
  REAL get_sum_xcol(const INT col); /// col: [0, feature_num - 1]
  REAL get_sum_nz_xcol(const INT col); /// col: [0, feature_num - 1]

  bool preprocess(const TrainingSet *ts);
};

class LrPara:algorithm::Para {
 public:
  REAL step_len;
  REAL epsilon;
  INT max_iter_num;

  LrPara(const char *conf_file_path, const char *encoding);
  ~LrPara();
};

void init(const char *logfile_path);
bool train(const TrainingSet *ts, const LrPara *lrpara);
Ytype_p predict(const dense::DenseRealVector *weight, const sparse::SparseVector *feature);
}

#endif //STRATEGY_CORE_SUPERVISED_LOGISTICREG_H_
