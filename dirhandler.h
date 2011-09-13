#ifndef DIRHANDLER_H
#define DIRHANDLER_H

#include <string>

using namespace std;

class DirHandler
{

public:

	string getWorkingDir();

private:

	string getCurrentPath();
	string getUserString();
	string getHomeDir();

};

#endif
