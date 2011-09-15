#include "runner.h"

#include <unistd.h>
#include <cstdio>
#include <sys/stat.h>
#include <fcntl.h>
#include <cerrno>
#include <cstring>
#include <ncurses.h>
#include <sys/wait.h>
#include <cstdlib>

#define STDIN 0
#define STDOUT 1

Runner::Runner()
{

}

Runner::~Runner()
{

}

// TODO: handle errors
int Runner::run(Command& cmd, IOHandler& ioHandler)
{
	int infd = STDIN;
	int outfd = STDOUT;
	
	if (cmd.getIn().size() > 0)
	{
		infd = open(cmd.getIn().c_str(), O_RDONLY);
		dup2(infd, STDIN);
	}
	if (cmd.getOut().size() > 0)
	{
		outfd = open(cmd.getOut().c_str(), O_WRONLY | O_CREAT);
		dup2(outfd, STDOUT);
	}
	
	ioHandler.disable();
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
		
	//ioHandler.enable();
		
	return 0;
}
