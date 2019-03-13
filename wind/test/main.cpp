//#include "mysql.h"
//#include <string>
//#include "mysql_test.h"
//#include "logger.h"
#include "easylogging++.h"
using namespace std;

namespace el 
{ 
	namespace base 
	{ 
		el::base::type::StoragePointer elStorage(new el::base::Storage(el::LogBuilderPtr(new el::base::DefaultLogBuilder()))); 
	} 
	el::base::debug::CrashHandler elCrashHandler(true); 
}

int main()
{
	el::Loggers::addFlag(el::LoggingFlag::StrictLogFileSizeCheck);

	el::Configurations conf("test_conf.conf");

	/// ���Ե�������ĳһ��logger������  
	el::Loggers::reconfigureLogger("default", conf);

	/// Ҳ��������ȫ��logger������  
	el::Loggers::reconfigureAllLoggers(conf);

	el::base::Writer(el::Level::Info, "d:\\c++code\\wind\\test\\main.cpp", 12, __FUNCSIG__, el::base::DispatchAction::NormalLog).construct(1, el::base::consts::kDefaultLoggerId) 
		<< "My first info log using default logger.";

	LOG(INFO) << "My first info log using default logger.";

	//CMysqlTest::Instance()->Test();
	return 0;
}