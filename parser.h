#ifndef PARSER_H
#define PARSER_H

#include "command.h"

#include <string>
#include <vector>
#include <sstream>

using namespace std;

class Parser
{

public:

	Parser();
	~Parser();
	
	void parseLine(string&, vector<Command>&);

private:

	void addCommand(vector<Command>&, vector<string>&, string&, string&, bool&);
	void getWord(istringstream&, string&);

};

#endif

