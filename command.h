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
	string toString();
	
#ifdef DEBUG_PRINT
	void print();
#endif

private:
	
	string cmd;
	vector<string> params;
	string in;
	string out;
	bool append;

};

#endif
