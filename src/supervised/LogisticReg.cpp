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
  this->features = new CrossList(sample_num, feature_num);
  this->y = new Ytype_t[sample_num];
}

SampleSet::~SampleSet () {
  delete this->features;  this->features = NULL;
  delete []this->y; this->y = NULL;
}

/*! TrainingSet */
TrainingSet::TrainingSet(const char *training_set_path) {
  this->training_set_path = training_set_path;
  this->line = NULL;
  this->size = 0;
  this->sample_set = NULL;
  this->opf = NULL;
}

TrainingSet::~TrainingSet() {
  if (NULL != this->opf)
    fclose(this->opf);
  free(this->line);  this->line = NULL;
  delete this->sample_set;  this->sample_set = NULL;
}

bool TrainingSet::load_header() {
  bool flag = true;
  map<string, string> kv;

  this->opf = fopen(this->training_set_path, "r");
  if (NULL == this->opf) {
    L4C_FATAL("Open training set file failed!");
    flag = false;
    goto end;
  }

  while (-1 != getline(&this->line, &this->size, this->opf)) {
    char key[MAX_VAR_LEN];
    char value[MAX_VAR_LEN];
    if (HEADER_MARK == this->line[0]) {
      for (INT i = 1; i < strlen(this->line); i++) {
        memset(key, 0, sizeof(key));
        memset(value, 0, sizeof(value));
        if (TOKENIZER == this->line[i]) {
          strncpy(key, this->line + 1, i - 1);
          strncpy(value, this->line + i + 1, strlen(this->line) - i - 2);
          kv[key] = value;
          break;
        }
      }
    }
    else {
      if (kv.find("sample_num") != kv.end())
        this->sample_num = atoi(kv["sample_num"].c_str());
      if (kv.find("feature_num") != kv.end())
        this->feature_num = atoi(kv["feature_num"].c_str());
      break;
    }
  }

end:
  return flag;
}

bool TrainingSet::load_sample() {
  bool flag = true;
  INT row_id = 0;
  this->sample_set = new SampleSet(this->sample_num, this->feature_num);

  do {
    this->sample_set->y[row_id] = atoi(&(this->line[0]));
    INT curr_pos = 2, last_pos = curr_pos;
    char feature_id[MAX_VAR_LEN];
    char feature_value[MAX_VAR_LEN];
    while (1) {
      memset(feature_id, 0, sizeof(feature_id));
      memset(feature_value, 0, sizeof(feature_value));
      INT fid;
      REAL fvalue;
      if (SEPERATOR == line[curr_pos]) {
        strncpy(feature_id, this->line + last_pos, curr_pos - last_pos);
        fid = atoi(feature_id);
        last_pos = curr_pos + 1;
      }
      if (TOKENIZER == line[curr_pos] or END_OF_LINE == line[curr_pos]) {
        strncpy(feature_value, this->line + last_pos, curr_pos - last_pos);
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
  } while (-1 != getline(&this->line, &this->size, this->opf));
  sample_set->features->output_all();

end:
  return flag;
}

/*! LrModel */
LrModel::LrModel(const SampleSet *ss) {
  this->ss = ss;
  const UINT d = this->ss->feature_num; /// Dimenstion
  this->weight_vector = new dense::RealMatrix(d, 1);
  this->gradient_vector = new dense::RealMatrix(d, 1);
  this->sum_xcol = new REAL[d];
  this->sum_nz_xcol = new REAL[d];
  this->ewx = new REAL[d];
}

LrModel::~LrModel() {
  delete this->weight_vector; this->weight_vector = NULL;
  delete this->gradient_vector; this->gradient_vector = NULL;
  delete []this->sum_xcol;  this->sum_xcol = NULL;
  delete []this->sum_nz_xcol; this->sum_nz_xcol = NULL;
  delete []this->ewx; this->ewx = NULL;
}

void LrModel::cal_target() {
  this->target = 0;
  for (INT i = 0; i < this->ss->sample_num; i++) {
    REAL inner;
    /*TODO
    /// Transform row features to Vector
    inner = sparse::inner_product_sd(this->ss->features->rslArray[i],
                                     this->weight_vector);
    */
    this->target -= std::log(1 + exp(inner));
    if (1.0 == this->ss->y[i])
      this->target += inner;
  }
}

void LrModel::cal_gradient() {
  for (INT i = 0; i < this->ss->feature_num; i++) {
    ///TODO
    /*
    REAL gradient = this->weight_vector->get(i) * (this->get_sum_nz_xcol[i] -
                                          this->logit * this->get_sum_xcol[i]);
    this->gradient_vector->set(i, gradient);
    */
  }
}

void LrModel::cal_logit() {
  ///TODO:
}

REAL LrModel::get_ewx(const INT row) {
  ///TODO:
  return this->ewx[row];
};

REAL LrModel::get_sum_xcol(const INT col) {
  ///TODO:
  return this->sum_xcol[col];
};

REAL LrModel::get_sum_nz_xcol(const INT col) {
  ///TODO:
  return this->sum_nz_xcol[col];
};

LrPara::LrPara(const char *conf_file_path, const char *encoding) {
  this->step_len = 1e-10;
  this->epsilon = 1e-100;
  this->max_iter_num = 2000;
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
