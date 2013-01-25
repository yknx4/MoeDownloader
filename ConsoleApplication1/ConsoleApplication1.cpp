// ConsoleApplication1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#using "mscorlib.dll"
#using "System.dll"
using namespace System;
using namespace System::Threading;
ref class MyClass
{
public:
static void lalala(Object^ a){
	Console::WriteLine("Thread "+(int)a);
}	

private:

};

int _tmain(int argc, _TCHAR* argv[])
{
	int count=0;
	/*ParameterizedThreadStart^ ThreadDelegate = gcnew ParameterizedThreadStart( &DownloadFiles) ; \
		Threads->Threads[ActualThread]=gcnew Thread(ThreadDelegate); \
		Threads->Threads[ActualThread]->Start(Threads->Pages[ActualThread]);\*/
	Thread^ Test;
	while (true)
	{
		Test =  gcnew Thread(gcnew ParameterizedThreadStart(&MyClass::lalala));
		Test->Start(count);
		//Thread::Sleep(1000);
		Test->Join();
		count++;
	}
	return 0;
}

