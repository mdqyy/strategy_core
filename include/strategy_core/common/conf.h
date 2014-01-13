//==============================================================================
/*!
 *  \file conf.h
 *
 *  \brief configure header
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
#ifndef STRATEGY_CORE_COMMON_CONF_H_
#define STRATEGY_CORE_COMMON_CONF_H_

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <string>
#include <libxml/parser.h>
#include "log4c.h"

class Conf {
  public:
    Conf(const char *conf_file_path, const char *encoding_type);
    ~Conf();

    bool parse();

  private:
    char *conf_file_path;
    char *encoding_type;
    xmlParserCtxtPtr ctxt;
    xmlDocPtr doc;
    xmlNodePtr cur;
}
#endif //STRATEGY_CORE_COMMON_CONF_H_
