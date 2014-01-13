//==============================================================================
/*!
 *  \file common.cpp
 *
 *  \brief common file
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

#include <strategy_core/common/conf.h>

Conf::Conf(const char *conf_file_path, const char *encoding_type) {
  this->conf_file_path = conf_file_path;
  this->encoding_type = encoding_type;
}

Conf:parse(void *para) {
  bool flag = true;

  if (NULL == this->conf_file_path) {
    L4C_WARN("Configure file pointer is null. Algorithms will use default values!");
    flag = false;
    goto end;
  }

  /// Create a parser
  this->ctxt = xmlNewParserCtxt();
  if (NULL == this->ctxt) {
    L4C_ERROR("Can't create xml parser. Algorithms will use default values!");
    flag = false;
    goto end;
  }

  /// Read iostream
  this->doc = xmlCtxtReadFile(this->ctxt, this->conf_file_path, this->encoding, XML_PARSE_DTDATTR|XML_PARSE_NOERROR);
  if (NULL == doc) {
    L4C_ERROR("Can't parse the content in %s! Algorithms will use default values!", this->conf_file_path);
    flag = false;
    goto end;
  }

  /// get root node
  curr = xmlDocGetRootElement(doc);
  if (NULL == curr) {
    L4C_ERROR("Can't get the root element in %s!", this->conf_file_path);
    flag = false;
    goto end;
  }

  /// get child node
  while (NULL != curr) {
    if (!xmlStrcmp(curr->name, (const xmlChar *)"news")){
      xmlChar *key;
      xmlNodePtr l_cur = curr->xmlChildrenNode;
      while (NULL != l_cur) {
        if (!xmlStrcmp(l_cur->name, (const xmlChar *)"title")) {
          key = xmlNodeListGetString(doc, l_cur->xmlChildrenNode, 1);
          printf("title: %s\n", key);
          xmlFree(key);
        }
        else if (!xmlStrcmp(l_cur->name, (const xmlChar *)"content")) {
          key = xmlNodeListGetString(doc, l_cur->xmlChildrenNode, 1);
          printf("content: %s\n", key);
          xmlFree(key);
        }
        l_cur = l_cur->next;
      }
    }
    curr = curr->next;
  }

end:
  xmlFreeDoc(this->doc);
  xmlFreeParserCtxt(this->ctxt);
  xmlCleanupParser();
  return flag;
}
