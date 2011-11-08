#include "dirhandler.h"

#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>

/** Max size for the directory. */
#define MAX_SIZE 1024

/** The dir handler. */
DirHandler dirHandler;

/**
 * @param path The string to be set as the path.
 * @return True on success.
 */
bool DirHandler::getWorkingPath(string& path)
{
	string user;
	if (!getCurrentPath(path) || !getUserString(user))
		return false;
	
	path += "$";
	path = user + path;
		
	return true;	
}

/**
 * @param path The path to set.
 * @return True on success.
 */
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

/**
 * @param path The string to be set as the path.
 * @return True on success.
 */
bool DirHandler::getCurrentPath(string& path)
{
	path = "";
	char s[MAX_SIZE + 1];
	
	if (getcwd(s, MAX_SIZE) != NULL)
		path = s;
		
	return path.size() > 0;
}

/**
 * @param user The string to be set as the user.
 * @return True on success.
 */
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

/**
 * @param home The string to be set as the home.
 * @return True on success.
 */
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
