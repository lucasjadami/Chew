#include "runner.h"

#include <unistd.h>
#include <fcntl.h>
#include <cstdio>
#include <cstring>

#define STDIN 0
#define STDOUT 1

Runner::Runner()
{

}

Runner::~Runner()
{

}

// TODO: handle errors
int Runner::run(Command& cmd, IOHandler& ioHandler)
{
	int infd = STDIN;
	FILE* outputBuffer;
	FILE* output = NULL;
	
	if (cmd.getIn().size() > 0)
	{
		infd = open(cmd.getIn().c_str(), O_RDONLY);
		dup2(infd, STDIN);
	}
	if (cmd.getOut().size() > 0)
	{
		output = fopen(cmd.getOut().c_str(), "w+");
	}
	
	outputBuffer = popen(cmd.toString().c_str(), "r");
	int obd = fileno(outputBuffer);
	fcntl(obd, F_SETFL, O_NONBLOCK);
	
	char buff[512];
	while (true)
	{
		ssize_t r = read(obd, buff, 512);
		if (r == -1)
	    		continue;
		else if (r > 0)
	    	{
	    		ioHandler.print(buff);
	    	}
		else
	    		break;
	}
	
	/*while (fgets(buff, 512, outputBuffer) != NULL) 
	{
		if (output == NULL)
    			ioHandler.print(buff);
    		else
    			fwrite(buff, sizeof(char), strlen(buff), output);
  	}*/
	
	if (infd != STDIN)
		close(infd);
	if (output != NULL)
		fclose(output);
	
	pclose(outputBuffer);
		
	return 0;
}

