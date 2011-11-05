#include "jobshandler.h"

#include <string.h>
#include <sys/types.h>
#include <signal.h>
#include <sstream>
#include <cstdlib>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <cstdio> /// TODO remove
#include <errno.h> /// TODO remove

JobsHandler jobsHandler;

void handleSignal(int sigNum, siginfo_t* sigInfo, void* context)
{
	if (sigNum == SIGTSTP || sigNum == SIGINT)
	{
		jobsHandler.setMainForeground();
	}
	else if (sigNum == SIGCHLD)
	{
		if (getpid() != jobsHandler.getMainPid())
		{
			if (sigInfo->si_code == CLD_STOPPED)
				kill(getpid(), SIGSTOP);
		}
		else
		{	
			if (sigInfo->si_code == CLD_STOPPED)
				jobsHandler.setJobStateByPid(sigInfo->si_pid, JOB_STOPPED);
			else if (sigInfo->si_code == CLD_EXITED || sigInfo->si_code == CLD_KILLED || sigInfo->si_code == CLD_DUMPED)
				jobsHandler.removeJobByPid(sigInfo->si_pid);	
		}
	}
}

int JobsHandler::getMainPid()
{
	return mainPid;
}

int JobsHandler::getJobPid(unsigned int job)
{
	if (job >= jobs.size())
		return -1;
	return jobs[job].getPid();
}

bool JobsHandler::isMainForeground()
{
	return tcgetpgrp(terminalFd) == mainPid;
}

bool JobsHandler::setupJob()
{
	struct sigaction action;
	memset (&action, '\0', sizeof(action));
	action.sa_sigaction = handleSignal;
	action.sa_flags = SA_SIGINFO;
	
	if (sigaction(SIGTSTP, &action, NULL) < 0 || sigaction(SIGINT, &action, NULL) < 0) 
		return false;

	return true;
}

bool JobsHandler::removeJob(unsigned int job)
{
	if (job >= jobs.size())
		return false;
	kill(jobs[job].getPid(), SIGKILL);
	jobs.erase(jobs.begin() + job);
	return true;
}

bool JobsHandler::removeJobByPid(int pid)
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

bool JobsHandler::addJob(string line, int pid, int state)
{
	jobs.push_back(Job(line, pid, state));
	
	setpgid(pid, pid);
	
	if (state == JOB_FOREGROUND)
	{
		return tcsetpgrp(terminalFd, pid) == 0;
	}
	
	return true;
}

bool JobsHandler::setJobState(unsigned int job, int state)
{
	if (job >= jobs.size())
		return false;

	if (state == JOB_FOREGROUND)
	{
		if (killpg(jobs[job].getPid(), SIGCONT) != 0)
			return false;
		if (tcsetpgrp(terminalFd, jobs[job].getPid()) != 0)
		{
			return false;
		}
	}
	else if (state == JOB_BACKGROUND)
	{
		if (killpg(jobs[job].getPid(), SIGCONT) != 0)
			return false;
	}
	jobs[job].setState(state);
	return true;
}

bool JobsHandler::setJobStateByPid(int pid, int state)
{
	for (unsigned int i = 0; i < jobs.size(); ++i)
	{
		if (jobs[i].getPid() == pid)
		{
			jobs[i].setState(state);
			return true;
		}
	}
	
	return false;
}

bool JobsHandler::setMainForeground()
{
	/// allways check if the current process owns the terminal, or it will get a SIGTTOU signal
	if (tcgetpgrp(terminalFd) == getpid())
		return tcsetpgrp(terminalFd, mainPid) == 0;
	return false;
}

bool JobsHandler::init()
{
	if ((terminalFd = open(ctermid(NULL), O_RDONLY)) == -1)
		return false;
		
	mainPid = getpid();
	
	struct sigaction action;
	memset (&action, '\0', sizeof(action));
	action.sa_sigaction = handleSignal;
	action.sa_flags = SA_SIGINFO;
	
	if (sigaction(SIGCHLD, &action, NULL) < 0) 
		return false;
	
	return true;
}

void JobsHandler::showJobs(IOHandler& ioHandler)
{
	for (unsigned int i = 0; i < jobs.size(); ++i)
	{
		stringstream ss;
		ss << "[";
		ss << i;
		ss << "] ";
		switch (jobs[i].getState())
		{
			case JOB_FOREGROUND:
				ss << "FG"; break;
			case JOB_BACKGROUND:
				ss << "BG"; break;
			case JOB_STOPPED:
				ss << "STP"; break;
		}
		ss << " ";
		ss << jobs[i].getLine().c_str();
		ss << "\n";
		ioHandler.print(ss.str().c_str());
	}
}
