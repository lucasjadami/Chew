#include "parser.h"

#include <vector>
#include <string>
#include <ncurses.h>

#define KEY_RETURN 10

using namespace std;

vector<Command> commands;
Parser parser;

int main()
{		
	// init ncurses
	initscr();
	// ncurses -> handling special keys
	keypad(stdscr, TRUE);
	// ncurses -> no echo
	noecho();
	
	while (1)
	{
		printw("> ");
		commands.clear();
		
		// gets the line typed by the user
		string in;
		
		int ch = 0;
		while (ch != KEY_RETURN)
		{
			ch = getch();
			
			if (ch == KEY_RETURN)
				continue;
				
			addch(ch);
			in += ch;
		}
		
		addch('\n');
		
		if (in == "exit")
			break;
			
		// parses the line typed commands
		parser.parseLine(in, commands);
		
#ifdef DEBUG_PRINT
		for (int i = 0; i < commands.size(); ++i)
			commands[i].print();
#endif
		// ncurses -> updates the screen
		refresh();
	}
	
	// end ncurses
	endwin();
	
	return 0;
}

