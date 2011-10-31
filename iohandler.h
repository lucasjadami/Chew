#ifndef IOHANDLER_H
#define IOHANDLER_H

#include <vector>
#include <string>

using namespace std;

// the IOHandler class has the commands (history) handling and the input and output handling
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
	
	int cursorPos;
	int historyIndex;
	vector<string> commandsHistory;

};

#endif
