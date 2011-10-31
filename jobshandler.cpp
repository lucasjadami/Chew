#include "jobshandler.h"

#include <string.h>

bool JobsHandler::handleStop()
{
	return false;
}

bool JobsHandler::handleInterrupt()
{
	return false;
}

void JobsHandler::showJobs(IOHandler& ioHandler)
{

}

bool JobsHandler::init(void (*handler)(int, siginfo_t*, void*) )
{
	struct sigaction act;
	memset (&act, '\0', sizeof(act));
	act.sa_sigaction = handler;
	act.sa_flags = SA_SIGINFO;
	
	if (sigaction(SIGINT, &act, NULL) < 0 || sigaction(SIGTSTP, &act, NULL) < 0 || sigaction(SIGCHLD, &act, NULL) < 0) 
	{
		return false;
	}
	return true;
}
