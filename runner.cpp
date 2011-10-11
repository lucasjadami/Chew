#include "runner.h"

#include <unistd.h>
#include <fcntl.h>
#include <cstdio>
#include <sys/wait.h>
#include <stdlib.h>

#define STDIN 0
#define STDOUT 1

Runner::Runner()
{
	one = ".test1";
	two = ".test2";
}

Runner::~Runner()
{

}

// TODO: handle errors
int Runner::run(Command& cmd, IOHandler& ioHandler, bool first, bool last)
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

