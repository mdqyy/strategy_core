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

using namespace std;

int main (int argc, char **argv) {
  int flag = 0;
///for (INT i = 0; i < 1; i++) { /// test memory leak
  const char *log_conf_path = "/home/cqy/work/strategy_core/etc/LogisticReg/l4c.cfg";
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
///}
  return flag;
}
