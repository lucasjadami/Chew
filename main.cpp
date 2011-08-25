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
		
		// pega a linha digitada
		string in;
		
		if (getline(cin, in) == NULL)
			break;
		if (in == "exit")
			break;
			
		// parseia os comandos digitados da linha
		parser.parseLine(in, commands);
		
#ifdef DEBUG_PRINT
		for (int i = 0; i < commands.size(); ++i)
			commands[i].print();
#endif
	}
	
	return 0;
}

