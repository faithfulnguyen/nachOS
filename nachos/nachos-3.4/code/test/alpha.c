#include "syscall.h"

void main()
{
	OpenFileId input = ConsoleOutput;
	int i;
	char *c = "a\n";
	
	for (i=0; i<26;i++) {
		WriteFile(c,2,ConsoleOutput);
		(*c)++;
	}
	
    return ;
}
