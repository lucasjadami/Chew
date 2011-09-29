#include "command.h"

#ifdef DEBUG_PRINT
#include "iohandler.h"
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

string Command::toString()
{
	string s = cmd;
	for (int i = 0; i < params.size(); ++i)
	{
		s += " " + params[i];
	}
	return s;
}

#ifdef DEBUG_PRINT
void Command::print()
{
	string s;
	s += "Command name: ";
	s += cmd.c_str();
	s += "\n";
	s += "Params:";
	for (int i = 0; i < params.size(); ++i)
	{
		s += " [";
		s += params[i].c_str();
		s += "]"; 
	}
	s += "\nInput: ";
	s += in.c_str();
	s += "\n";
	s += "Output: ";
	s += out.c_str();
	s += " ";
	s += append ? "[append]" : "";
	s += "\n";
	
	IOHandler::debugPrint(s.c_str());
}
#endif

