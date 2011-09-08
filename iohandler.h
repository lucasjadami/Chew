#ifndef IOHANDLER_H
#define IOHANDLER_H

#include <vector>
#include <string>

using namespace std;

// the IOHandler class has the commands (history) handling and the input and output handling
class IOHandler
{

public:

	vector<string> startIteration();
	bool endIteration(vector<string>&);
	int getHistoryIndex();
	int readKey();
	void handleKey(vector<string>&, int);
	void start();
	void refreshIt();
	void end();

private:

	vector<string> createIterationHistory();
	
	int cursorPos;
	int historyIndex;
	vector<string> commandsHistory;

};

#endif
