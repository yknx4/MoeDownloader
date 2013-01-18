// MoeDownloader.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
//#define DEBUGGING
using namespace System;
using namespace System::Net;
using namespace System::Threading;
using namespace HtmlAgilityPack;
ref class YandereDownloader
{
public:

	// The ThreadProc method is called when the thread starts.
	// It loops ten times, writing to the console and yielding 
	// the rest of its time slice each time, and then ends.
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

	//void ParseHtmlTags(String^ HtmlContent){
	void ParseHtmlTags(Object^ HtmlContent2){
		String^ HtmlContent=gcnew String(HtmlContent2->ToString());
		String^ TempUrl;
		HtmlDocument doc;
		doc.LoadHtml(HtmlContent);
		HtmlNode^ nodo_p = doc.DocumentNode;
		HtmlNodeCollection^ nodos_a = nodo_p->SelectNodes("//a[@class='directlink largeimg']");
		for each (HtmlNode^ var in nodos_a)
		{
			TempUrl=var->GetAttributeValue("href","");
			Console::WriteLine(TempUrl);
			//Console::WriteLine();
		}
		//Console::WriteLine(HtmlContent2->ToString());
		Thread::Sleep( 0 );
	}

};


int _tmain(int argc, _TCHAR* argv[])
{
	Console::BackgroundColor=ConsoleColor::White;
	Console::ForegroundColor=ConsoleColor::Black;
	Console::Title="Yande.re Batch image downloader";
	array<String^>^args = Environment::GetCommandLineArgs();
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
		int ActualPage=0;
		bool Condition_Satisfied=0;
	/*	array<String^>^Tags=gcnew array<String^>(args->Length);
		Array::Copy(args,Tags,args->Length);
		Console::WriteLine(Tags->Length);*/
		Array::Reverse(args);
		Array::Resize(args,args->Length -1);
		array<Thread^>^Threads = gcnew array<Thread^>(0);
		array<YandereDownloader^>^DownloaderReferences = gcnew array<YandereDownloader^>(0);
		do
		{
			ActualPage++;
			//String^ Tags = gcnew String("shijou_takane");
			String^ texto = YandereDownloader::ReadYande_re(ActualPage,args);
			if (!(texto==nullptr))
			{
				//Nobody here but us chickens!
				if (!(texto->Contains("Nobody here but us chickens!")))
				{
					Array::Resize(Threads,ActualPage);
					Array::Resize(DownloaderReferences,ActualPage);
					DownloaderReferences[ActualPage-1]=gcnew YandereDownloader;
					//YandereDownloader^ Test = gcnew YandereDownloader;
					ParameterizedThreadStart^ ThreadDelegate = gcnew ParameterizedThreadStart(DownloaderReferences[ActualPage-1], &YandereDownloader::ParseHtmlTags) ;
					Threads[ActualPage-1]=gcnew Thread(ThreadDelegate);
					Threads[ActualPage-1]->Start(texto);
					Thread::Sleep( 0 );
					//Threads[ActualPage-1]->Start(ActualPage);

					#ifdef DEBUGGING
					Console::WriteLine(ActualPage);
					Console::WriteLine(texto->Contains("<span class=\"next_page disabled\">"));
					#endif
					Condition_Satisfied = !(texto->Contains("<span class=\"next_page disabled\">"));

				}else
				{
					Console::WriteLine("Nobody here but us chickens!");
				}
				}
		} while (Condition_Satisfied);
		Console::ResetColor();
		return 0;
	}
	
}
