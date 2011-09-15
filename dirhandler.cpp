#include "dirhandler.h"

#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <errno.h>
#include <cstring>

#define MAX_SIZE 1024

// returns a string starting with * on error
string DirHandler::getWorkingDir()
{
	string dir = getCurrentPath();
	if (dir.size() == 0)
	{
		dir = "*";
		dir += strerror(errno);
		return dir;
	}
	
	dir += "$";
	
	string homeDir = getHomeDir();
	if (homeDir.size() == 0)
	{
		dir = "*";
		dir += strerror(errno);
		return dir;
	}
	
	// TODO: test on other computers, with different home dirs
	if (dir.find_first_of(homeDir) == 0)
		dir = dir.substr(homeDir.size() + 2);
	
	dir = getUserString() + dir;
		
	return dir;	
}

// returns the error string on error
string DirHandler::setDir(string path)
{
	// result of the set dir; empty on success
	string s;
	
	int result;
	if (path.size() == 0)
		result = chdir(getHomeDir().c_str());
	else if (path[0] != '/')
		result = chdir(getCurrentPath().append(path).c_str());
	else
		result = chdir(path.c_str());
	
	if (result == -1)
		s = strerror(errno);
		
	return s;
}

// returns empty string on error
string DirHandler::getCurrentPath()
{
	char s[MAX_SIZE];
	string path;
	
	if (getcwd(s, MAX_SIZE) != NULL)
		path = s;
	else
		path = "";
		
	return path;
}

string DirHandler::getUserString()
{
	char s[MAX_SIZE];
	string user;
	int result;
	const char* login;
	
	login = getlogin();
	user = login == NULL ? "-" : login;
	user += "@";
	result = gethostname(s, MAX_SIZE);
	user += result == 0 ? s : "-";	
	user += ":";
	
	return user;
}

// returns empty string on error
string DirHandler::getHomeDir()
{
	string home;
	struct passwd* pw = getpwuid(getuid());
	
	if (pw != NULL)
	{
		const char* homeDir = pw->pw_dir;
		home = homeDir;
	}
	
	return home;
}
