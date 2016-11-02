#include "syscall.h"
void main()
{
	OpenFileId input = ConsoleOutput;
	int i;
	char *c = "0\n";
	
	for (i=0; i<=9;i++) 
	{
		WriteFile(c,2,ConsoleOutput);
		(*c)++;
	}
    	return ;
}
