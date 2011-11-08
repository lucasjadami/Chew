#ifndef PARSER_H
#define PARSER_H

#include "command.h"

#include <string>
#include <vector>
#include <sstream>

using namespace std;

/**
 * The Parser class transforms raw strings into Command objects.
 */
class Parser
{

public:
	
	bool parseLine(string&, vector<Command>&);

private:

	void addCommand(vector<Command>&, vector<string>&, string&, string&, bool&);
	void getWord(istringstream&, string&);
	void trim(string&);

};

extern Parser parser;

#endif

