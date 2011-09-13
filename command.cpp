#include "command.h"

#ifdef DEBUG_PRINT
#include <ncurses.h>
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

string Command::getIn()
{
	return in;
}

string Command::getOut()
{
	return out;
}

string Command::getCmd()
{
	return cmd;
}

const char** Command::buildArgs()
{
	args = new const char*[params.size()];
	for (int i = 0; i < params.size(); ++i)
		args[i] = params[i].c_str();
}

void Command::destroyArgs()
{
	delete[] args;
}

#ifdef DEBUG_PRINT
void Command::print()
{
	printw("Command name: %s\n", cmd.c_str());
	printw("Params:");
	for (int i = 0; i < params.size(); ++i)
		printw(" [%s]", params[i].c_str());
	printw("\nInput: %s\n", in.c_str());
	printw("Output: %s %s\n", out.c_str(), (append ? "[append]" : "" ));
}
#endif

