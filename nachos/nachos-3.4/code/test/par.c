
#include "syscall.h"

void
main()
{
	OpenFileID input = ConsoleOutput;
	SpaceId alpha, count;
	WriteFile("Starting par.c\n",15,ConsoleOutput);
	
	alpha = Exec("./test/alpha");
	count = Exec("./test/number");
	
	Join(alpha);
	Join(count);
	
    return ;
}
