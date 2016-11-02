#include "syscall.h"

int
main()
{
    SpaceId newProc;
    OpenFileID input = ConsoleInput;
    OpenFileID output = ConsoleOutput;

    char prompt[2], ch, buffer[60]; 
    int i;

    prompt[0] = '-';
    prompt[1] = '-';

   do
    {
	Read(buffer,10, ConsoleInput);
	if(buffer[0] == '0')
		break;
	newProc = Exec(buffer);
	if(newProc < 0)
	{
		Write("\nCan't not exec ",50,ConsoleOutput);
		continue;
	}

	Join(newProc);
	}while(1);
}

