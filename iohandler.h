#ifndef IOHANDLER_H
#define IOHANDLER_H

#include <vector>
#include <string>
#include <termios.h>
#include <unistd.h>

using namespace std;
 
/**
 * The IOHandler class has the commands (history) handling and the input and output handling.
 */
class IOHandler
{

public:

#ifdef DEBUG_PRINT
	static void debugPrint(const char* s);
#endif

	vector<string> startIteration(string);
	void endIteration(vector<string>&);
	int getHistoryIndex();
	int readKey(bool&);
	void handleKey(vector<string>&, int, bool);
	void start();
	void end();
	void print(const char* s);

private:

	vector<string> createIterationHistory();
	
	/** The old terminal state. It is kept to restore the terminal state on exit. */
	struct termios oldT;
	/** The carret position on the input. From 0 to -INF. */
	int cursorPos;
	/** The current history index. */
	int historyIndex;
	/** Vector containing the commands history. */
	vector<string> commandsHistory;

};

extern IOHandler ioHandler;

#endif
