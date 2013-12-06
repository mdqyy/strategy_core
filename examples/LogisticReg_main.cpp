//==============================================================================
/*!
*  \file BFGS_main.cpp
*
*  \brief main program of BFGS
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
#include <string>
#include <iostream>

using namespace std;

int main (int argc, char **argv) {
  int flag = 0;
///for (INT i = 0; i < 1; i++) { /// test memory leak
  const char *log_conf_path = "/home/cqy/work/strategy_core/etc/LogisticReg/l4c.cfg";
  const char *trainingset_path = "/home/cqy/work/strategy_core/data/logistic.sample";
  LogisticReg::init(log_conf_path);
  //LogisticReg::LrPara *lrpara = new LogisticReg::LrPara();
  L4C_INFO("Logistic regression training starts!");

  LogisticReg::TrainingSet *ts = new LogisticReg::TrainingSet(trainingset_path);
  if (!ts->load_header()) {
    L4C_FATAL("Loading header failed!");
    flag = -1;
    goto end;
  }
  //cout<<ts->sample_num<<' '<<ts->feature_num<<endl;

  if (!ts->load_sample()) {
    L4C_FATAL("Loading sample failed!");
    flag = -1;
    goto end;
  }
  /*
  if (!ts->train(ts, lrpara)) {
    L4C_FATAL("Training failed!");
    flag = -1;
    goto end;
  }
  */
end:
  delete ts; ts = NULL;
  //delete lrpara; lrpara = NULL;
  if (flag < 0) {
    L4C_INFO("Logistic regression training failed!");
  }
  else {
    L4C_INFO("Logistic regression training ends successfully!");
  }
///}
  return flag;
}