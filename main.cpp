#include "parser.h"
#include "iohandler.h"
#include "runner.h"
#include "dirhandler.h"
#include "jobshandler.h"

#include <string>

#define KEY_RETURN 10

using namespace std;

/**
 * This is the main Chew program.
 */
int main()
{		
	ioHandler.start();
	jobsHandler.init();
	
	int key;

	while (1)
	{
		while (!jobsHandler.isMainForeground())
			usleep(5);
		
		/// Starts the command iteration, reseting everything.
		string path;
		if (!dirHandler.getWorkingPath(path))
			path = "ERROR$";
		vector<string> itHistory = ioHandler.startIteration(path);
		key = 0;
		
		/// Gets keys until the RETURN is typed.
		while (key != KEY_RETURN)
		{
			bool specialChar;
			key = ioHandler.readKey(specialChar);
			
			if (key == KEY_RETURN)
				continue;
			
			/// Handles the keys properly.
			ioHandler.handleKey(itHistory, key, specialChar);
		}
		
		/// Ends the iteration, saving everything done.
		ioHandler.endIteration(itHistory);
			
		/// Parses the line typed commands.
		vector<Command> commands;
		string line = itHistory[ioHandler.getHistoryIndex()];
		bool background = parser.parseLine(itHistory[ioHandler.getHistoryIndex()], commands);
		
		if (commands.size() > 0)
			runner.runChain(line, commands, ioHandler, dirHandler, jobsHandler, background);
		
#ifdef DEBUG_PRINT
		for (int i = 0; i < (int) commands.size(); ++i)
			commands[i].print();
#endif
	}
	
	ioHandler.end();
	
	return 0;
}

