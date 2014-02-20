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

#include <strategy_core/supervised/LogisticReg.h>
#include <string>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>

using namespace std;


int main(int argc, char **argv) {
  int flag = 0;
  char* conf = NULL;
  char* input = NULL;
  char* output = NULL;
  int errflag = 0;

  for (int i = 1; i < argc; i++) {
    if (0 == strcmp(argv[i], "-c")) {
      i++;
      if (i < argc) {
        conf = argv[i];
      }
      else {
        errflag = 1;
        break;
      }
    }
    else if (0 == strcmp(argv[i], "-i")) {
      i++;
      if (i < argc) {
        input = argv[i];
      }
      else {
        errflag = 1;
        break;
      }
    }
    else if (0 == strcmp(argv[i], "-o")) {
      i++;
      if (i < argc) {
        output = argv[i];
      }
      else {
        errflag = 1;
        break;
      }
    }
    else {
      cerr << argv[i] << endl;
      errflag = 1;
    }
  }

  if (!conf || !input || !output || errflag) {
    cerr << "Usage: " << endl;
    cerr << '\t' << argv[0] << " [options]" << endl;
    cerr << "Example:" << endl;
    cerr << '\t' << argv[0] <<
    " -c LogisticReg.conf -i LogisticReg.input -o LogisticReg.model" << endl;
    return -1;
  }

  const char *log_conf_path = "/home/cqy/work/strategy_core/etc/LogisticReg/l4c.cfg";
  const char *conf_file_path = conf;
  const char *trainingset_path = input;
  const char *encoding_type = "utf-8";

  LogisticReg::init(log_conf_path);
  L4C_INFO("Logistic regression training starts!");

  LogisticReg::LrPara *lrpara = new LogisticReg::LrPara(conf_file_path, encoding_type);
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
  delete ts; ts = NULL;
  delete lrpara; lrpara = NULL;

  return flag;
}
