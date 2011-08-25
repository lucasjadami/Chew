#include "parser.h"

#include <iostream>
#include <vector>
#include <string>

using namespace std;

vector<Command> commands;
Parser parser;

int main()
{		
	while (1)
	{
		cout << "> ";
		commands.clear();
		
		// gets the line typed by the user
		string in;
		
		if (getline(cin, in) == NULL)
			break;
		if (in == "exit")
			break;
			
		// parses the line typed commands
		parser.parseLine(in, commands);
		
#ifdef DEBUG_PRINT
		for (int i = 0; i < commands.size(); ++i)
			commands[i].print();
#endif
	}
	
	return 0;
}

