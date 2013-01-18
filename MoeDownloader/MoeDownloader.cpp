// MoeDownloader.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
//#define DEBUGGING
using namespace System;
using namespace System::Net;
using namespace System::Threading;
using namespace HtmlAgilityPack;
value struct ReadYande_reParameters
{
	int page; 
	array<String^>^tags;
};
ref class YandereDownloader
{
public:
	static String^ ReadYande_re(int page, array<String^>^tags)
	{
		String^ Page_Data;
		String^ FinalUrl= gcnew String("https://yande.re/post?page=");
		FinalUrl += page.ToString() + "&tags=";
		for each (String^ tag in tags)
		{
			FinalUrl+=tag+"+";
		}
		FinalUrl=FinalUrl->Remove(FinalUrl->Length-1);
#ifdef DEBUGGING
		System::Console::WriteLine(FinalUrl);
		//System::Console::ReadKey();
		//System::Console::Clear();
#endif
		try
		{
			WebClient^ Host_Reader = gcnew WebClient;
			Host_Reader->Headers->Add("user-agent", "Mozilla/5.0 (Windows NT 5.1) AppleWebKit/536.5 (KHTML, like Gecko) Chrome/19.0.1084.56 Safari/536.5");
			Page_Data = Host_Reader->DownloadString(FinalUrl);		
		}
		catch (WebException^ e)
		{
			Console::ForegroundColor=ConsoleColor::Red;
			Console::BackgroundColor=ConsoleColor::Black;
#ifdef DEBUGGING
			Console::WriteLine("Cannot connect to Host {0}", e);
#endif
#ifndef DEBUGGING
			Console::WriteLine("Cannot connect to Host");
			Console::WriteLine("Is Internet connection alive?");
#endif
		}

		return Page_Data;
	}
	static int GetPagesNumber(array<String^>^tags)
	{
		String^ texto = YandereDownloader::ReadYande_re(1,tags);
		if (!(texto==nullptr))
		{
			String^ TempUrl;
			HtmlDocument doc;
			doc.LoadHtml(texto);
			HtmlNode^ nodo_p = doc.DocumentNode;
			HtmlNodeCollection^ nodos_a = nodo_p->SelectNodes("//div[@class='pagination']/a");
			try
			{
				nodos_a->RemoveAt(nodos_a->Count-1);
				for each (HtmlNode^ var in nodos_a)
				{
					TempUrl=var->InnerText;
#ifdef DEBUGGING
					//Console::WriteLine(TempUrl);
					//Console::WriteLine();
#endif
				}
				return Convert::ToInt32(TempUrl,10);
			}
			catch (NullReferenceException^ e)
			{
				return 1;	
			}
			return 1;
		}
	}
	void ParseHtmlTags(Object^ data){
		ReadYande_reParameters^ Parameteres =(ReadYande_reParameters^)data;
		String^ HtmlContent=YandereDownloader::ReadYande_re(Parameteres->page,Parameteres->tags);
		String^ TempUrl;
		HtmlDocument doc;
		doc.LoadHtml(HtmlContent);
		HtmlNode^ nodo_p = doc.DocumentNode;
		HtmlNodeCollection^ nodos_a = nodo_p->SelectNodes("//a[@class='directlink largeimg'] | //a[@class='directlink smallimg']");
		Console::WriteLine("Downloading "+nodos_a->Count+" files.");
		for each (HtmlNode^ var in nodos_a)
		{
			TempUrl=var->GetAttributeValue("href","");
			Uri^ Link=gcnew Uri(TempUrl);
			try
			{
				String^ FilePath = gcnew String("");
				String^ FileExtension = gcnew String("");
				FilePath=Link->Segments[3];
				FileExtension=FilePath->Substring(FilePath->Length-4,4);
				FilePath=FilePath->Replace("yande.re%20","");
				FilePath=Uri::UnescapeDataString(FilePath);
				FilePath=FilePath->Substring(0,FilePath->Length-4);
				try
				{
					FilePath=FilePath->Substring(0,160);
				}
				catch (System::ArgumentOutOfRangeException^ e)
				{
#ifdef DEBUGGING
					Console::WriteLine("Error ", e);
					Console::WriteLine(FilePath);
#endif

				}
				FilePath="files/"+FilePath+FileExtension;
				if (!(IO::File::Exists(FilePath)))
				{
					WebClient^ Host_Reader = gcnew WebClient;
					Host_Reader->Headers->Add("user-agent", "Mozilla/5.0 (Windows NT 5.1) AppleWebKit/536.5 (KHTML, like Gecko) Chrome/19.0.1084.56 Safari/536.5");
					Console::WriteLine("Downloading "+FilePath);
					Host_Reader->DownloadFile(Link,FilePath);
				} 
				else
				{
					//Console::WriteLine(FilePath+" Already Exists");
					Console::WriteLine("Already Exists");
				}
					
			}
			catch (WebException^ e)
			{
				Console::ForegroundColor=ConsoleColor::Red;
				Console::BackgroundColor=ConsoleColor::Black;
#ifdef DEBUGGING
				Console::WriteLine("Cannot connect to Host {0}", e);
#endif
#ifndef DEBUGGING
				Console::WriteLine("Cannot Download {0}", TempUrl);
#endif
			}
		}
		Thread::Yield();
	}

};



