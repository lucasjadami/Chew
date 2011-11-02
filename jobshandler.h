#ifndef JOBSHANDLER_H
#define JOBSHANDLER_H

#include "iohandler.h"

#include <unistd.h>
#include <signal.h>
#include <vector>

#define JOB_BACKGROUND 0
#define JOB_FOREGROUND 1

using namespace std;

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

	string line;
	int pid;
	int state;

};

class JobsHandler
{

public:

	bool handleStop();
	bool handleInterrupt();
	bool init(void (*)(int, siginfo_t*, void*));
	bool removeJob(unsigned int);
	bool removeJobAsPid(int);
	void showJobs(IOHandler&);
	void addJob(string, int, int);

private:

	vector<Job> jobs;

};

#endif
