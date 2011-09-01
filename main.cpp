#include "parser.h"

#include <vector>
#include <string>
#include <ncurses.h>

#define KEY_RETURN 10
#define KEY_SPACE 32

using namespace std;

vector<Command> commands;
Parser parser;

void insertChar(string&, int&, int);

int main()
{		
	// init ncurses
	initscr();
	// ncurses -> handling special keys
	keypad(stdscr, TRUE);
	// ncurses -> no echo
	noecho();
	
	// 0 means the end of the command line
	int cursorPos;

	while (1)
	{
		printw("> ");
		commands.clear();
		cursorPos = 0;
		
		// gets the line typed by the user
		string in;
		
		int ch = 0;
		while (ch != KEY_RETURN)
		{
			ch = getch();
			
			if (ch == KEY_RETURN)
				continue;
			
			insertChar(in, cursorPos, ch);
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

void insertChar(string& in, int& cursorPos, int ch)
{
	int oldSize = in.size();
	
	if (ch == KEY_BACKSPACE)
	{
		if (in.size() > 0 && (int) in.size() + cursorPos > -1)
		{
			in.erase(in.end() + cursorPos - 1);
		}
	}
	else			
		in.insert(in.end() + cursorPos, ch);
		
	int x, y, xMax, yMax, xCur, yCur;
	getyx(stdscr, y, x);
	getmaxyx(stdscr, yMax, xMax);
	xCur = x;
	yCur = y;
		
	while (oldSize-- >= 0)
	{
		if (x == 0)
		{
			x = xMax;
			y--;
		}
		
		move(y, --x);
		addch(KEY_SPACE);
	}
	
	printw("%s", in.c_str());
}

