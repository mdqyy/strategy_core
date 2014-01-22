//==============================================================================
/*!
 *  \file common.h
 *
 *  \brief common file
 *
 *  \author chenqinyu
 *          contact:
 *              superthierry14@gmail.com
 *  \date 2013-07
 *
 *  \par
 */
//==============================================================================
#ifndef STRATEGY_CORE_COMMON_COMMON_H_
#define STRATEGY_CORE_COMMON_COMMON_H_

#include "log4c.h"
#include <log4cxx/logger.h>
#include <log4cxx/logstring.h>
#include <log4cxx/propertyconfigurator.h>

namespace Log {
PropertyConfigurator::configure("conf.log");

LoggerPtr logger1 = Logger::getLogger("1");

LOG4CXX_TRACE(logger1, "trace\n");
LOG4CXX_WARN(logger1, "warn\n");
LOG4CXX_DEBUG(logger1, "debug\n");
LOG4CXX_ASSERT(logger1, false, "assert\n");
LOG4CXX_FATAL(logger1, "fatal\n");
}
#endif //STRATEGY_CORE_COMMON_COMMON_H_
