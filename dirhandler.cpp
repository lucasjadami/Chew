#include "dirhandler.h"

#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>

// TODO: make this dynamic
#define MAX_SIZE 1024

string DirHandler::getWorkingDir()
{
	string dir = getCurrentPath();
	dir += "$";
	
	string homeDir = getHomeDir();
	
	// TODO: test on other computers, with different home dirs
	if (dir.find_first_of(homeDir) == 0)
		dir = dir.substr(homeDir.size() + 2);
	
	dir = getUserString() + dir;
		
	return dir;	
}

string DirHandler::getCurrentPath()
{
	char s[MAX_SIZE];
	string path;
	
	// TODO: handle better this error
	if (getcwd(s, MAX_SIZE) != NULL)
		path = s;
	else
		path = "PATH ERROR";
		
	return path;
}

string DirHandler::getUserString()
{
	char s[MAX_SIZE];
	string user;
	user = getlogin();
	user += "@";
	// TODO: handle error
	gethostname(s, MAX_SIZE);
	user += s;
	user += ":";
	return user;
}

string DirHandler::getHomeDir()
{
	string home;
	struct passwd *pw = getpwuid(getuid());
	const char *homeDir = pw->pw_dir;
	home = homeDir;
	return home;
}
