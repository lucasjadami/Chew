#include "iohandler.h"

#include <cstdio>
#include <termios.h>
#include <unistd.h>

#define KEY_SPACE 32
#define KEY_BACKSPACE 127
#define KEY_UP 65
#define KEY_DOWN 66
#define KEY_RIGHT 67
#define KEY_LEFT 68

#ifdef DEBUG_PRINT
void IOHandler::debugPrint(const char* s)
{
	printf("\n%s\n", s);
}
#endif

vector<string> IOHandler::startIteration(string path)
{
	printf("[CHEW] %s ", path.c_str());
	cursorPos = 0;
	historyIndex = 0;
	return createIterationHistory();
}

bool IOHandler::endIteration(vector<string>& itHistory)
{
	printf("%c", '\n');
	
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
	struct termios oldT, newT;
	int ch;
	tcgetattr(STDIN_FILENO, &oldT);
	newT = oldT;
	newT.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newT);
	ch = getchar();
	
	if (ch == 27)
	{
		ch = getchar();
		if (ch == 91)
			ch = getchar();
	}
	
	tcsetattr(STDIN_FILENO, TCSANOW, &oldT);
	return ch;
}

void IOHandler::handleKey(vector<string>& itHistory, int key)
{
	// move towards the end of the command, putting blanks
	for (int i = cursorPos; i < 0; ++i)
	{
		printf(" ");
	}
	
	int size = itHistory[historyIndex].size();
	
	// going backwards trough the beginning of the command, putting blanks
	for (int i = 0; i < size; ++i)
	{
		printf("\b \b");
	}
	
	if (key == KEY_BACKSPACE)
	{
		if (itHistory[historyIndex].size() > 0 && -cursorPos < itHistory[historyIndex].size())
		{
			itHistory[historyIndex].erase(itHistory[historyIndex].end() + cursorPos - 1);
		}
	}
	else if (key == KEY_LEFT)
	{
		cursorPos--;
		if (-cursorPos > itHistory[historyIndex].size())
			cursorPos = -itHistory[historyIndex].size();
	}
	else if (key == KEY_RIGHT)
	{
		cursorPos++;
		if (cursorPos > 0)
			cursorPos = 0;
	}
	else if (key == KEY_UP)
	{
		historyIndex = historyIndex == itHistory.size() - 1 ? historyIndex : historyIndex + 1;
		cursorPos = 0;
	}
	else if (key == KEY_DOWN)
	{
		historyIndex = historyIndex == 0 ? historyIndex : historyIndex - 1;
		cursorPos = 0;
	}
	else
	{
		itHistory[historyIndex].insert(itHistory[historyIndex].end() + cursorPos, key);
	}
	
	printf("%s", itHistory[historyIndex].c_str());
	
	// moves the cursor to the right pos
	for (int i = cursorPos; i < 0; ++i)
	{
		printf("\b");
	}
}

void IOHandler::start()
{
	
}

void IOHandler::end()
{
	
}

void IOHandler::print(const char* s)
{
	printf("%s", s);
}

vector<string> IOHandler::createIterationHistory()
{
	vector<string> itHistory = commandsHistory;
	itHistory.insert(itHistory.begin(), string());
	return itHistory;
}

