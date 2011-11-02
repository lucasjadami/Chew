#include "runner.h"

#include <unistd.h>
#include <fcntl.h>
#include <cstdio>
#include <sys/wait.h>
#include <stdlib.h>
#include <sstream>

#define STDIN 0
#define STDOUT 1

using namespace std;

Runner::Runner()
{
	one = ".test1";
	two = ".test2";
}

Runner::~Runner()
{

}

void Runner::runChain(vector<Command>& commands, IOHandler& ioHandler, DirHandler& dirHandler, JobsHandler& jobsHandler, bool background)
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
			ioHandler.print(dir.c_str());
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
	else if (first.getCmd() == "kill")
	{
		unsigned int process = 0;
		if (first.getParams().size() >= 1)
		{
			stringstream ss;
			ss << first.getParams()[0];
			ss >> process;

			jobsHandler.removeJob(process); 
		}
	}
	else
	{
		int pid = fork();
		if (pid != 0)
		{
			string chainString;
			for (unsigned int i = 0; i < commands.size(); ++i)
				chainString += commands[0].getCmd();
			
			if (background)
			{
				jobsHandler.addJob(chainString, pid, JOB_BACKGROUND);
				return;
			}
			else
			{
				jobsHandler.addJob(chainString, pid, JOB_FOREGROUND);
				int status;
				waitpid(pid, &status, WUNTRACED);
				if (!WIFSIGNALED(status) || WTERMSIG(status) == SIGKILL)
					jobsHandler.removeJobAsPid(pid); 
			}
		}
		else
		{
			for (int i = 0; i < (int) commands.size(); ++i)
				run(commands[i], ioHandler, dirHandler, jobsHandler, i == 0, i == (int) commands.size()-1);
		
			exit(0);
		}
	}
}

// TODO: handle errors
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
		outfd = open(cmd.getOut().c_str(), O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR | S_IRGRP);
		dup2(outfd, STDOUT);
	}
	
	swap(one, two);
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
	wait(NULL);

	if (infd != STDIN)
		close(infd);
	if (outfd != STDOUT)
		close(outfd);
		
	dup2(stdinbak, STDIN);
	dup2(stdoutbak, STDOUT);

	return 0;
}

