// exception.cc 
//	Entry point into the Nachos kernel from user programs.
//	There are two kinds of things that can cause control to
//	transfer back to here from user code:
//
//	syscall -- The user code explicitly requests to call a procedure
//	in the Nachos kernel.  Right now, the only function we support is
//	"Halt".
//
//	exceptions -- The user code does something that the CPU can't handle.
//	For instance, accessing memory that doesn't exist, arithmetic errors,
//	etc.  
//
//	Interrupts (which can also cause control to transfer from user
//	code into the Nachos kernel) are handled elsewhere.
//
// For now, this only handles the Halt() system call.
// Everything else core dumps.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "syscall.h"
#include "string.h"
#include "stdio.h"
//----------------------------------------------------------------------
// ExceptionHandler
// 	Entry point into the Nachos kernel.  Called when a user program
//	is executing, and either does a syscall, or generates an addressing
//	or arithmetic exception.
//
// 	For system calls, the following is the calling convention:
//
// 	system call code -- r2
//		arg1 -- r4
//		arg2 -- r5
//		arg3 -- r6
//		arg4 -- r7
//
//	The result of the system call, if any, must be put back into r2. 
//
// And don't forget to increment the pc before returning. (Or else you'll
// loop making the same system call forever!
//
//	"which" is the kind of exception.  The list of possible exceptions 
//	are in machine.h.
//----------------------------------------------------------------------
//////////////////////
void SC_HaltFunt();
void SC_ReadIntFunt();
void SC_PrintIntFunt();
void SC_ReadCharFunt();
void SC_PrintCharFunt();
void SC_PrintStringFunt();
void SC_ReadStringFunt();
////////////////////////
void SC_CreateFileFunt();
void SC_OpenFileIdFunt();
void SC_ReadFileFunt();
void SC_WriteFileFunt();
void SC_SeekFileFunt();
void SC_CloseFileFunt();
///////////////////////
void SC_ExecFileFunt();
void SC_JoinFileFunt();
void SC_ExitFileFunt();
///////////////////////
void SC_UpFileFunt();
void SC_DowFileFunt();
void SC_CreateSemaphoreFileFunt();
///////////////////////
void IncreRegister();
void SystemCall(int type);
	
void ExceptionHandler(ExceptionType which)
{
    int type = machine->ReadRegister(2);
	
	switch (which)
	{
		case NoException :
			printf("No exception \n");
			break;

		case SyscallException: // A program executed a system call
			SystemCall(type);
			break;

		case PageFaultException :   // No valid translation found
			printf("ERRO: No valid translation found %d %d\n", which, type);
			break;

		case ReadOnlyException :    // Write attempted to page marked  "read-only"
			printf ( "ERRO: Write attempted to page marked read-only %d %d\n", which, type);
			interrupt->Halt();
			break;

		case BusErrorException :    // Translation resulted in an invalid physical address
			printf ( "ERRO: Translation resulted in an invalid physical address %d %d\n", which, type);
			interrupt->Halt();
			break;

		case AddressErrorException : // Unaligned reference or one that was beyond the end of the address space
			printf ( "ERRO: Unaligned reference or one that was beyond the end of the address space %d %d\n", which, type);
			interrupt->Halt();
			break;

		case OverflowException :     // Integer overflow in add or sub.
			printf("ERRO: Integer overflow in add or sub %d %d\n", which, type);
			interrupt->Halt();
			break;

		case IllegalInstrException : // Unimplemented or reserved instr.
			printf(" ERRO: Unimplemented or reserved instr %d %d\n", which, type);
			interrupt->Halt();
			break;

		default :
			printf ("Unexpected user mode exception %d %d\n", which, type);
			ASSERT(FALSE);
			break;  //unreached statement 
	}
}


