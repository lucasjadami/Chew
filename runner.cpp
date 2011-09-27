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
	
	int tmpfd = -1;
	if (outfd == STDOUT)
		tmpfd = open("/tmp/chew", O_RDWR | O_CREAT);
	int pid = fork();
	
	if (pid == 0)
	{
		if (tmpfd != -1)
			dup2(tmpfd, STDOUT);
		char* const* args = (char* const*) cmd.buildArgs();
		execvp(cmd.getCmd().c_str(), args);
		cmd.destroyArgs();
		exit(-1);
	}
	
	int secondPid = fork();
	
	if (secondPid == 0)
	{
		char buffer[1001];
		int ret;
		while (ret = read(tmpfd, buffer, 1000) != EOF)
		{
			buffer[ret] = 0;
			printw("%s", buffer);
			refresh();
		}
		exit(-1);
	}
	else
	{
		waitpid(pid, NULL, 0);
		kill(secondPid, SIGKILL);
	}
	
	wait(NULL);
	
	if (tmpfd != -1)
		close(tmpfd);
	if (infd != STDIN)
		close(infd);
	if (outfd != STDOUT)
		close(outfd);

	//ioHandler.enable();
		
	return 0;
}

