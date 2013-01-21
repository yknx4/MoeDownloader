#define THREADING_MACRO Threads->Pages[ActualThread]=ActualPage; \
ParameterizedThreadStart^ ThreadDelegate = gcnew ParameterizedThreadStart( &DownloadFiles) ; \
Threads->Threads[ActualThread]=gcnew Thread(ThreadDelegate); \
Threads->Threads[ActualThread]->Start(Threads->Pages[ActualThread]);\
Console::WriteLine("Downloading page "+ActualPage+" of "+TotalPages);

#define THREADING_MACRO_DEBUGGING String^ Debug = gcnew String(""); \
Debug="Thread "+(ActualThread+1)+" Started"; \
Console::WriteLine(Debug); \
Debug="Downloading page "+ActualPage+" of "+TotalPages;\
Console::WriteLine(Debug); 
//Thread::Sleep( 0 ); 
//Console::WriteLine(ActualPage);

#define  THREADING_MACRO_JOINER Threads->Threads[ActualThread]->Join();

#define  THREADING_MACRO_JOINER_DEBUGGING String^ Debug = gcnew String("");\
Debug="Thread "+(ActualThread+1)+" Terminated";\
Console::WriteLine(Debug);