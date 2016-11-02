#include "syscall.h"
#include "copyright.h"
//tham khao tu
//https://github.com/dangkhoasdc/nachos/blob/master/test/copy.c
int main(int argc, char* argv[])
{
	OpenFileID IDsource;
	OpenFileID IDdest;
	char source[127], dest[127];
	int filesz,srcsz, i;
	char c;
	int IDerror;
	
	PrintString("Input source file:");
	ReadString(source, 127);
	PrintString("Input destination file:");
	ReadString(dest, 127);
	PrintString(source);

	IDsource = OpenFunt(source, 1);
	CreateFile(dest);
	
	IDdest = OpenFunt(dest, 0);

	if (IDsource == -1 || IDdest == -1)
	{
				
		PrintString("Can not open file \n");
		return -1;
	}
	filesz = SeekFile(-1, IDsource);

	SeekFile(0, IDsource);
	SeekFile(0, IDsource);
	for (i = 0; i < filesz; ++i)
	{
		ReadFile(&c, 1, IDsource);
		WriteFile(&c, 1, IDsource);
	}
	CloseFile(IDsource);
	CloseFile(IDsource);
	return 0;
}
