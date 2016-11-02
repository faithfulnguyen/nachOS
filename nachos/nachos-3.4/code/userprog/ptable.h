//ptable.h
#ifndef PTABLE_H
#define PTABLE_H

#include "bitmap.h"
#include "pcb.h"
#include "synch.h"


#define MAX_PROCESS 10

class PTable
{
	private:
		BitMap *bm; // đánh dấu các vị trí đã được sử dụng trong pcb 
		PCB* pcb[MAX_PROCESS];
		int psize;
		Semaphore* bmsem;// dùng để ngăn chặn trường hợp nạp 2 tiến trình cùng lúc
		
	public:
	// khởi tạo size đối tượng PCB để lưu size process.Gán giá trị ban đầu là null 
// nhớ khởi tạo bm và bmsem để sử dụng 
		PTable(int size);
		
		~PTable();//huy cac doi tuong da tao
		
		int ExecUpdate(char* name); // Xử lý cho system call SC_Exit 
		int ExitUpdate(int ec);// Xử lý cho system call SC_Exit

		int JoinUpdate(int id);// Xử lý cho system call SC_Join 
		
		// tìm free slot để lưu thông tin cho tiến trình mới 
		int GetFreeSlot();

		// kiểm tra tồn tại processID này không? 
		bool IsExist(int pid);

	// khi tiến trình kết thúc, delete processID ra khỏi mảng quản lý nó 
		void Remove(int pid);
		// Trả về tên của tiến trình 
		char* GetFileName(int id);
};
#endif 
