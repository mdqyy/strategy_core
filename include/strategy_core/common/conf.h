//==============================================================================
/*!
 *  \file conf.h
 *
 *  \brief configure header
 *
 *  \author chenqinyu
 *          contact:
 *              superthierry14@gmail.com
 *  \date 2013-07
 *
 *  \par
 */
//==============================================================================
#ifndef STRATEGY_CORE_COMMON_CONF_H_
#define STRATEGY_CORE_COMMON_CONF_H_

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <string>
#include "algorithm.h"

class Conf {
  public:
    Conf(const char *conf_file_path, const char *encoding_type);
    ~Conf();

    bool parse();

  private:
    const char *conf_file_path;
    const char *encoding_type;
};
#endif //STRATEGY_CORE_COMMON_CONF_H_
