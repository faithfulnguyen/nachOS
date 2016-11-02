#include"syscall.h"

void main()
{
	char string [100];	
	int n = 0;
	PrintString(" nhap do dai chuoi \n " );
	n = ReadInt();
	PrintString(" nhap chuoi \n " );

	ReadString ( string, n );
	PrintString ( string );

}
