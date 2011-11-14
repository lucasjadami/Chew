#ifndef COMMAND_H
#define COMMAND_H

#include <string>
#include <vector>

using namespace std;

/**
 * The Command class contains all necessary data of a shell command.
 */
class Command
{

public:

	Command(string&, vector<string>&, string&, string&, bool);
	~Command();

	string getIn();
	string getOut();
	string getCmd();
	vector<string> getParams();
	string getFirstParam();
	const char** buildArgs();
	void destroyArgs();
	void removeAnd();
	bool isAppend();
	
#ifdef DEBUG_PRINT
	void print();
#endif

private:
	
	/** The command as string. */
	string cmd;
	/** The params. */
	vector<string> params;
	/** The in string (<). */
	string in;
	/** The out string (>). */
	string out;
	/** Append flag. */
	bool append;
	/** Array of strings used for the exec system call. */
	const char** args;

};

#endif
