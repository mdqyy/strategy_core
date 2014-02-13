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
#include <strategy_core/lin_alg/sd.h>
#include <strategy_core/discrete/CrossList.h>
#include <strategy_core/common/algorithm.h>

using namespace std;

namespace LogisticReg {
class SampleSet {
 public:
  CrossList *features;    // X
  REAL *y;                 // Y
  INT sample_num;
  INT feature_num;
  SampleSet(const INT sample_num, const INT feature_num);
  ~SampleSet();
};

class TrainingSet {
 public:
  //header info
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
  const SampleSet *ss;
  REAL target;           // Value of target function.
  dense::RealVector *wv; // weight vector
  dense::RealVector *gv; // gradient vector
  REAL *sum_xcol;        // intermediate result: sum(x_ij, i, 1, sample_num)
                         // init when loading whole data set.
                         // len: feature_num
  REAL *sum_nz_xcol;     // intermediate result: sum(x_ij, i, 1, sample_num) | yi != 0
                         // init when loading whole data set.
                         // len: feature_num
  REAL *logit;           // intermediate result: ewxi/(1 + ewxi)
                         // init when doing every step.
                         // len: sample_num

  LrModel(const SampleSet *ss);
  ~LrModel();
  bool preprocess();     // init sum_xcol, sum_nz_xcol
  bool write();
  bool read();
  void cal_target();     // The target funtion is the likelihood function of
                         // logisitic regression.
  void cal_gradient();
  void cal_logit();
};

class LrPara {
 public:
  REAL step_len;
  REAL step;
  REAL epsilon;
  INT max_iter_num;

  LrPara(const char *conf_file_path, const char *encoding);
  ~LrPara() {};
};

void init(const char *logfile_path);
bool preprocess(TrainingSet *ts);
bool train(TrainingSet *ts, LrPara *lrpara);
REAL predict(const SingleList &feature, const dense::RealVector &weight);
}

#endif //STRATEGY_CORE_SUPERVISED_LOGISTICREG_H_
