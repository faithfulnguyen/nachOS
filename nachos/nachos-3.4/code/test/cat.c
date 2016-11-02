#include "syscall.h"
#include "copyright.h"

int main(int argc, char* argv[])
{
	OpenFileID fileId;
	int filesz, i, j;
	char c;
	char name[127];
	PrintString("Input file name:");
	ReadString(name, 127);
	if ( (fileId= OpenFunt(name, 1)) == -1)
	{
		PrintString("Can not open file ");
		return 0;
	}
	filesz = SeekFile(-1, fileId);

	i = 0;
	SeekFile(0, fileId);
	
	for (; i < filesz; i++)
	{
		ReadFile(&c, 1, fileId);
		PrintChar(c);
	} 
	
	
	i = 0;

	PrintInt(filesz);
	CloseFile(fileId);
	return 0;
}
