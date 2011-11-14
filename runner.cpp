#include "runner.h"

#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <sstream>

#define STDIN 0
#define STDOUT 1

using namespace std;

Runner runner;

/**
 * Constructor.
 */
Runner::Runner()
{
	one = ".test1"; /// pipes
	two = ".test2";
}

/**
 * Destructor.
 */
Runner::~Runner()
{

}

/**
 * Runs a chain of commands, that the user typed on the input.
 * @param line The line typed.
 * @param commands The vector of commands from the parser object.
 * @param ioHandler The iohandler object for printing purposes.
 * @param dirHandler The dirhandler object for dir purposes.
 * @param jobsHandler The jobshandler object for jobs purposes.
 * @param background If the chain must be executed in background.
 */
void Runner::runChain(string line, vector<Command>& commands, IOHandler& ioHandler, DirHandler& dirHandler, JobsHandler& jobsHandler, bool background)
{
	Command first = commands[0];
	if (first.getCmd() == "exit")
	{
		exit(0);
	}
	else if (first.getCmd() == "pwd")
	{
		string dir;
		if (dirHandler.getCurrentPath(dir))
		{
			dir += "\n";
			ioHandler.print(dir.c_str());
		}
		else
			ioHandler.print("ERROR");
	}
	else if (first.getCmd() == "cd")
	{
		dirHandler.setDir(first.getFirstParam());
	}
	else if (first.getCmd() == "jobs")
	{
		jobsHandler.showJobs(ioHandler);
	}
	else if (first.getCmd() == "fg")
	{
		unsigned int job = 0;
		if (first.getParams().size() >= 1)
		{
			stringstream ss;
			ss << first.getParams()[0];
			ss >> job;

			jobsHandler.setJobState(job, JOB_FOREGROUND);
		}
	}
	else if (first.getCmd() == "bg")
	{
		unsigned int job = 0;
		if (first.getParams().size() >= 1)
		{
			stringstream ss;
			ss << first.getParams()[0];
			ss >> job;

			jobsHandler.setJobState(job, JOB_BACKGROUND);
		}
	}
	else if (first.getCmd() == "kill" && first.getParams().size() >= 1 && first.getParams()[0].find("%") == 0)
	{
		unsigned int job = 0;
		stringstream ss;
		ss << first.getParams()[0].substr(1);
		ss >> job;
		jobsHandler.removeJob(job);
	}
	else
	{		
		int pid = fork();
		if (pid != 0)
		{	
			jobsHandler.addJob(line, pid, background ? JOB_BACKGROUND : JOB_FOREGROUND);
		}
		else
		{
			for (int i = 0; i < (int) commands.size(); ++i)
				run(commands[i], ioHandler, dirHandler, jobsHandler, i == 0, i == (int) commands.size()-1);			
		
			jobsHandler.setMainForeground();
			
			exit(0);
		}
	}
}

/**
 * Runs a chain of commands, that the user typed on the input.
 * @param cmd The command object to be executed.
 * @param ioHandler The iohandler object for printing purposes.
 * @param dirHandler The dirhandler object for dir purposes.
 * @param jobsHandler The jobshandler object for jobs purposes.
 * @param first If its the first command in the chain, for pipe control.
 * @param last If its the last command in the chain, for pipe control.
 * @return An integer, 0 on success.
 */
int Runner::run(Command& cmd, IOHandler& ioHandler, DirHandler& dirHandler, JobsHandler& jobsHandler, bool first, bool last)
{
	int infd = STDIN;
	int outfd = STDOUT;
	int stdinbak = dup(STDIN);
	int stdoutbak = dup(STDOUT);

	if (cmd.getIn().size() > 0)
	{
		infd = open(cmd.getIn().c_str(), O_RDONLY);
		dup2(infd, STDIN);
	}
	if (cmd.getOut().size() > 0)
	{
		int flags = O_CREAT | O_WRONLY;
		if (cmd.isAppend()) 
			flags |= O_APPEND;
		outfd = open(cmd.getOut().c_str(), flags, S_IRUSR | S_IWUSR | S_IRGRP);
		dup2(outfd, STDOUT);
	}
	
	swap(one, two); /// pipes
	if (infd == STDIN && !first)
	{
		infd = open(one, O_RDONLY);
		dup2(infd, STDIN);
	}
	if (outfd == STDOUT && !last)
	{
		outfd = open(two, O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR | S_IRGRP);
		dup2(outfd, STDOUT);
	}
	
	int pid = fork();
	if (pid == 0)
	{
		execvp(cmd.getCmd().c_str(), (char* const*) cmd.buildArgs());
		cmd.destroyArgs();
		exit(-1);
	}
	
	jobsHandler.setupJob();
	
	do
	{
		wait(NULL);
	}
	/// do while the process of execvp exists.
	while (getpgid(pid) != -1);

	if (infd != STDIN)
		close(infd);
	if (outfd != STDOUT)
		close(outfd);
		
	dup2(stdinbak, STDIN);
	dup2(stdoutbak, STDOUT);

	return 0;
}

