#include "../userprog/ptable.h"
#include "system.h"
PTable :: PTable(int size)
{
	this->psize = size;
	this->bm = new BitMap(size);
	this->bmsem = new Semaphore("bmsem",1);
	for(int i = 0 ; i < MAX_PROCESS ; ++i)
		pcb[i] = NULL;
	bm->Mark(0);
	pcb[0] = new PCB(0);
	pcb[0]->SetFileName("./test/shell");
	pcb[0]->parentID = -1;
}

PTable :: ~PTable()
{
	
	if( bm != NULL )
		delete []bm;
	for(int i = 0; i < psize ; i++)
		if(pcb[i] != NULL)
			delete pcb[i];
	if( bmsem != NULL)
		delete bmsem;
}

//////////////////////////////////////////////////////////////////////////

int PTable :: ExecUpdate(char* name)
{
//Gọi mutex->P(); để giúp tránh tình trạng nạp 2 tiếntrình cùng 1 lúc
	bmsem->P();
	//Kiểm tra tính hợp lệ của chương trình “name”.
	if(name == NULL)
	{
		printf("PTable: Name not valid \n");
		bmsem->V();
		return -1;
	}

	
	//Kiểm tra sự tồn tại của chương trình “name” bằng cách gọi phương thức Open của lớp fileSystem
	OpenFile* op = fileSystem->Open( name );
	if ( op == NULL )
	{
		printf("PTable: File can't not Open\n");
		bmsem->V();
		return -1;
	}
	//kiểm tra chương trình đang chạy có gọi lại chính nó không?
	if( strcmp(name,"./test/shell") == 0 )
	{
		printf("PTable: Can't not exec itself \n");		
		bmsem->V();
		return -1;
	}
//So sánh tên chương trình và tên của currentThread để chắc chắn rằng chương trình này không gọi thực thi chính nó. 
	if( strcmp(name,currentThread->getName()) == 0 )
	{
		printf("PTable: Can't not exec itself\n");		
		bmsem->V();
		return -1;
	}
		
	//Tìm slot trống trong bảng Ptable
	int id = this->GetFreeSlot();
	// kiểm tra tính hợp lệ của Id nhận được.
	if(id < 0)
	{
		printf("PTable : id not valid \n");
		bmsem->V();
		return -1;
	}
//Nếu có slot trống thì khởi tạo một PCB mới với processID chính là index của slot này, parrentID là processID của currentThread.
	pcb[id] = new PCB(id);
	//Đánh dấu đã sử dụng
	pcb[id]->SetFileName(name);
	
	//Gọi thực thi phương thức Exec của lớp PCB
	int pid = pcb[id]->Exec(name,id);
	//Gọi bmsem->V();
	bmsem->V();
	//Trả về kết quả thực thi của PCB->Exec.
	return pid;
}

int PTable :: JoinUpdate(int id)
{
	//Ta kiểm tra tính hợp lệ của processID id và kiểm tra tiến trình gọi Join có phải là cha của tiến trình có processID là id hay không. Nếu không thỏa, ta báo lỗi hợp lý và trả về -1. 
	if(id < 0)
	{
		printf("\nPTable::JoinUpdate : id = %d",id);
		return -1;
	}
	// kiểm tra tiến trình đang chạy là cha của tiến trình cần join hay không?
	if(currentThread->processID != pcb[id]->parentID)
	{
		printf("\nPTable: can't join this process \n");
		return -1;
	}

//Tăng numwait và gọi JoinWait() để chờ tiến trình con thực hiện.
	pcb[pcb[id]->parentID]->IncNumWait();
	pcb[id]->JoinWait();
	
	//Xử lý exitcode.
	int ec = pcb[id]->GetExitCode();

//ExitRelease() để cho phép tiến trình con thoát.
	pcb[id]->ExitRelease();

	// trả về exit code
	return ec;

}

int PTable :: ExitUpdate(int ec)
{

	//Nếu tiến trình gọi là main process thì gọi Halt()
	//id main process được khởi tạo là 0.
	int id = currentThread->processID;
	if(id == 0)
	{
		interrupt->Halt();
		return 0;
	}
	//kiểm tra tính hợp lệ của Id nhận được.
	if(IsExist(id) == false)
	{
		printf("\nPTable::ExitUpdate : ID %d khong ton tai ",id);
		return -1;
	}
	
	//gọi SetExitCode để đặt exitcode cho tiến trình gọi.
	pcb[id]->SetExitCode(ec);
	// giảm số tiến trình chờ đợi;
	pcb[pcb[id]->parentID]->DecNumWait();

	//Gọi JoinRelease để giải phóng tiến trình cha đang đợi nó(nếu có )
	printf("PTable: release thread parent \n");
	pcb[id]->JoinRelease();
	//ExitWait() để xin tiến trình cha cho phép thoát
	printf("PTable: exit thread \n");
	pcb[id]->ExitWait();
	Remove(id);
	return ec;
}
int PTable :: GetFreeSlot()
{
	return this->bm->Find();
}

bool PTable :: IsExist(int pid)
{
	return this->bm->Test(pid);
		
}
void PTable :: Remove(int pid)
{
	this->bm->Clear(pid);
	if(pcb[pid] != NULL)
		delete pcb[pid];
}

char* PTable :: GetFileName(int id)
{
	return ( pcb[id]->GetFileName());
}
