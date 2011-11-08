#ifndef JOBSHANDLER_H
#define JOBSHANDLER_H

#include "iohandler.h"

#include <unistd.h>
#include <signal.h>
#include <vector>

#define JOB_BACKGROUND 0
#define JOB_FOREGROUND 1
#define JOB_STOPPED 2

using namespace std;

/**
 * The job class represents a chain running.
 */
class Job
{

public:

	Job(string line, int pid, int state)
	{
		this->line = line;
		this->pid = pid;
		this->state = state;
	}
	
	string getLine() { return line; }
	int getPid() { return pid; }
	int getState() { return state; }
	void setState(int state) { this->state = state; }

private:

	/** The line beeing executed. */
	string line;
	/** The pid (it is also the gpid). */
	int pid;
	/** State of the job. */
	int state;

};

/**
 * Handles the job part.
 */
class JobsHandler
{

public:

	int getMainPid();
	int getJobPid(unsigned int);
	bool isMainForeground();
	bool setupJob();
	bool removeJob(unsigned int);
	bool removeJobByPid(int);
	bool addJob(string, int, int);
	bool setJobState(unsigned int, int);
	bool setJobStateByPid(int, int);
	bool setMainForeground();
	bool init();
	void showJobs(IOHandler&);

private:

	/** Jobs created. */
	vector<Job> jobs;
	/* Main pid. */
	int mainPid;
	/** The terminal file descriptor. */
	int terminalFd;
};

extern JobsHandler jobsHandler;

#endif
