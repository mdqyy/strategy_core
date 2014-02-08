//==============================================================================
/*!
 *  \file LogisticReg.cpp
 *
 *  \brief Logistic regression.
 *
 *  \author chenqinyu
 *          contact:
 *              superthierry14@gmail.com
 *  \date 2013-07
 *
 *  \par
 */
//==============================================================================

#include <strategy_core/supervised/LogisticReg.h>
#include <strategy_core/gradient_descent/BFGS.h>
#include <map>

#define MAX_LINE_LEN 1024*1024 /// 1 MB for each line.
#define MAX_VAR_LEN 128
#define HEADER_MARK '@'
#define TOKENIZER ' '
#define SEPERATOR ':'
#define END_OF_LINE '\n'

namespace LogisticReg {
/*! SampleSet */
SampleSet::SampleSet (const INT sample_num, const INT feature_num) {
  this->sample_num = sample_num;
  this->feature_num = feature_num;
  features = new CrossList(sample_num, feature_num);
  y = new Ytype_t[sample_num];
}

SampleSet::~SampleSet () {
  delete features; features = NULL;
  delete []y; y = NULL;
}

/*! TrainingSet */
TrainingSet::TrainingSet(const char *training_set_path) {
  this->training_set_path = training_set_path;
  line = NULL;
  size = 0;
  sample_set = NULL;
  opf = NULL;
}

TrainingSet::~TrainingSet() {
  if (NULL != opf) {
    fclose(opf);
  }
  free(line); line = NULL;
  delete sample_set; sample_set = NULL;
}

bool TrainingSet::load_header() {
  bool flag = true;
  map<string, string> kv;

  opf = fopen(training_set_path, "r");
  if (NULL == opf) {
    L4C_FATAL("Open training set file failed!");
    flag = false;
    goto end;
  }

  while (-1 != getline(&line, &size, opf)) {
    char key[MAX_VAR_LEN];
    char value[MAX_VAR_LEN];
    if (HEADER_MARK == line[0]) {
      for (INT i = 1; i < strlen(line); i++) {
        memset(key, 0, sizeof(key));
        memset(value, 0, sizeof(value));
        if (TOKENIZER == line[i]) {
          strncpy(key, line + 1, i - 1);
          strncpy(value, line + i + 1, strlen(line) - i - 2);
          kv[key] = value;
          break;
        }
      }
    }
    else {
      if (kv.find("sample_num") != kv.end())
        sample_num = atoi(kv["sample_num"].c_str());
      if (kv.find("feature_num") != kv.end())
        feature_num = atoi(kv["feature_num"].c_str());
      break;
    }
  }

end:
  return flag;
}

bool TrainingSet::load_sample() {
  bool flag = true;
  INT row_id = 0;
  sample_set = new SampleSet(sample_num, feature_num);

  do {
    sample_set->y[row_id] = atoi(&(line[0]));
    INT curr_pos = 2, last_pos = curr_pos;
    char feature_id[MAX_VAR_LEN];
    char feature_value[MAX_VAR_LEN];
    while (1) {
      memset(feature_id, 0, sizeof(feature_id));
      memset(feature_value, 0, sizeof(feature_value));
      INT fid;
      REAL fvalue;
      if (SEPERATOR == line[curr_pos]) {
        strncpy(feature_id, line + last_pos, curr_pos - last_pos);
        fid = atoi(feature_id);
        last_pos = curr_pos + 1;
      }
      if (TOKENIZER == line[curr_pos] or END_OF_LINE == line[curr_pos]) {
        strncpy(feature_value, line + last_pos, curr_pos - last_pos);
        fvalue = atof(feature_value);
        CrossListNode *clnode = new CrossListNode(row_id, fid, fvalue);
        last_pos = curr_pos + 1;
        if (!sample_set->features->append(clnode)) {
          L4C_FATAL("Insert element error at (%d, %d, %f)!",
                    clnode->i, clnode->j, clnode->e);
          flag = false;
          goto end;
        }
        if (END_OF_LINE == line[curr_pos])
          break;
      }
      curr_pos++;
    }
    row_id++;
  } while (-1 != getline(&line, &size, opf));
  sample_set->features->output_all();

end:
  return flag;
}

/*! LrModel */
LrModel::LrModel(const SampleSet *ss) {
  this->ss = ss;
  const UINT d = ss->feature_num; /// Dimenstion
  wv = new dense::RealVector(d, 1);
  gv = new dense::RealVector(d, 1);
  sum_xcol = new REAL[d];
  sum_nz_xcol = new REAL[d];
  ewx = new REAL[d];
}

LrModel::~LrModel() {
  delete wv; wv = NULL;
  delete gv; gv = NULL;
  delete []sum_xcol;  sum_xcol = NULL;
  delete []sum_nz_xcol; sum_nz_xcol = NULL;
  delete []ewx; ewx = NULL;
}

void LrModel::cal_target() {
  target = 0;
  for (INT i = 0; i < ss->sample_num; i++) {
    REAL inner;
    /*TODO
    /// Transform row features to Vector
    inner = sparse::inner_product_sd(ss->features->rslArray[i], wv);
    */
    target -= std::log(1 + exp(inner));
    if (1.0 == ss->y[i])
      target += inner;
  }
}

void LrModel::cal_gradient() {
  for (INT i = 0; i < ss->feature_num; i++) {
    ///TODO
    /*
    REAL gradient = wv->get(i) * (get_sum_nz_xcol[i] -
                                          logit * get_sum_xcol[i]);
    gv->set(i, gradient);
    */
  }
}

void LrModel::cal_logit() {
  ///TODO:
}

REAL LrModel::get_ewx(const INT row) {
  ///TODO:
  return ewx[row];
};

REAL LrModel::get_sum_xcol(const INT col) {
  ///TODO:
  return sum_xcol[col];
};

REAL LrModel::get_sum_nz_xcol(const INT col) {
  ///TODO:
  return sum_nz_xcol[col];
};

LrPara::LrPara(const char *conf_file_path, const char *encoding) {
  step_len = 1e-10;
  epsilon = 1e-100;
  max_iter_num = 2000;
}

/*! Logistic regression inition */
void init(const char *log_conf_path) {
  set_cfg_path(log_conf_path);
  /// TODO:
}

/*! Logistic regression preprocessing */
bool preprocess(TrainingSet *ts) {
  bool flag = true;

  L4C_INFO("Loading header starts!");
  if (!ts->load_header()) {
    L4C_FATAL("Loading header failed!");
    flag = false;
    goto end;
  }
  L4C_INFO("Loading header finished!");

  L4C_INFO("Loading sample starts!");
  if (!ts->load_sample()) {
    L4C_FATAL("Loading sample failed!");
    flag = false;
    goto end;
  }
  L4C_INFO("Loading sample finished!");

end:
  return flag;
}

/*! Logistic regression training */
bool train(TrainingSet *ts, LrPara *lrpara) {
  bool flag = true;
  LrModel *lrmodel = new LrModel(ts->sample_set);
  BFGS *bfgs = new BFGS(lrmodel, lrpara);

  L4C_INFO("Training starts!");
  if (!bfgs->solve()) {
    L4C_FATAL("Training failed!");
    flag = false;
    goto end;
  }
  L4C_INFO("Training finished!");

end:
  delete bfgs; bfgs = NULL;
  delete lrmodel; lrmodel = NULL;
  return flag;
}

/*! Logistic regression prediction */
Ytype_p predict(const dense::RealVector *weight, const sparse::RealVector *feature) {
  Ytype_p probability;
  REAL wx = 0;
  sd::inner_product(wx, feature, weight);
  REAL exp_wx = exp(wx);
  probability = exp_wx/(1.0 + exp_wx);
  return probability;
}
}
