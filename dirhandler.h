#ifndef DIRHANDLER_H
#define DIRHANDLER_H

#include <string>

using namespace std;

class DirHandler
{

public:

	bool getWorkingPath(string&);
	bool setDir(string);
	bool getCurrentPath(string&);

private:

	bool getUserString(string&);
	bool getHomeDir(string&);

};

#endif