void SystemCall(int type)
{
	switch (type) 
	{
		/////////////////////////////
		//////đồ án 1////////////////
		case SC_Halt:
		SC_HaltFunt();
		break;
		
		case SC_ReadInt:
		SC_ReadIntFunt();
		IncreRegister();
		break;
		
		case SC_PrintInt:
		SC_PrintIntFunt();
		IncreRegister();
		break;
		
		case SC_ReadChar:
		SC_ReadCharFunt();
		IncreRegister();
		break;
		
		case SC_PrintChar:
		SC_PrintCharFunt();
		IncreRegister();
		break;
		
		case SC_ReadString:
		SC_ReadStringFunt();
		IncreRegister();
		break;
		
		case SC_PrintString:
		SC_PrintStringFunt();
		IncreRegister();
		break;
		/////////////////////////////
		//////đồ án 2////////////////
		case SC_CreateFile:
		SC_CreateFileFunt();
		IncreRegister();
		break;
		
		case SC_OpenFileID:
		SC_OpenFileIdFunt();
		IncreRegister();
		break;

		case SC_ReadFile:
		SC_ReadFileFunt();
		IncreRegister();
		break;

		case SC_WriteFile:
		SC_WriteFileFunt();
		IncreRegister();
		break;

		case SC_SeekFile:
		SC_SeekFileFunt();
		IncreRegister();
		break;

		case SC_CloseFile:
		SC_CloseFileFunt();
		IncreRegister();
		break;
		/////////////////////////////
		//////đồ án 3////////////////
		case SC_Exec:
		SC_ExecFileFunt();
		IncreRegister();
		break;
		
		case SC_Join:
		SC_JoinFileFunt();
		IncreRegister();
		break;
		
		case SC_Exit:
		SC_ExitFileFunt();
		IncreRegister();
		//////////////////
		//////////////////
		case SC_Up:
		SC_UpFileFunt();
		IncreRegister();
		break;
		
		case SC_Dow:
		SC_DowFileFunt();
		IncreRegister();
		break;
		
		case SC_CreateSemaphore:
		SC_CreateSemaphoreFileFunt();
		IncreRegister();
		break;
		
	}
	
}

