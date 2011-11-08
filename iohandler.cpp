#include "iohandler.h"

#include <cstdio>

#define KEY_SPACE 32
#define KEY_BACKSPACE 127
#define KEY_UP 65
#define KEY_DOWN 66
#define KEY_RIGHT 67
#define KEY_LEFT 68
#define KEY_TAB 9

/** The I/O handler. */
IOHandler ioHandler;

#ifdef DEBUG_PRINT
/**
 * @param s The string to print.
 */
void IOHandler::debugPrint(const char* s)
{
	printf("\n%s\n", s);
}
#endif

/**
 * Sets the iteration up.
 * @param path The current path.
 * @return The history on the beggining of the iteration.
 */
vector<string> IOHandler::startIteration(string path)
{
	printf("[CHEW] %s ", path.c_str());
	cursorPos = 0;
	historyIndex = 0;
	return createIterationHistory();
}

/**
 * Finishes the iteration, updating the history.
 * @param itHistory The history of the iteration. Used to update the commands history.
 */
void IOHandler::endIteration(vector<string>& itHistory)
{
	printf("%c", '\n');
		
	/// Adds the current itHistory command to the beginning, because it is the most recent command.
	commandsHistory.insert(commandsHistory.begin(), itHistory[historyIndex]);
	
	int i = historyIndex == -1 ? 0 : 1;
	/// Merges the itHistory on the commandsHistory.
	while (i != (int) itHistory.size())
	{
		/// Do not merges the command on the position of the most recent command (LINUX behavior).
		if (i != historyIndex)
		{
			commandsHistory[i] = itHistory[i];
		}
		
		++i;
	}
	
	/// Checks if the command is an empty one, to remove it.
	if (commandsHistory[0].size() == 0)
		commandsHistory.erase(commandsHistory.begin());
}

/**
 * @return The history index.
 */
int IOHandler::getHistoryIndex()
{
	return historyIndex;
}

/**
 * @param specialChar Set to true if the key read is an extended key.
 * @return The key id.
 */
int IOHandler::readKey(bool& specialChar)
{
	struct termios newT;
	int ch;
	tcgetattr(STDIN_FILENO, &oldT);
	newT = oldT;
	newT.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newT);
	ch = getchar();
	/// If the key is the 27, a special char will be read (arrow keys).
	specialChar = ch == 27;
	if (ch == 27)
	{
		ch = getchar();
		if (ch == 91)
			ch = getchar();
	}
	else if (ch == 195)
	{
		/// Note: does not support accents.
		ch = getchar();
	}
	
	tcsetattr(STDIN_FILENO, TCSANOW, &oldT);
	
	return ch;
}

/**
 * With the key given, updates the screen/history.
 * @param itHistory The iteration history.
 * @param key The key to handle.
 * @param specialChar True if it is an extended key.
 */
void IOHandler::handleKey(vector<string>& itHistory, int key, bool specialChar)
{
	if (key == -1)
		return;
		
	/// Move towards the end of the command, putting blanks.
	for (int i = cursorPos; i < 0; ++i)
	{
		printf(" ");
	}
	
	int size = itHistory[historyIndex].size();
	
	/// Going backwards trough the beginning of the command, putting blanks.
	for (int i = 0; i < size; ++i)
	{
		printf("\b \b");
	}
	
	if (key == KEY_BACKSPACE)
	{
		if (itHistory[historyIndex].size() > 0 && -cursorPos < (int) itHistory[historyIndex].size())
		{
			itHistory[historyIndex].erase(itHistory[historyIndex].end() + cursorPos - 1);
		}
	}
	else if (key == KEY_TAB)
	{
		/// TODO: auto-complete.
	}
	else if (key == KEY_LEFT && specialChar)
	{
		cursorPos--;
		if (-cursorPos > (int) itHistory[historyIndex].size())
			cursorPos = -itHistory[historyIndex].size();
	}
	else if (key == KEY_RIGHT && specialChar)
	{
		cursorPos++;
		if (cursorPos > 0)
			cursorPos = 0;
	}
	else if (key == KEY_UP && specialChar)
	{
		historyIndex = historyIndex == (int) itHistory.size() - 1 ? historyIndex : historyIndex + 1;
		cursorPos = 0;
	}
	else if (key == KEY_DOWN && specialChar)
	{
		historyIndex = historyIndex == 0 ? historyIndex : historyIndex - 1;
		cursorPos = 0;
	}
	else
	{
		itHistory[historyIndex].insert(itHistory[historyIndex].end() + cursorPos, key);
	}
	
	printf("%s", itHistory[historyIndex].c_str());
	
	/// Moves the cursor to the right pos.
	for (int i = cursorPos; i < 0; ++i)
	{
		printf("\b");
	}
}

/**
 * Performs start operations on the I/O.
 */
void IOHandler::start()
{
	
}

/**
 * Finishes the I/O.
 */
void IOHandler::end()
{
	tcsetattr(STDIN_FILENO, TCSANOW, &oldT);
}

/**
 * @param s The string to print.
 */
void IOHandler::print(const char* s)
{
	printf("%s", s);
}

/**
 * @return The iteration history.
 */
vector<string> IOHandler::createIterationHistory()
{
	vector<string> itHistory = commandsHistory;
	itHistory.insert(itHistory.begin(), string());
	return itHistory;
}

