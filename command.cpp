#include "command.h"

#ifdef DEBUG_PRINT
#include "iohandler.h"

#include <sstream>
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

// TODO: fix pointer bug
const char** Command::buildArgs()
{
	args = new const char*[params.size()+1];
	args[0] = cmd.c_str();
	for (int i = 0; i < params.size(); ++i)
		args[i+1] = params[i].c_str();
		args[params.size()+1] = 0;
#ifdef DEBUG_PRINT
	string s;
	for (int i = 0; i < params.size()+1; ++i)
	{
		stringstream ss;
		ss << "argv[";
		ss << i;
		ss << "] = ";
		ss << args[i];
		s += ss.str();
		s += "\n";
	}
	IOHandler::debugPrint(s.c_str());
#endif

	return args;
}

void Command::destroyArgs()
{
	delete[] args;
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

