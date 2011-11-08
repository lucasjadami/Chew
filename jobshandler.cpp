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
#include <cstdio> /// TODO: remove.
#include <errno.h> /// TODO: remove.

/** The jobs handler. */
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
			/// Checks if the child has stopped and sends a stop signal to itself so the main process can catch it.
			/// Note that the kill/end signals does not need to be sent (they are already sent).
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

/**
 * @return The main pid.
 */
int JobsHandler::getMainPid()
{
	return mainPid;
}

/**
 * @param job The job index.
 * @return The job pid.
 */
int JobsHandler::getJobPid(unsigned int job)
{
	if (job >= jobs.size())
		return -1;
	return jobs[job].getPid();
}

/**
 * @return True if the main process owns the foreground.
 */
bool JobsHandler::isMainForeground()
{
	return tcgetpgrp(terminalFd) == mainPid;
}

/**
 * When a process calls this, it setups the signals as it is a job.
 * @return True on success.
 */
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

/**
 * @param job The job index to remove.
 * @return True on success.
 */
bool JobsHandler::removeJob(unsigned int job)
{
	if (job >= jobs.size())
		return false;
	kill(jobs[job].getPid(), SIGKILL);
	jobs.erase(jobs.begin() + job);
	return true;
}

/**
 * @param pid The job pid to remove.
 * @return True on success.
 */
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

/**
 * @param line The job line.
 * @param pid The job pid.
 * @param state The job state.
 * @return True on success.
 */
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

/**
 * @param job The job index.
 * @parma state The job state.
 * @return True on success.
 */
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

/**
 * This method does not take further actions to set the environment for the job!
 * @param pid The job pid.
 * @param state The job state.
 * @return True on success.
 */
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

/**
 * Sets the main process as the controller of the foreground.
 * @return True on success.
 */
bool JobsHandler::setMainForeground()
{
	/// Allways check if the calling process owns the terminal, or it will get a SIGTTOU signal.
	if (tcgetpgrp(terminalFd) == getpid())
		return tcsetpgrp(terminalFd, mainPid) == 0;
	return false;
}

/**
 * Inits the handler.
 * @return True on success.
 */
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

/**
 * Lists all the jobs.
 * @param ioHandler The I/O handler.
 */
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