void IncreRegister()
{
	int pc; // bien chua dia chi thanh ghi
	pc = machine->ReadRegister(PCReg); // gan dia chi cua thanh ghi hien tai vao pc
	machine->WriteRegister(PrevPCReg, pc); // gan dia chi cua truoc bang dia chi hien tai
	pc = machine->ReadRegister(NextPCReg); // gan pc bang dia ki cau lenh ke tiep
	machine->WriteRegister(PCReg, pc); // dia chi lenh hien tai se bang dia chi lenh ke tiep
	pc += 4;  // tang dia chi lenh mot 
	machine->WriteRegister(NextPCReg, pc); // gan dia chi ke lenh ke tiep sau khi tang dai chi pc		
}
//////////////////////////////////////////////////
void SC_HaltFunt()
{
	DEBUG('a', "Shutdown, initiated by user program.\n");
	interrupt->Halt();
}
void SC_ReadIntFunt()
{
	int i, j, sizechar;
	int s = 0; // dung de luu so da doc vao. 			
	bool flag = true;
	char* bufer = new char[10]; // 10: la ch-ieu dai cua so nguyen, chuoi nay dung de luu gia tri doc vao
	sizechar = gSynchConsole->Read(bufer, 10); // doc vao so nguyen o dang ki tu vao chuoi bufer
	if ( bufer[0] == '-' ) // neu am thi bat dau xu ly tai vi tri so 1
	{
		i = 1; j = 1;
		
	}
	else // nguoc lai xu ly tu dau	
	{
		i = 0; j = 0;
	}
	if ( sizechar > 10 ) // neu nguoi dung nhap qua 10 ki tu
		return;
	// kiem tra nguoi dung nhap co dung khong
	for ( ; j < strlen ( bufer ); j++ )
	{
		if ( bufer[j] - 48 > 10 || bufer[j] - 48 < 0 )
		{
			flag = false;
		}
	}
	if ( flag == false ) // neu nguoi dung nhap sai break, va goi ham Halt
	{
		printf("input wrong\n");
		interrupt->Halt();
		ASSERT(FALSE);
		return;
	}
	else
	{	// chuyen ki tu kieu char sang so nguyen int
		for ( ; i < strlen( bufer ); i++ )
		{
			s = s * 10 + bufer[ i ] - 48;
		}
		if ( bufer[0] == '-' )
			s = s * ( -1 );
		machine->WriteRegister(2, s); // ghi ket qua vao thanh ghi so 2;
		delete bufer;
	}
}
void SC_PrintIntFunt()
{
	int n = 0;					
	char dau = NULL; // dau cua so nguyen da doc vao
	n = machine->ReadRegister(4); // doc du lieu tu thanh ghi so 4
	if ( n < 0 )
	{
		dau = '-'; // n am, lua lai dau
		n *= -1; // chuyen n thanh so duong de tinh toan
	}
	char subbuf [ 10 ]; // mot mang de luu tru tung so nho trong so nguyen
	int count = 0; // tao mot bien dem, de dem so luong so trong so nguyen, vd 1234 count = 4;
	do
	{
		subbuf[count] = n % 10 + 48; // chuyen so nguyen int thanh kieu char
		n = n/10;
		if ( n > 0 )
		count++;
	}while ( n > 0 );
	
	if ( dau == '-' ) // neu am in ra dau -
	{
		gSynchConsole->Write( &dau, 1);
	}
	char mainbuf[ 10 ]; // tao mang nay de dao mang
	subbuf[ count + 1 ] = '\0'; // tao ki tu ket thuc cho mang phu		
	int i = 0;
	for ( ; i <= count; i++ ) // dao lai mang, vi du 321 => 123
	{
		mainbuf[ i ] = subbuf[ count - i ];
	}
	gSynchConsole->Write( mainbuf, count + 1);// in ra man hinh
}
void SC_ReadCharFunt()
{
	int size; // luu kich thuoc cua chuoi doc vao
	char bufer[127]; // mang chua ki tu
	// doc vao kich thuoc cua bien
	size = gSynchConsole->Read(bufer, 127);
	// ghi du lieu xuong thanh ghi bo di ki tu ket thuc \0
	machine->WriteRegister(2, bufer[size-1]); // ghi xuong thanh ghi chuoi da doc vao voi kich thuoc size - 1, bo ki tu /0
	
}
void SC_PrintCharFunt()
{
	char tmpch;
	// doc du chuoi can in ra tu thanh ghi
	tmpch = (char) machine->ReadRegister(4);
	// dung synchconsole de in ra man hinh, in chuoi temCh, kich thuoc cua bien char
	// vi chi in ra mot ki tu kiee char nen la so 1
	gSynchConsole->Write( &tmpch, 1);
}
void SC_PrintStringFunt()
{
	int adrbufWri = machine->ReadRegister( 4 ); // doc dia chi cua chuoi da doc vao.
	char* bufer = new char [127]; // khai bao mang bufer chua chuoi da doc vao
	if ( bufer == NULL )
	{
		printf ( " not enuogh menory or somthing fail \n");
		return;
	}
	bufer = machine -> User2System ( adrbufWri, 127 ); // chuyen tu bo nho user sang kernel
	if ( bufer == NULL )
	{
		printf ( "khong the chuyen vung nho \n");
		return;	
	}
	int seek = 0; // in ra chuoi ki tu
	for ( int i = 0; i < strlen ( bufer ); i++ ) // in tung ki tu trong mang ki tu
	{
		if ( bufer[i] == '\n' ) // neu khong gap ki tu '\n' thi tiep tuc in ra
		{
			seek = i;  // neu gap ki tu '\n' luu lai vi tri cua ki tu do.
			return; // thoat vong lap
		}
		gSynchConsole->Write( &bufer[i], 1); // in 1 ki tu ra man hinh
							
	}
	bufer[seek] = '\n';
	gSynchConsole->Write( &bufer[seek], 1);
	delete bufer; // xoa chuoi					
}
void SC_ReadStringFunt()
{
	char* bufer;
	int size;
	int adrbufRead; // dia chi bat day cua chuoi
	int lenght; // chieu dai cua chuoi
	bufer = new char [127]; // 127 kich thuoc toi da cua bien char
	// kiem tra bufer co khoi tao duoc hay khong
	if ( bufer == 0)
	{
		printf ( "not enough memory or something\n");
		interrupt->Halt();
		ASSERT(FALSE);
		return;
	}
	adrbufRead = machine->ReadRegister( 4 ); // nhan dia chi cua bufer
	lenght = machine->ReadRegister( 5 ); // kich thuoc cua chuoi can doc vao
	size = gSynchConsole->Read(bufer, lenght); // nhan chuoi can doc vao bufer, voi chieu dai cua chuoi la lenght
	machine->System2User( adrbufRead, size, bufer); // chuyen sang bo nho sang user;
	delete bufer;
}

