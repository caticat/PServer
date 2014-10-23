#include "config.h"
#include "plua.h"

bool Config::load()
{
	boost::shared_ptr<PLua> pt(new PLua("config.lua"));
	pt->GetVal("scriptPath", scriptPath);
	pt->GetVal("logPath", logPath);
	return true;
}
