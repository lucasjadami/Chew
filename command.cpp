#include "command.h"

#ifdef DEBUG_PRINT
#include <iostream>
#endif

using namespace std;

Command::Command(string& cmd, vector<string>& params, string& in, string& out, bool append)
{
	this->cmd = cmd;
	this->params = params;
	this->in = in;
	this->out = out;
	this->append = append;
}

Command::~Command()
{

}

#ifdef DEBUG_PRINT
void Command::print()
{
	cout << "Command name: " << cmd << endl;
	cout << "Params:";
	for (int i = 0; i < params.size(); ++i)
		cout << " [" << params[i] << "]";
	cout << endl << "Input: " << in << endl;
	cout << "Output: " << out << " " << (append ? "[append]" : "" ) << endl;
}
#endif