int _tmain(int argc, _TCHAR* argv[])
{
	Console::BackgroundColor=ConsoleColor::White;
	Console::ForegroundColor=ConsoleColor::Black;
	Console::Title="Yande.re Batch image downloader";
	array<String^>^args = Environment::GetCommandLineArgs();
	bool CheckTags=0;
	if ( args == nullptr || args->Length == 1 )
	{
		Console::BackgroundColor=ConsoleColor::Red;
		Console::WriteLine("Specify the tags to crawl");
		Console::WriteLine("Usage: MoeDownloader moe suzumiya_haruhi");
		Console::WriteLine("To download pictures with tags moe and suzumiya_haruhi");
		Console::WriteLine("Remember Yande.re threats spaces as underline score");
		return 1;
	}else
	{
		Array::Reverse(args);
		Array::Resize(args,args->Length -1);
		CheckTags=YandereDownloader::ReadYande_re(1,args)->Contains("<a class=\"directlink largeimg\"")||YandereDownloader::ReadYande_re(1,args)->Contains("<a class=\"directlink smallimg\"");
		if (CheckTags)
		{
			int TotalPages =YandereDownloader::GetPagesNumber(args);
			int ThreadGroups = TotalPages/NUMBER_OF_THREADS;
			int ThreadRemainder = TotalPages % NUMBER_OF_THREADS;
			int ActualPage = 0;
			array<Thread^>^Threads = gcnew array<Thread^>(0);
			array<YandereDownloader^>^DownloaderReferences = gcnew array<YandereDownloader^>(0);
			array<ReadYande_reParameters>^Parameters = gcnew array<ReadYande_reParameters>(0);
			for (int tg=0;tg<ThreadGroups;tg++){
				for (int th = 0; th < NUMBER_OF_THREADS; th++)
				{
					ActualPage=tg*NUMBER_OF_THREADS+th+1;
					Array::Resize(Threads,ActualPage);
					Array::Resize(DownloaderReferences,ActualPage);
					Array::Resize(Parameters,ActualPage);
					DownloaderReferences[ActualPage-1]=gcnew YandereDownloader;
					Parameters[ActualPage-1].page=ActualPage;
					Parameters[ActualPage-1].tags=args;
					ParameterizedThreadStart^ ThreadDelegate = gcnew ParameterizedThreadStart(DownloaderReferences[ActualPage-1], &YandereDownloader::ParseHtmlTags) ;
					Threads[ActualPage-1]=gcnew Thread(ThreadDelegate);
					Threads[ActualPage-1]->Start(Parameters[ActualPage-1]);
#ifdef DEBUGGING
					String^ Debug = gcnew String("");
					Debug="Thread "+ActualPage+" Started";
					Console::WriteLine(Debug);
					Thread::Sleep( 0 );
					Console::WriteLine(ActualPage);
#endif

				}
				for (int tw=0;tw<NUMBER_OF_THREADS;tw++)
				{
					
					Threads[tg*NUMBER_OF_THREADS+tw]->Join();
#ifdef DEBUGGING
					String^ Debug = gcnew String("");
					Debug="Thread "+tg*NUMBER_OF_THREADS+tw+1+" Terminated";
					Console::WriteLine(Debug);
#endif // DEBUGGING
				}
			}
				if (ThreadRemainder>0){
					for (int tr=0;tr<ThreadRemainder;tr++)
					{
						ActualPage =ThreadGroups*NUMBER_OF_THREADS+tr+1;
						Array::Resize(Threads,ActualPage);
						Array::Resize(DownloaderReferences,ActualPage);
						Array::Resize(Parameters,ActualPage);
						DownloaderReferences[ActualPage-1]=gcnew YandereDownloader;
						Parameters[ActualPage-1].page=ActualPage;
						Parameters[ActualPage-1].tags=args;
						ParameterizedThreadStart^ ThreadDelegate = gcnew ParameterizedThreadStart(DownloaderReferences[ActualPage-1], &YandereDownloader::ParseHtmlTags) ;
						Threads[ActualPage-1]=gcnew Thread(ThreadDelegate);
						Threads[ActualPage-1]->Start(Parameters[ActualPage-1]);
#ifdef DEBUGGING
						String^ Debug = gcnew String("");
						Debug="Thread "+ActualPage+" Started";
						Console::WriteLine(Debug);
						Thread::Sleep( 0 );
						Console::WriteLine(ActualPage);
#endif
					}
				}

#ifdef DEBUGGING
			Console::WriteLine("All Threads Started");
#endif // DEBUGGING


		} 
		else
		{
			Console::WriteLine("Nothing here but us Chickens!");
		}
		
		Console::ResetColor();
		return 0;
	}
	
}
