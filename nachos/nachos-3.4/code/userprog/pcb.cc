#include "pcb.h"
#include "utility.h"
#include "system.h"
#include "thread.h"
#include "addrspace.h"

extern void StartProcess_2(int id);

//constuctor
PCB :: PCB()
{
	this->joinsem = new Semaphore("joinsem",0);
	this->exitsem = new Semaphore("exitsem",0);
	this->multex = new Semaphore("multex",1);
	this->pid = 0;
	this->parentID = -1;
	this->numwait = 0;
	this->exitcode = 0;
	this->thread = NULL;

}

PCB :: PCB(int id)
{
	this->joinsem = new Semaphore("joinsem",0);
	this->exitsem = new Semaphore("exitsem",0);
	this->multex = new Semaphore("multex",1);
	this->parentID = currentThread->processID;
	this->pid = id;
	this->numwait = 0;
	this->exitcode = 0;
	this->thread = NULL;

}
//deconstructor
PCB :: ~PCB()
{
	if(joinsem != NULL)
		delete this->joinsem;
	if(exitsem != NULL)
		delete this->exitsem;
	if(multex != NULL)
		delete this->multex;
	// giải phóng tiến trình
	if(thread != NULL)
	{
		if (thread->space != NULL )
		 thread->space = NULL;
		thread->Finish();
	}
	this->pid = 0;
	this->numwait = 0;
	this->exitcode = 0;
}
int PCB :: GetID()
{
	return this->pid;
}

int PCB::GetNumWait()
{
	return this->numwait;
}

int PCB :: GetExitCode()
{
	return this->exitcode;
}

void PCB::SetExitCode(int ec)
{
	this->exitcode = ec;
}

void PCB :: JoinWait()
{
	joinsem->P();
}

void PCB :: JoinRelease()
{
	joinsem->V();
}

void PCB :: ExitWait()
{
	exitsem->P();
}

void PCB :: ExitRelease()
{
	exitsem->V();
}

void PCB :: IncNumWait()
{
	multex->P();
	numwait++;
	multex->V();
}

void PCB :: DecNumWait()
{
	multex->P();
	if(numwait > 0)
		numwait--;
	multex->V();
}

void PCB :: SetFileName(char* fn)
{
	strcpy(FileName,fn);
}

char* PCB :: GetFileName()
{
	return FileName;
}

int PCB :: Exec(char* filename,int id)
{
//mutex->P(); để giúp tránh tình trạng nạp 2 tiếntrình cùng 1 lúc.
	multex->P();
	// tạo một tiến trình mới.
	this->thread = new Thread(filename);
	// kiểm tra có tạo được hay không?
	//Kiểm tra thread đã khởi tạo thành công chưa, nếu chưa thì báo lỗi là không đủ bộ nhớ, gọi mutex->V() và return.
	if(this->thread == NULL)
	{
		printf("can' not create thread new \n");
		multex->V();
		return -1;
	}
	//Đặt processID của thread này là id
	this->thread->processID = id;
	//Đặt parrentID của thread này là processID của thread gọi thực thi Exec 
	this->parentID = currentThread->processID;
//Gọi thực thi Fork(StartProcess_2,id) => Ta cast thread thành kiểu int, sau đó khi xử ký hàm 
//StartProcess ta cast Thread về đúng kiểu của nó. 
	this->thread->Fork(StartProcess_2,id);
	multex->V();
	//Trả về id. 
	return id;

}