void SC_CreateFileFunt()
{
	int addr;
	char* buf = new char [127];
	addr = machine->ReadRegister(4);
	buf = machine->User2System ( addr, 32 );
	if ( buf == NULL )
	{
		machine->WriteRegister(2,0);
		printf ( "loi file \n");
		machine->WriteRegister(2,-1);
	}					

	fileSystem->Create(buf,0); // goi phuong thuc tao file trong lop fileSystem
	machine->WriteRegister(2,0);
	//delete buf;

}
void SC_OpenFileIdFunt()
{
	int addr;
	OpenFile *of;
	int i;
	char* buf = new char [127];
	addr= machine->ReadRegister(4);
	int type = machine->ReadRegister(5);
	if ( type != 0 && type != 1 ) // neu khong phai type = 0 ( read only ) hoac type = 1( read and write ) bao 						//loi cho nguoi dung
	{
		printf("Type only 1 or 0 ( 0 -> input, 1-> output \n");
		interrupt->Halt();
		ASSERT(FALSE);
		return;
	}
	buf = machine->User2System( addr, 127); 
	for ( i = 5; i < 10; i++) // tim kiem vi tri trong trong bang dat ta file: FileTableOpen
		if ( currentThread->space->openfiletab->FileTableOpen[i] == NULL) // neu tim duoc vi tri trong thoat.
			break;

	if (i == 10) // truong hop bang khong con cho trong
	{
		printf("file table full !\n");
		machine->WriteRegister(2,-1);
		return;
	}
	of = fileSystem->Open(buf);//mo file
	if (!of) // khong the mo duoc file
	{
		printf("can't not open file \n");
		machine->WriteRegister(2,-1);
		return;
	}
	currentThread->space->openfiletab->FileTableOpen[i] = of; // lua file da mo vao bang dat ta file	
	printf("now file is opening!!! \n");
	machine->WriteRegister(2,i);
}
// doc file
void SC_ReadFileFunt()
{
	int addr;
	int charcount;
	int OpenF_ID;
	int nSize;
	int oldpos;
	int newpos;
	OpenFile *of = NULL;
	addr = machine->ReadRegister(4);
	charcount = machine->ReadRegister(5);
	OpenF_ID = machine->ReadRegister(6);
	char* buf = new char [127];
	char* tempbuf = new char [127];
	int i = 0;

	if ( charcount == 0 )
	{
		machine->WriteRegister(2,0);
		return;
	}
	of = currentThread->space->openfiletab->FileTableOpen[OpenF_ID]; // mo file nguoi dung yeu cau
	buf = machine -> User2System( addr, charcount );

	oldpos = currentThread->space->openfiletab->FileTableOpen[OpenF_ID]->GetCurrentPos(); // gan oldpos bang vi 
// tri hien tai cua con tro tap tin
	buf = machine->User2System(addr, charcount);

	if (OpenF_ID < 0 || OpenF_ID > 9) // neu vuot qua gioi han mo file
	{
		machine->WriteRegister(2, -1);
		return;
	}
	if ( OpenF_ID == 0) // neu la doc du lieu tu console
   	{
		int sizeread = gSynchConsole->Read( buf, charcount ); 
		machine->System2User(addr, charcount, buf);
		machine->WriteRegister(2, sizeread);
		return;
   	}

	if ( !of || OpenF_ID == 1 || OpenF_ID == 2 || OpenF_ID == 3 )// neu of khac NULL, hoac ID = 1, 2, 3
	{
		printf ("Invalid File, Request fail, because ID error \n");
		machine->WriteRegister(2,-1);
		interrupt->Halt();
		ASSERT(FALSE);
		return;
	}
	if ( of && ( OpenF_ID == 1 || OpenF_ID == 2 || OpenF_ID == 3) ) // neu of la NULL, va ID = 1, 2, 3
	{
		printf ("Invalid File, Request fail, because ID error and file NUll\n");
		machine->WriteRegister(2,-1);
		interrupt->Halt();
		ASSERT(FALSE);
		return;
	}
	// neu co du lieu trong file
	if ((currentThread->space->openfiletab->FileTableOpen[OpenF_ID]->Read(buf, charcount) ) > 0)
	{
		// Copy data from kernel to user space
		// DANGKHOA/NACHOS
		

		newpos = currentThread->space->openfiletab->FileTableOpen[OpenF_ID]->GetCurrentPos();

		machine->System2User(addr, newpos - oldpos + 1, buf);
		
		machine->WriteRegister(2, newpos - oldpos + 1);

		printf("%s",buf);
		//i++;

	}
}

