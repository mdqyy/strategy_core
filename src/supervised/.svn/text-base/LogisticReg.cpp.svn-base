//==============================================================================
/*!
 *  \file LogisticReg.cpp
 *
 *  \brief Logistic regression.
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

#include <strategy_core/supervised/LogisticReg.h>
#include <strategy_core/gradient_descent/BFGS.h>
#include <strategy_core/common/log4c.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <map>
#include <cmath>

#define MAX_LINE_LEN 1024*1024 // 1 MB for each line.
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
  this->features = new sparse::CrossList(sample_num, feature_num);
  this->y = new Ytype_t[sample_num];
}

SampleSet::~SampleSet () {
  delete this->features;
  this->features = NULL;

  delete[] this->y;
  this->y = NULL;
}

/*! TrainingSet */
TrainingSet::TrainingSet (const char *training_set_path) {
  this->training_set_path = training_set_path;
  this->line = NULL;
  this->size = 0;
  this->sample_set = NULL;
  this->opf = NULL;
}

TrainingSet::~TrainingSet () {
  if (NULL != this->opf)
    fclose(this->opf);
  delete this->line;
  this->line = NULL;
  delete this->sample_set;
  this->sample_set = NULL;
}

void init (const char *log_conf_path) {
  set_cfg_path(log_conf_path);
  /// TODO:
}

bool TrainingSet::load_header () {
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

bool TrainingSet::load_sample () {
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
        sparse::CrossListNode *clnode = new sparse::CrossListNode();
        clnode->i = row_id;
        clnode->j = fid;
        clnode->e = fvalue;
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
REAL LrModel::get_target (TrainingSet *ts, VectorType *weight) {
  /*!
    The likelihood function of logisitic regression.
  */
  REAL target;
  for (INT i = 0; i < ts->feature_num; i++) {
    REAL inner, part;
    for (INT j = 0; j < ts->feature_num; j++) {
      ///TODO:
      ///inner = inner_product(ts->features, weight);
    }
    target -= std::log(1 + exp(inner));
    /*
    if (1 == ts->sample_set->)
      target += inner;
    */
  }
  return target;
}

VectorType *LrModel::get_gradient () {
  VectorType *gradient;
  ///TODO:
  return gradient;
}

/*! Logistic regression training */
bool train (const TrainingSet *ts, LrPara *lrpara) {
  bool flag = true;

  BFGS *bfgs = new BFGS();
  if (!bfgs->init()) {
    flag = false;
    goto end;
  }
  if (!bfgs->read()) {
    flag = false;
    goto end;
  }
  if (!bfgs->step()) {
    flag = false;
    goto end;
  }
  if (!bfgs->write()) {
    flag = false;
    goto end;
  }

end:
  delete bfgs;
  return flag;
}

/*! Logistic regression prediction */
Ytype_p predict (Model *model, VectorType *feature) {
  Ytype_p probability;
  ///TODO:
  return probability;
}
}
