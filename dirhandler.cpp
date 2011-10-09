#include "dirhandler.h"

#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>

#define MAX_SIZE 1024

// returns true on success, the string var must be passed as param
bool DirHandler::getWorkingPath(string& path)
{
	string user;
	if (!getCurrentPath(path) || !getUserString(user))
		return false;
	
	path += "$";
	path = user + path;
		
	return true;	
}

// returns false on error
bool DirHandler::setDir(string path)
{
	string home;
	
	if (!getHomeDir(home))
		return false;
	
	int result;
	if (path.size() > 0)
		result = chdir(path.c_str());
	else
	// empty 'cd', goes to home
		result = chdir(home.c_str());
	
	return result != -1;
}

// returns false on error
bool DirHandler::getCurrentPath(string& path)
{
	path = "";
	char s[MAX_SIZE + 1];
	
	if (getcwd(s, MAX_SIZE) != NULL)
		path = s;
		
	return path.size() > 0;
}

// returns false on error
bool DirHandler::getUserString(string& user)
{
	char s[MAX_SIZE];
	const char* login;
	
	login = getlogin();
	
	if (login == NULL)
		return false;
		
	user = login;
	user += "@";
	
	if (gethostname(s, MAX_SIZE) == -1)
		return false;
		
	user += s;	
	user += ":";
	
	return true;
}

// returns false on error
bool DirHandler::getHomeDir(string& home)
{
	struct passwd* pw = getpwuid(getuid());
	
	if (pw != NULL)
	{
		const char* homeDir = pw->pw_dir;
		home = homeDir;
	}
	
	return pw != NULL;
}