void SC_WriteFileFunt ()
{
	int addr;
	int charcount;
	int OpenF_ID;
	OpenFile *of;
	int oldpos;
	int newpos;

	addr= machine->ReadRegister(4);
	charcount = machine->ReadRegister(5);
	OpenF_ID = machine->ReadRegister(6);
	char* buf = new char[127];

	buf = machine->User2System ( addr, charcount);


	of = currentThread->space->openfiletab->FileTableOpen[OpenF_ID];// mo file nguoi dung yeu cau

	oldpos = currentThread->space->openfiletab->FileTableOpen[OpenF_ID]->GetCurrentPos(); //gan oldpos bang vi 
// tri hien tai cua con tro tap tin

	buf = machine->User2System(addr, charcount);

	if ( OpenF_ID < 0 || OpenF_ID > 9 ) // neu ID vuot qua khai bao
	{
		printf ("Type ID wrong, beyond limit maxfiletable ( 10 ) \n");
		machine->WriteRegister(2,-1);
		interrupt->Halt();
		ASSERT(FALSE);
		return;
	}

	if ( OpenF_ID == 0 || OpenF_ID == 2 || OpenF_ID == 3 || OpenF_ID == 4 ) // neu ID trung vao cac vi tri da //khai bao san
	{
		printf ("Type ID wrong, ID can't = 1 or = 3 or 4 \n");
		machine->WriteRegister(2,-1);
		interrupt->Halt();
		ASSERT(FALSE);
		return;
	}
	if (!of) 
	{
		printf ("Not Exists File,OpenFile = NULL \n");
		machine->WriteRegister(2,-1);
		interrupt->Halt();
		ASSERT(FALSE);
		return;
	}

	if ( OpenF_ID == 1 ) // neu la doc tu console
	{
		//DANGKHOA/NACHOS
		int i = 0;
		printf("stdout mode\n");
		while (buf[i] != 0 && buf[i] != '\n')
		{
			gSynchConsole->Write(buf+i, 1);
			i++;
		}
		buf[i] = '\n';
		gSynchConsole->Write(buf+i,1);
		machine->WriteRegister(2, i-1);

	}
	else 
	{
		printf(" Write file \n");
		of->Write(buf,charcount);
	}
	machine->WriteRegister(2,0);	
}

void SC_SeekFileFunt()
{
	int pos = machine->ReadRegister(4);
	int OpenF_ID = machine->ReadRegister(5);

	OpenFile *of;
	
	of = currentThread->space->openfiletab->FileTableOpen[OpenF_ID];// mo file

	if (OpenF_ID < 0 || OpenF_ID > 9)
	{
		machine->WriteRegister(2, -1);
		return;
	}
	
	if (OpenF_ID == 0)
	{
		printf("can't not call seek on console \n");
		machine->WriteRegister(2, -1);
		return;
	}
	if (OpenF_ID == 1)
	{
		printf("can't not call seek on console \n");
		machine->WriteRegister(2, -1);
		return;
	}
	if (OpenF_ID == 4)
	{
		printf("standard error \n");
		machine->WriteRegister(2, -1);
		return;
	}
	if (currentThread->space->openfiletab->FileTableOpen[OpenF_ID] == NULL)
	{
		printf("seek fail \n");
		machine->WriteRegister(2, -1);
		return;
	}
	if ( pos == -1 )// theo yeu cau do an pos = -1, doc den cuoi file
	{
		pos = currentThread->space->openfiletab->FileTableOpen[OpenF_ID]->Length();
	}
	
	if (pos >  currentThread->space->openfiletab->FileTableOpen[OpenF_ID]->Length()  || pos < 0) // neu vi tri khong phu hop, lon qua so voi file, hoac vi tri < 0
	{
		machine->WriteRegister(2, -1);
		return;
	} 
	else 
	{
		currentThread->space->openfiletab->FileTableOpen[OpenF_ID]->Seek(pos); // di chuyen con tro
		machine->WriteRegister(2, pos);
		return;
	}
}
void SC_CloseFileFunt()
{
	int OpenF_ID;
	OpenFile *of;
	OpenF_ID = machine->ReadRegister(4);
	of = currentThread->space->openfiletab->FileTableOpen[OpenF_ID]; // mo file 
	if (!of)
	{
	 	printf("file error!\n");
		machine->WriteRegister(2,-1);
		return;
	}
	delete of;
	currentThread->space->openfiletab->FileTableOpen[OpenF_ID]=NULL; // gan file bang NULL de ket thuc mo file.
	machine->WriteRegister(2,0);
}

