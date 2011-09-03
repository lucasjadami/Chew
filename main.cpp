#include "parser.h"

#include <vector>
#include <string>
#include <ncurses.h>

#define KEY_RETURN 10
#define KEY_SPACE 32

using namespace std;

vector<Command> commands;
vector<string> commandsHistory;
int historyIndex;
Parser parser;

void handleKey(string&, int&, int&, int);

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
		historyIndex = -1;
		cursorPos = 0;
		
		// gets the line typed by the user
		string in;
		
		int key = 0;
		while (key != KEY_RETURN)
		{
			key = getch();
			
			if (key == KEY_RETURN)
				continue;
			
			handleKey(in, cursorPos, historyIndex, key);
		}
		
		addch('\n');
		
		if (in == "exit")
			break;
			
		// parses the line typed commands
		parser.parseLine(in, commands);
		commandsHistory.insert(commandsHistory.begin(), in);
		
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

void handleKey(string& in, int& cursorPos, int& historyPos, int key)
{
	int x, y, xMax, yMax;
	getyx(stdscr, y, x);
	getmaxyx(stdscr, yMax, xMax);
	
	int xEx = x - cursorPos;
	move(y, xEx);
	
	int size = in.size();
		
	while (size-- >= 0)
	{
		if (xEx == 0)
		{
			xEx = xMax;
			y--;
		}
		
		move(y, --xEx);
		addch(KEY_SPACE);
	}
	
	if (key == KEY_BACKSPACE)
	{
		if (in.size() > 0 && -cursorPos < in.size())
		{
			in.erase(in.end() + cursorPos - 1);
			x--;
		}
	}
	else if (key == KEY_LEFT)
	{
		cursorPos--;
		if (-cursorPos > in.size())
			cursorPos = -in.size();
		else
			x--;
	}
	else if (key == KEY_RIGHT)
	{
		cursorPos++;
		if (cursorPos > 0)
			cursorPos = 0;
		else
			x++;
	}
	else if (key == KEY_UP)
	{
		// TODO
	}
	else if (key == KEY_DOWN)
	{
		// TODO
	}
	else
	{
		in.insert(in.end() + cursorPos, key);
		x++;
	}
	
	if (x < 0)
	{
		x = xMax;
		y--;
	}
	else if (x > xMax)
	{
		x = 0;
		y++;
	}
	
	printw("%s", in.c_str());
	
	move(y, x);
}

