#include "parser.h"

#include <iostream>
#include <vector>
#include <string>
#include <cstdio>
#include <termios.h>

using namespace std;

vector<Command> commands;
// history of commands
vector<vector<Command> > lastCommands;
int lastCommandIndex = 0;
Parser parser;

void changemode(int);

int main()
{		
	changemode(1);
	
	while (1)
	{
		cout << "> ";
		commands.clear();
		
		// gets the line typed by the user
		string in;
		
		int ch = 0;
		// 10 is the keycode for ENTER
		while (ch != 10)
		{
			ch = getchar();
			
			putchar(ch);
			
			if (ch == 10)
				continue;	
			
			// appends the char on the string
			in += ch;
		}
		
		if (in == "exit")
			break;
			
		// parses the line typed commands
		parser.parseLine(in, commands);

		lastCommands.insert(lastCommands.begin(), commands);
		lastCommandIndex = 0;
		
#ifdef DEBUG_PRINT
		for (int i = 0; i < commands.size(); ++i)
			commands[i].print();
#endif
	}
	
	changemode(0);
	
	return 0;
}

void changemode(int dir)
{
	static struct termios oldt, newt;

	if (dir == 1)
	{
		tcgetattr(STDIN_FILENO, &oldt);
		newt = oldt;
		newt.c_lflag &= ~(ICANON | ECHO);
		tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	}
	else
		tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
}

