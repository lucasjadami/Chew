#include "jobshandler.h"

#include <string.h>
#include <sys/types.h>
#include <signal.h>
#include <sstream>

bool JobsHandler::handleStop()
{
	for (unsigned int i = 0; i < jobs.size(); ++i)
	{
		if (jobs[i].getState() == JOB_FOREGROUND)
		{
			kill(jobs[i].getPid(), SIGTSTP);
			jobs[i].setState(JOB_BACKGROUND);
			return true;
		}
	}
	return false;
}

bool JobsHandler::handleInterrupt()
{
	for (unsigned int i = 0; i < jobs.size(); ++i)
	{
		if (jobs[i].getState() == JOB_FOREGROUND)
		{
			kill(jobs[i].getPid(), SIGKILL);
			jobs.erase(jobs.begin() + i);
			return true;
		}
	}
	return false;
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

bool JobsHandler::removeJob(unsigned int index)
{
	if (index >= jobs.size())
		return false;
	kill(jobs[index].getPid(), SIGKILL);
	jobs.erase(jobs.begin() + index);
	return true;
}

bool JobsHandler::removeJobAsPid(int pid)
{
	for (unsigned int i = 0; i < jobs.size(); ++i)
	{
		if (jobs[i].getPid() == pid)
		{
			jobs.erase(jobs.begin() + i);
			return true;
		}
	}
	return false;
}

void JobsHandler::showJobs(IOHandler& ioHandler)
{
	for (unsigned int i = 0; i < jobs.size(); ++i)
	{
		stringstream ss;
		ss << "[";
		ss << i;
		ss << "] ";
		ss << jobs[i].getState();
		ss << " ";
		ss << jobs[i].getLine().c_str();
		ss << "\n";
		ioHandler.print(ss.str().c_str());
	}
}

void JobsHandler::addJob(string line, int pid, int state)
{
	jobs.push_back(Job(line, pid, state));
}
