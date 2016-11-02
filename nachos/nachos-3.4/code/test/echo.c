#include "syscall.h"

int main(int argc, char* argv[])
{
	char buf[127];
	OpenFileID stdin = OpenFunt("stdin",0);
	OpenFileID stdout = OpenFunt("stdout",1);
	if (stdin == -1 || stdout == -1)
	{
		PrintString("Open file error, please try again\n");
		return 0;
	}
	ReadFile(buf, 127, stdin);
	WriteFile(buf, 127, stdout);
	CloseFile(stdin);
	CloseFile(stdout);
	return 0;
}
