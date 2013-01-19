#define THREADING_MACRO Threads->Parameters[ActualThread].page=ActualPage; \
ParameterizedThreadStart^ ThreadDelegate = gcnew ParameterizedThreadStart( &DanbooruDownloader::DownloadFiles) ; \
Threads->Threads[ActualThread]=gcnew Thread(ThreadDelegate); \
Threads->Threads[ActualThread]->Start(Threads->Parameters[ActualThread]);

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