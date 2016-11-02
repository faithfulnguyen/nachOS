#ifndef STABLE_H
#define STABLE_H
#include "synch.h"
#include "bitmap.h"

#define MAX_SEMAPHORE 10

class Sem
{
	private:
		char name[50];
		Semaphore *sem;
	public :
	// khởi tạo đối tượng Sem. Gán giá trị ban đầu là null 
		Sem(char* na,int i)
		{
			strcpy(this->name,na);
			sem = new Semaphore(name,i);
		}
		// hủy các đối tượng đã tạo
		~Sem()
		{
			delete sem;
		}
		// thực hiện thao tác chờ
		void wait()
		{
			sem->P();
		}
		// thực hiện thao tác giải phóng Semaphore
		void signal()
		{
			sem->V();
		}
		// Trả về tên của Semaphore
		char* GetName()
		{
			return name;
		}
		
		

};

class STable
{
	private:
		BitMap* bm; //quản lý slot trống
		Sem* semTab[MAX_SEMAPHORE]; // quản lý tối đa 10 đối tượng Sem
	public:
		// khởi tạo size đối tượng Sem để quản lý 10 Semaphore. Gán giá trị ban đầu là null 
		// nhớ khởi tạo bm để sử dụng
		STable();
		// hủy các đối tượng đã tạo
		~STable();
		// Kiểm tra Semaphore “name” chưa tồn tại thì tạo Semaphore mới. Ngược lại, báo lỗi.
		int Create(char* name,int init);
		// Nếu tồn tại Semaphore “name” thì gọi this->P()để thực thi, ngược lại, báo lỗi.
		int Wait(char* name);
		// Nếu tồn tại Semaphore “name” thì gọi this->V()để thực thi, ngược lại, báo lỗi.
		int Signal(char* name);
		// Tìm slot trống.
		int FindFreeSlot(int id);
		// kiem tra nam co ton tai hay chua?		
		bool testname(char* n);
};
#endif 
