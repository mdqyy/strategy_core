//==============================================================================
/*!
 *  \file LogisticReg.h
 *
 *  \brief LogisticReg.h
 *
 *  \author chenqinyu
 *          contact:
 *              qinyu.chen@corp.elong.com
 *              superthierry14@gmail.com
 *  \date 2013-07
 *
 *  \par http://www.elong.com<BR>
 */
//==============================================================================

#ifndef STRATEGY_CORE_SUPERVISED_LOGISTICREG_H_
#define STRATEGY_CORE_SUPERVISED_LOGISTICREG_H_

#include <strategy_core/lin_alg/sparse.h>
#include <strategy_core/common/common.h>
#include <strategy_core/lin_alg/base.h>
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

class LrPara {
 public:
  REAL step_len;
  REAL stop_cond;
  LrPara();
  ~LrPara();

 protected:
  ;
 private:
  ;
};

class LrModel {
 public:
  REAL *tmp_xcol; /// intermediate result: sum(x_ij, i, 1, sample_num)
                  /// init when loading whole data set.
  REAL *tmp_ewx;  /// intermediate result: exp(inner(W, Xi))
                  /// init when doing every step.
  REAL get_target(TrainingSet *ts, VectorType *weight);
  VectorType *get_gradient();
  LrModel();
  ~LrModel();

  REAL get_tmp_ewx(const INT row) { /// row: [0, sample_num - 1]
    return tmp_ewx[row];
  };

  REAL get_tmp_xcol(const INT col) { /// col: [0, feature_num - 1]
    return tmp_xcol[col];
  };

};

void init(const char *logfile_path);
bool train(const TrainingSet *ts, LrPara *lrPara);
Ytype_p predict(Model *model, VectorType *features);

}

#endif //STRATEGY_CORE_SUPERVISED_LOGISTICREG_H_
