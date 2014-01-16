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
#include <libxml/parser.h>
#include "algorithm.h"
#include "log4c.h"

class Conf {
  public:
    Conf(const char *conf_file_path, const char *encoding_type);
    ~Conf();

    bool parse(algorithm::Para *para);

  private:
    const char *conf_file_path;
    const char *encoding_type;
    xmlParserCtxtPtr ctxt;
    xmlDocPtr doc;
    xmlNodePtr cur;
};
#endif //STRATEGY_CORE_COMMON_CONF_H_
