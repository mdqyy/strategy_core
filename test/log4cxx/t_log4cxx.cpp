#include <log4cxx/logger.h>    
#include <log4cxx/logstring.h> 
#include <log4cxx/propertyconfigurator.h>

int main(int argc, char* argv[]) { 
	using namespace log4cxx;

  PropertyConfigurator::configure("conf.log");

  LoggerPtr logger1 = Logger::getLogger();
  //LoggerPtr logger2 = Logger::getLogger("2");
	
  LOG4CXX_TRACE(logger1, "trace"); 
	/*
  LOG4CXX_WARN(logger1, "warn\n"); 
  LOG4CXX_DEBUG(logger1, "debug\n"); 
  LOG4CXX_ASSERT(logger1, false, "assert\n"); 
  LOG4CXX_FATAL(logger1, "fatal\n");

  LOG4CXX_TRACE(logger2, "trace\n"); 
  LOG4CXX_ERROR(logger2, "error\n");
	*/
  return 0; 
}

