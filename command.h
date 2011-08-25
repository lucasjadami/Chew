#ifndef COMMAND_H
#define COMMAND_H

#include <string>
#include <vector>

using namespace std;

class Command
{

public:

	Command(string&, vector<string>&, string&, string&, bool);
	~Command();
	
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
