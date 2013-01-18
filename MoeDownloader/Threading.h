#define THREADING_MACRO Array::Resize(Threads,ActualPage); \
Array::Resize(DownloaderReferences,ActualPage); \
Array::Resize(Parameters,ActualPage); \
DownloaderReferences[ActualPage-1]=gcnew YandereDownloader; \
Parameters[ActualPage-1].page=ActualPage; \
Parameters[ActualPage-1].tags=args; \
ParameterizedThreadStart^ ThreadDelegate = gcnew ParameterizedThreadStart(DownloaderReferences[ActualPage-1], &YandereDownloader::ParseHtmlTags) ; \
Threads[ActualPage-1]=gcnew Thread(ThreadDelegate); \
Threads[ActualPage-1]->Start(Parameters[ActualPage-1]);

#define THREADING_MACRO_DEBUGGING String^ Debug = gcnew String(""); \
Debug="Thread "+ActualPage+" Started"; \
Console::WriteLine(Debug); \
Thread::Sleep( 0 ); \
Console::WriteLine(ActualPage);

#define  THREADING_MACRO_JOINER Threads[ActualThread]->Join();

#define  THREADING_MACRO_JOINER_DEBUGGING String^ Debug = gcnew String("");\
Debug="Thread "+(ActualThread+1)+" Terminated";\
Console::WriteLine(Debug);