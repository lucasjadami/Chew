#include "parser.h"

#include <vector>
#include <string>
#include <ncurses.h>

#define KEY_RETURN 10
#define KEY_SPACE 32

using namespace std;

vector<Command> commands;
vector<string> commandsHistory;
Parser parser;

vector<string> createIterationHistory();
void mergeCommandsHistory(vector<string>&, int);
void handleKey(vector<string>&, int&, int&, int);

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
	int historyIndex;
	int key;

	while (1)
	{
		printw("> ");
		commands.clear();
		cursorPos = 0;
		historyIndex = 0;
		key = 0;
		
		vector<string> itHistory = createIterationHistory();
		
		while (key != KEY_RETURN)
		{
			key = getch();
			
			if (key == KEY_RETURN)
				continue;
			
			handleKey(itHistory, historyIndex, cursorPos, key);
		}
		
		addch('\n');
		
		if (itHistory[historyIndex] == "exit")
			break;
			
		// parses the line typed commands
		parser.parseLine(itHistory[historyIndex], commands);
		
		mergeCommandsHistory(itHistory, historyIndex);
		
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

vector<string> createIterationHistory()
{
	vector<string> itHistory = commandsHistory;
	itHistory.insert(itHistory.begin(), string());
	return itHistory;
}

void mergeCommandsHistory(vector<string>& itHistory, int historyIndex)
{
	commandsHistory.insert(commandsHistory.begin(), itHistory[historyIndex]);
	
	int i = historyIndex == -1 ? 0 : 1;
	while (i != itHistory.size())
	{
		if (i != historyIndex)
		{
			commandsHistory[i] = itHistory[i];
		}
		
		++i;
	}
}

void handleKey(vector<string>& itHistory, int& historyIndex, int& cursorPos, int key)
{
	int x, y, xMax, yMax;
	getyx(stdscr, y, x);
	getmaxyx(stdscr, yMax, xMax);
	
	int xEx = x - cursorPos;
	move(y, xEx);
	
	int size = itHistory[historyIndex].size();
	
	// the move back is the movement of the cursor after the printw
	bool disableMoveBack = false;
		
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
		if (itHistory[historyIndex].size() > 0 && -cursorPos < itHistory[historyIndex].size())
		{
			itHistory[historyIndex].erase(itHistory[historyIndex].end() + cursorPos - 1);
			x--;
		}
	}
	else if (key == KEY_LEFT)
	{
		cursorPos--;
		if (-cursorPos > itHistory[historyIndex].size())
			cursorPos = -itHistory[historyIndex].size();
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
		historyIndex = historyIndex == itHistory.size() - 1 ? historyIndex : historyIndex + 1;
		cursorPos = 0;
		disableMoveBack = true;
	}
	else if (key == KEY_DOWN)
	{
		historyIndex = historyIndex == 0 ? historyIndex : historyIndex - 1;
		cursorPos = 0;
		disableMoveBack = true;
	}
	else
	{
		itHistory[historyIndex].insert(itHistory[historyIndex].end() + cursorPos, key);
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
	
	printw("%s", itHistory[historyIndex].c_str());
	
	// this is the move back movement
	if (!disableMoveBack)
		move(y, x);
}