////////////////////////////////////////////////////
//////////////////đồ án 3///////////////////////////
void SC_ExecFileFunt()
{
	int addr;
	char* name;
	OpenFile *opf;
	name = new char[32];
	//Đọc địa chỉ tên chương trình “name” từ thanh ghi r4.
	addr = machine->ReadRegister(4);
	//Tên chương trình lúc này đang ở trong user space. Gọi hàmUser2Systemđã được khai báo trong lớp machine để chuyển vùng nhớ user space tới vùng nhớ system space. 
	name = machine->User2System(addr,32);
	//mở file
	opf = fileSystem->Open(name);
	
	//lỗi báo “Không mở được file” và gán -1 vào thanh ghi 2. 
    if (opf == NULL)
	{
		printf("unable open file (exception) %s\n", name);
		machine->WriteRegister(2,-1);
		delete opf;
		return;
    }
	// không có lỗi thì gọi pTab-> ExecUpdate(name), trả về và lưu kết quả thực thi phương thức này vào thanh ghi r2
	int id = pTab->ExecUpdate(name); 
	machine->WriteRegister(2,id);
	//delete opf;
}

void SC_JoinFileFunt()
{
	//Đọc id của tiến trình cần Join từ thanh ghi r4. 
	int id = machine->ReadRegister(4);
	//Gọi thực hiện pTab->JoinUpdate(id) 
	int ec = pTab->JoinUpdate(id);
	//lưu kết quả thực hiện của hàm vào thanh ghi r2.
	machine->WriteRegister(2,ec);
}

void SC_ExitFileFunt()
{
	//Đọc exitStatus từ thanh ghi r4
	int ec;
  	int exitStatus = machine->ReadRegister(4);

  //nếu có lỗi sảy ra
	if (exitStatus != 0)
	{
    	printf("\nProcess %s exited with error level %d",currentThread->getName(),exitStatus);
		return;
	}
	//Gọi thực hiện pTab->ExitUpdate(exitStatus) 
	ec = pTab->ExitUpdate(exitStatus);
	//lưu kết quả thực hiện của hàm vào thanh ghi r2.
	machine->WriteRegister(2,ec);
}

void SC_UpFileFunt()
{
	int addr;
	int numb;
	char *name;
	name = new char[32];
	
	addr = machine->ReadRegister(4);
	
	name = machine->User2System(addr,32);
	//Kiểm tra Semaphore “name” này có trong bảng sTab chưa, nếu chưa có thì báo lỗi.
	bool flag = semTab->testname(name);
	if ( flag == false )
	{
		printf("STable: name is not exists in semTab \n");
		interrupt->Halt();
		return;
	}
	numb = semTab->Signal(name);
	if(numb < 0)
	{
		printf("Can't Up semaphore : %s",name);
		machine->WriteRegister(2,-1);
		return;
	}
	machine->WriteRegister(2,0);
}

void SC_DowFileFunt()
{
	int addr;
	int numb;
	char *name;
	name = new char[32];
	
	addr = machine->ReadRegister(4);
	
	name = machine->User2System(addr,32);
	//Kiểm tra Semaphore “name” này có trong bảng sTab chưa, nếu chưa có thì báo lỗi.
	bool flag = semTab->testname(name);
	if ( flag == false )
	{
		printf("STable: name is not exists in semTab\n");
		interrupt->Halt();
		return;
	}
	
	numb = semTab->Wait(name);
	if(numb < 0)
	{
		printf("Can't dow semaphore : %s",name);
		machine->WriteRegister(2,-1);
		return;
	}
	machine->WriteRegister(2,0);
}

void SC_CreateSemaphoreFileFunt()
{
	int addr;
	char *name;
	int semval;
	int semnum;
	name = new char[32];
	//Đọc địa chỉ “name” từ thanh ghi r4
	addr = machine->ReadRegister(4);
	
	//Đọc giá trị “semval” từ thanh ghi r5
	semval = machine->ReadRegister(5);
	// hàmUser2Systemđã được khai báo trong lớp machine để chuyển vùng nhớ user space tới vùng nhớ system space
	name = machine->User2System(addr,32);
	//kiểm tra tên 
	if(name == NULL)
	{
		printf("error, name can't NULL\n");
		machine->WriteRegister(2,-1);
		return;
	}
	//Gọi thực hiện hàm semTab->Create(name,semval) để tạo Semaphore, có lỗi thì báo lỗi
	semnum = semTab->Create(name,semval);
	if(semnum < 0)
	{
		printf("Can't create semaphore : %s",name);
		machine->WriteRegister(2,-1);
		return;
	}
	machine->WriteRegister(2,0);
}