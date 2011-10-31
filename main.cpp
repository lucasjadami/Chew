#include "parser.h"
#include "iohandler.h"
#include "runner.h"
#include "dirhandler.h"
#include "jobshandler.h"

#include <signal.h>
#include <cstdlib>
#include <unistd.h>
#include <string>

#define KEY_RETURN 10

using namespace std;

IOHandler ioHandler;
Parser parser;
Runner runner;
DirHandler dirHandler;
JobsHandler jobsHandler;

void sigHandler(int signum)
{
    if (signum == SIGINT)
    {
    	if (!jobsHandler.handleInterrupt())
    		exit(0);
    }
    else if (signum == SIGTSTP)
    {
    	if (!jobsHandler.handleStop())
    	{
    		// TODO stop it
    	}
    }
}

int main()
{	
	signal(SIGINT, sigHandler);
	signal(SIGTSTP, sigHandler);
		
	// starts io
	ioHandler.start();
	
	int key;

	while (1)
	{
		// starts the command iteration, reseting everything
		string path;
		if (!dirHandler.getWorkingPath(path))
			path = "ERROR$";
		vector<string> itHistory = ioHandler.startIteration(path);
		key = 0;
		
		// gets keys until the RETURN is typed
		while (key != KEY_RETURN)
		{
			bool specialChar;
			key = ioHandler.readKey(specialChar);
			
			if (key == KEY_RETURN)
				continue;
			
			// handles the keys properly
			ioHandler.handleKey(itHistory, key, specialChar);
		}
		
		// ends the iteration, saving everything done.
		ioHandler.endIteration(itHistory);
			
		// parses the line typed commands
		vector<Command> commands;
		parser.parseLine(itHistory[ioHandler.getHistoryIndex()], commands);
		
		// TODO 'jobs' 'fg' & 'bg' handling
		for (int i = 0; i < (int) commands.size(); ++i)
			runner.run(commands[i], ioHandler, dirHandler, i == 0, i == (int) commands.size()-1);
		
#ifdef DEBUG_PRINT
		for (int i = 0; i < (int) commands.size(); ++i)
			commands[i].print();
#endif
	}
	
	// ends io
	ioHandler.end();
	
	return 0;
}

