#ifndef COMMAND_H
#define COMMAND_H

#include <string>
#include <vector>

using namespace std;

// the Command class contains all necessary data of a shell command
class Command
{

public:

	Command(string&, vector<string>&, string&, string&, bool);
	~Command();

	string getIn();
	string getOut();
	string getCmd();
	const char** buildArgs();
	void destroyArgs();
	
#ifdef DEBUG_PRINT
	void print();
#endif

private:
	
	string cmd;
	vector<string> params;
	string in;
	string out;
	bool append;
	const char** args;

};

#endif
