#ifndef DIRHANDLER_H
#define DIRHANDLER_H

#include <string>

using namespace std;

/**
 * This class is responsible for handling the directory related system calls.
 */
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

extern DirHandler dirHandler;

#endif
