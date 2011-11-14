#include "parser.h"

#include <sstream>

Parser parser;

/**
 * Parses a line from the input, returning a vector of commands, this will be called chain of cmds.
 * @param line The line in a string object.
 * @param commands The vector reference to hold the chain of cmds.
 * @return if the chain must be executed in background.
 */

bool Parser::parseLine(string& line, vector<Command>& commands)
{
	string str;
	istringstream stream(line);
	vector<string> params;
	string in, out;
	bool append = false;
	/// while there are strings on the line
	while (1)
	{
		getWord(stream, str);
		if (str.size() == 0)
			break;
		/// if it is pipe, it is a new command
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
	/// last command
	addCommand(commands, params, in, out, append);
	
	if (commands.size() > 0 && commands[commands.size() - 1].getParams().size() > 0 && commands[commands.size() - 1].getParams()[
		commands[commands.size() - 1].getParams().size() - 1] == "&")
	{
		commands.rbegin()->removeAnd();
		return true; /// used '&' and it should be run in background!
	}
	else
		return false;
}

/**
 * Adding a command to the vector.
 * @param commands The vector of commands.
 * @param params The parameters of the command.
 * @param in The input file if any.
 * @param out The output file if any.
 * @param append If need to append to the output file.
 */
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

/**
 * Gets a valid word from the input stream.
 * @param stream The input stream.
 * @param str The word that will be returned.
 */
void Parser::getWord(istringstream& stream, string& str)
{
	string aux;
	str.clear();
	if (!(stream >> aux))
		return;
	str = aux;
	/// checks if it is an opening "
	if ((aux[0] == '"' && aux[aux.size()-1] != '"') || aux == "\"")
	{
		while (stream >> aux)
		{
			str += " ";
			str += aux;
			/// breaks on the closing "
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

/**
 * Trim the string, removing extra spaces.
 * @param str The input string.
 */
void Parser::trim(string& str)
{
	string::size_type pos = str.find_last_not_of(' ');
	if(pos != string::npos) 
	{
		str.erase(pos + 1);
		pos = str.find_first_not_of(' ');
		if (pos != string::npos)
			str.erase(0, pos);
	}
	else 
		str.erase(str.begin(), str.end());
}

