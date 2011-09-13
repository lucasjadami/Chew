#ifndef RUNNER_H
#define RUNNER_H

#include "command.h"

// the Runner class executes a command
class Runner
{

public:

	Runner();
	~Runner();
	
	int run(Command&);
	
private:

};

#endif
