#include "syscall.h"

int main(int argc, char* argv[])
{
	char ch[127];
	char o[10];
	int n = 0;
	int cre = 2;
	OpenFileID TypeID;
	TypeID = - 2;

	PrintString ("nhap Do dai ten\n");
	n = ReadInt();
	PrintString("nhap Ten File Muon Tao\n");
	ReadString(ch,n);
	cre = CreateFile(ch);

	if ( cre == 0 )
	PrintString(" Tao Thanh Cong\n");
	if ( cre == -1 )
	PrintString("Eo Tao Duoc\n");
	

	
	TypeID = OpenFunt(ch,0);
	if ( TypeID == -1 )
		PrintString (" ko the mo file \n");
	else PrintString("Mo File Thanh Cong\n");

	PrintInt(TypeID );

	WriteFile( ch, 5, TypeID);

	ReadFile(ch, 5, TypeID);
	PrintString(ch);


}

