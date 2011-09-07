#include "parser.h"
#include "iohandler.h"

#define KEY_RETURN 10

using namespace std;

IOHandler ioHandler;
Parser parser;

int main()
{		
	// starts io
	ioHandler.start();
	
	int key;

	while (1)
	{
		// starts the command iteration, reseting everything
		vector<string> itHistory = ioHandler.startIteration();
		key = 0;
		
		// gets keys until the RETURN is typed
		while (key != KEY_RETURN)
		{
			key = ioHandler.readKey();
			
			if (key == KEY_RETURN)
				continue;
			
			// handles the keys properly
			ioHandler.handleKey(itHistory, key);
		}
		
		// ends the iteration, saving everything done
		// if it returns true, it means the "exit" was typed on the iteration
		if (ioHandler.endIteration(itHistory))
			break;
			
		// parses the line typed commands
		vector<Command> commands;
		parser.parseLine(itHistory[ioHandler.getHistoryIndex()], commands);
		
#ifdef DEBUG_PRINT
		for (int i = 0; i < commands.size(); ++i)
			commands[i].print();
#endif
		// refreshes anything to be refreshed
		ioHandler.refresh();
	}
	
	// ends io
	ioHandler.end();
	
	return 0;
}

