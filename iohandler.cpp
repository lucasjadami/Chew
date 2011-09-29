#include "iohandler.h"

#include <ncurses.h>

#define KEY_SPACE 32

#ifdef DEBUG_PRINT
void IOHandler::debugPrint(const char* s)
{
	printw("\n%s\n", s);
}
#endif

vector<string> IOHandler::startIteration(string path)
{
	printw("[CHEW] %s ", path.c_str());
	cursorPos = 0;
	historyIndex = 0;
	return createIterationHistory();
}

bool IOHandler::endIteration(vector<string>& itHistory)
{
	addch('\n');
	
	if (itHistory[historyIndex] == "exit")
		return true;
		
	// adds the current itHistory command to the beginning, because it is the most recent command
	commandsHistory.insert(commandsHistory.begin(), itHistory[historyIndex]);
	
	int i = historyIndex == -1 ? 0 : 1;
	// merges the itHistory on the commandsHistory
	while (i != itHistory.size())
	{
		// do not merges the command on the position of the most recent command (LINUX behavior)
		if (i != historyIndex)
		{
			commandsHistory[i] = itHistory[i];
		}
		
		++i;
	}
	
	// checks if the command is an empty one, to remove it
	if (commandsHistory[0].size() == 0)
		commandsHistory.erase(commandsHistory.begin());
	
	return false;
}

int IOHandler::getHistoryIndex()
{
	return historyIndex;
}

int IOHandler::readKey()
{
	return getch();
}

void IOHandler::handleKey(vector<string>& itHistory, int key)
{
	int x, y, xMax, yMax;
	getyx(stdscr, y, x);
	getmaxyx(stdscr, yMax, xMax);
	
	// moves to the end of the command line
	int xEx = x - cursorPos;
	move(y, xEx);
	
	int size = itHistory[historyIndex].size();
	
	// the move back is the movement of the cursor after the printw
	bool disableMoveBack = false;
		
	// backforwards adding empty spaces until the command beggining is reached
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
	
	// does the move back movement
	if (!disableMoveBack)
		move(y, x);
}

void IOHandler::start()
{
	// init ncurses
	initscr();
	// ncurses -> handling special keys
	keypad(stdscr, TRUE);
	// ncurses -> no echo
	noecho();
	// ncurses -> set to true so the window will scroll
	scrollok(stdscr, TRUE);
}

void IOHandler::refreshIt()
{
	// ncurses -> updates the screen
	refresh();
}

void IOHandler::end()
{
	// end ncurses
	endwin();
}

void IOHandler::print(const char* s)
{
	printw("%s", s);
}

vector<string> IOHandler::createIterationHistory()
{
	vector<string> itHistory = commandsHistory;
	itHistory.insert(itHistory.begin(), string());
	return itHistory;
}

