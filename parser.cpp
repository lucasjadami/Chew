#include "parser.h"

#include <sstream>

using namespace std;

Parser::Parser()
{

}

Parser::~Parser()
{

}

void Parser::parseLine(string& line, vector<Command>& commands)
{
	string str;
	istringstream stream(line);
	vector<string> params;
	string in, out;
	bool append = false;
	// enquanto tiver strings na linha
	while (1)
	{
		getWord(stream, str);
		if (str.size() == 0)
			break;
		// se for pipe eh novo comando
		if (str == "|")	
			addCommand(commands, params, in, out, append);
		else if (str == "<")
			stream >> in;
		else if (str == ">")
			stream >> out;
		else if (str == ">>")
		{
			append = true;
			stream >> out;
		}
		else
			params.push_back(str);
	}
	// ultimo comando
	addCommand(commands, params, in, out, append);
}

void Parser::addCommand(vector<Command>& commands, vector<string>& params, string& in, string& out, bool& append)
{
	if (params.size() != 0)
	{
		string cmd = params[0];
		params.erase(params.begin());
		Command command(cmd, params, in, out, append);
		commands.push_back(command);
	}
	in.clear();
	out.clear();
	params.clear();
	append = false;
}

void Parser::getWord(istringstream& stream, string& str)
{
	string aux;
	str.clear();
	if (!(stream >> aux))
		return;
	str = aux;
	if (aux[0] == '"' && aux[aux.size()-1] != '"' || aux == "\"")
	{
		while (stream >> aux)
		{
			str += " ";
			str += aux;
			if (aux[aux.size()-1] == '"')
				break;
		}
		if (str[str.size()-1] == '"')
			str = str.substr(1, str.size()-2);
		else
			str = str.substr(1, str.size()-1);
	}
	else if (aux[0] == '"' && aux[aux.size()-1] == '"')
		str = str = str.substr(1, str.size()-2);
	
	trim(str);
}

void trim2(string& str)
{
  string::size_type pos = str.find_last_not_of(' ');
  if(pos != string::npos) {
    str.erase(pos + 1);
    pos = str.find_first_not_of(' ');
    if(pos != string::npos) str.erase(0, pos);
  }
  else str.erase(str.begin(), str.end());
}

