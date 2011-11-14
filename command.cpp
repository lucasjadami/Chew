#include "command.h"

#ifdef DEBUG_PRINT
#include "iohandler.h"

#include <sstream>
#endif

using namespace std;

/**
 * Constructor.
 */
Command::Command(string& cmd, vector<string>& params, string& in, string& out, bool append)
{
	this->cmd = cmd;
	this->params = params;
	this->in = in;
	this->out = out;
	this->append = append;
}

/**
 * Destructor.
 */
Command::~Command()
{

}

/**
 * @return The in attribute.
 */
string Command::getIn()
{
	return in;
}

/**
 * @return The out attribute.
 */
string Command::getOut()
{
	return out;
}

/**
 * @return The c,d attribute.
 */
string Command::getCmd()
{
	return cmd;
}

/**
 * @return The params attribute.
 */
vector<string> Command::getParams()
{
	return params;
}

/**
 * @return The first param of the command.
 */
string Command::getFirstParam()
{
	string param = "";
	if (params.size() > 0)
		param = params[0];
	return param;
}

/**
 * @return The args attribute.
 */
const char** Command::buildArgs()
{
	args = new const char*[params.size()+2];
	args[0] = cmd.c_str();
	for (int i = 0; i < (int) params.size(); ++i)
		args[i+1] = params[i].c_str();
		args[params.size()+1] = 0;
	args[params.size()+1] = 0;
#ifdef DEBUG_PRINT
	string s;
	for (int i = 0; i < (int) params.size()+1; ++i)
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

/**
 * Frees the args param.
 */
void Command::destroyArgs()
{
	delete[] args;
}

/**
 * Removes the '&' from the command params.
 */
void Command::removeAnd()
{
	params.pop_back();
}

/**
 * @return If text to the output file will be appended.
 */
bool Command::isAppend()
{
	return append;
}

#ifdef DEBUG_PRINT
/**
 * Prints the command info.
 */
void Command::print()
{
	string s;
	s += "Command name: ";
	s += cmd.c_str();
	s += "\n";
	s += "Params:";
	for (int i = 0; i < (int) params.size(); ++i)
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

