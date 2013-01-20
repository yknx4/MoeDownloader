// MoeDownloader.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
//#define _DEBUG //Uncomment this line to enable special debugging procedures
using namespace System;
using namespace System::Net;
using namespace System::Threading;
using namespace HtmlAgilityPack;
ref class Yandere_Containers
{
public:
	value struct ReadYande_reParameters //Defines a Struc with YandereDownloader::ReadYande_re parameters needance
	{
		int page; 
		array<String^>^tags;
	};
	value struct ThreadGroup {
		array<Thread^>^Threads;
		//array<YandereDownloader^>^DownloaderReferences;
		array<ReadYande_reParameters>^Parameters;
	};
};
ref class YandereDownloader //MainClass yandere downloader
{
public:
	static String^ ReadYande_re(int page, array<String^>^tags) //Function to get RAW html source code from site with 'page' and 'tags' as source
	{
		String^ Page_Data;//RAW Html string initialization
		String^ FinalUrl= gcnew String(SITE_DOMAIN+"post?page=");//URL String initialization
		//String^ FinalUrl= gcnew String("http://konachan.com/post?page=");//URL String initialization
		FinalUrl += page.ToString() + "&tags=";//Add page and "&tags=" to URL
		for each (String^ tag in tags)//Cycle to add each tag to URL
		{
			FinalUrl+=tag+"+";//each tags must be separated by '+' so each tag is added to URL after '+'
		}
		FinalUrl=FinalUrl->Remove(FinalUrl->Length-1);//removes the extra '+' from URL output
#ifdef _DEBUG
		System::Console::WriteLine(FinalUrl);
#endif
		try
		{
			WebClient^ Host_Reader = gcnew WebClient; //Initialize the Webclient needed to parse HTML
			Host_Reader->Headers->Add("user-agent", USER_AGENT_STRING); //Add user agent header
			Page_Data = Host_Reader->DownloadString(FinalUrl);	//Download Source Code and put it in PageData String
		}
		catch (WebException^ e) //In case of Webexception the following code is run
		{
			Console::ForegroundColor=ConsoleColor::Red;
			Console::BackgroundColor=ConsoleColor::Black;
#ifdef _DEBUG
			Console::WriteLine("Cannot connect to Host {0}", e); //Detailed error code
#endif
#ifndef _DEBUG
			Console::WriteLine("Cannot connect to Host");
			Console::WriteLine("Is Internet connection alive?");
#endif
		}

		return Page_Data; //Returns RAW HTLM string
	};
	static int GetPagesNumber(array<String^>^tags) //Function to get the number of pages the tags have
	{
		String^ texto = YandereDownloader::ReadYande_re(1,tags);//Put RAW HTML in the String Texto
		if (!(texto==nullptr)) //If the RAW HTML exist, do following code
		{
			String^ PageTemp; // Create string pointer to get inner text from Html Node
			HtmlNodeCollection^ nodos_a = YandereDownloader::GetHtmlNodes(texto,"//div[@class='pagination']/a"); //Select 'a' nodes from 'div' with class='pagination'
			try //Try to read nodes
			{
				nodos_a->RemoveAt(nodos_a->Count-1); //Remove an extra node that doesn't fullfill our needance
				for each (HtmlNode^ var in nodos_a) // Cycle for each node
				{
					PageTemp=var->InnerText; //Put the data in PageTemp variable
				}
				return Convert::ToInt32(PageTemp,10); //Return the last PageTemp, and so the biggest number, meanning the last page
			}
			catch (NullReferenceException^ e) //If no div availavable, means it just have 1 page
			{
				return 1;	//return 1 page
			}
		}
		return 1;//in any case return 1 page, to avoid program crash, further errors are handled after
	};
	static void DownloadFiles(Object^ data){//function to Parse Html Tags and call download
		Yandere_Containers::ReadYande_reParameters^ Parameteres =(Yandere_Containers::ReadYande_reParameters^)data; //Convert input object point to a ReadYandereParameters Struct pointer and direct it to object
		array<String^>^test =gcnew array<String^>(1);
		String^ HtmlContent=YandereDownloader::ReadYande_re(Parameteres->page,Parameteres->tags); //Get Raw HTML with needed parameters
		String^ TempUrl; //Initialize string to store URL
		HtmlNodeCollection^ nodos_a = YandereDownloader::GetHtmlNodes(HtmlContent,"//a[@class='directlink largeimg'] | //a[@class='directlink smallimg']");; //Select all nodes with direct image link
		Console::WriteLine("Downloading "+nodos_a->Count+" files."); //Write the number of downloadable files
		for each (HtmlNode^ var in nodos_a)
		{
			TempUrl=var->GetAttributeValue("href","");
			Uri^ Link=gcnew Uri(TempUrl);
			try
			{
				String^ FilePath = gcnew String("");
				String^ FileExtension = gcnew String("");
				FilePath=Link->Segments[3];
				FileExtension=FilePath;
				FileExtension=FileExtension->Replace("jpeg","jpg");
				//FileExtension=FileExtension->Replace("?"+Links[i]->ID,"");
				FileExtension=FileExtension->Substring(FileExtension->Length-4,4);
				FilePath=FilePath->Replace(SITE_NAME+"%20","");
				FilePath=Uri::UnescapeDataString(FilePath);
				FilePath=FilePath->Join("",FilePath->Split(IO::Path::GetInvalidFileNameChars()));
				FilePath=FilePath->Substring(0,FilePath->Length-4);
				try
				{
					FilePath=FilePath->Substring(0,160);
				}
				catch (System::ArgumentOutOfRangeException^ e)
				{
				}
				FilePath=SITE_NAME+"/"+FilePath+FileExtension;
				if (!(IO::File::Exists(FilePath)))
				{
					WebClient^ Host_Reader = gcnew WebClient;
					Host_Reader->Headers->Add("user-agent", USER_AGENT_STRING);
					Console::WriteLine("Downloading "+FilePath);
#ifndef _DEBUG
					Host_Reader->DownloadFile(Link,FilePath);  
#endif // !DEBUGGING

				} 
				else
				{
					Console::WriteLine(FilePath+" Already Exists");
				}
					
			}
			catch (WebException^ e)
			{
				Console::ForegroundColor=ConsoleColor::Red;
				Console::BackgroundColor=ConsoleColor::Black;
#ifdef _DEBUG
				Console::WriteLine("Cannot connect to Host {0}", e);
#endif
#ifndef _DEBUG
				Console::WriteLine("Cannot Download {0}", TempUrl);
#endif
			}
		}
		Thread::Yield();
	};
	static HtmlNodeCollection^ GetHtmlNodes(String^ RawHtml,String^ Xpath){
		HtmlDocument doc; //initialize HTML Documents
		doc.LoadHtml(RawHtml);//load Html from RAW Html string
		HtmlNode^ nodo_p = doc.DocumentNode; //Get Nodes from HTML Doc
		return nodo_p->SelectNodes(Xpath); //Select all nodes with direct image link
	};
	static Yandere_Containers::ThreadGroup^ SetThreads(array<String^>^tags){
		Yandere_Containers::ThreadGroup^ Threads=gcnew Yandere_Containers::ThreadGroup;
		Threads->Threads = gcnew array<Thread^>(NUMBER_OF_THREADS);
		Threads->Parameters = gcnew array<Yandere_Containers::ReadYande_reParameters>(NUMBER_OF_THREADS);
		for (int th=0;th<NUMBER_OF_THREADS;th++)
		{
			Threads->Parameters[th].tags=tags;
		}
#ifdef _DEBUG
		for each (String^ var in Threads->Parameters[0].tags)
		{
			Console::WriteLine(var);
		}
#endif
		//array<YandereDownloader^>^DownloaderReferences = gcnew array<YandereDownloader^>(0);
		return Threads; //Select all nodes with direct image link
	};
	
};





int _tmain(int argc, _TCHAR* argv[])
{
	Console::BackgroundColor=ConsoleColor::White;
	Console::ForegroundColor=ConsoleColor::Black;
	Console::Title=SITE_NAME+" Batch image downloader";
	array<String^>^args = Environment::GetCommandLineArgs();
	bool CheckTags=0;
	if ( args == nullptr || args->Length == 1 )
	{
		Console::BackgroundColor=ConsoleColor::Red;
		Console::WriteLine("Specify the tags to crawl");
		Console::WriteLine("Usage: "+SITE_NAME+" moe suzumiya_haruhi");
		Console::WriteLine("To download pictures with tags moe and suzumiya_haruhi");
		Console::WriteLine("Remember "+SITE_NAME+" threats spaces as underline score");
		return 1;
	}else
	{
		Array::Reverse(args);
		Array::Resize(args,args->Length -1);
		CheckTags=YandereDownloader::ReadYande_re(1,args)->Contains("<a class=\"directlink largeimg\"")||YandereDownloader::ReadYande_re(1,args)->Contains("<a class=\"directlink smallimg\"");
#ifdef _DEBUG
		Console::WriteLine("Checking tags.... Returned "+CheckTags);
#endif // DEBUGGING
		if (CheckTags)
		{
			int TotalPages =YandereDownloader::GetPagesNumber(args);
			int ThreadGroups = TotalPages/NUMBER_OF_THREADS;
			int ThreadRemainder = TotalPages % NUMBER_OF_THREADS;
			int ActualPage = 0;
			int ActualThread = 0;
			Yandere_Containers::ThreadGroup^ Threads=YandereDownloader::SetThreads(args);
			/*for each (Yandere_Containers::ReadYande_reParameters var in Threads->Parameters)
			{
				Console::WriteLine(var.page);
				for each ( String^ var2 in var.tags)
				{
					Console::WriteLine(var2);
				}
			}*/
			for (int tg=0;tg<ThreadGroups;tg++){
				for (int th = 0; th < NUMBER_OF_THREADS; th++)
				{
					ActualThread=th;
					ActualPage=tg*NUMBER_OF_THREADS+th+1;
					THREADING_MACRO
#ifdef _DEBUG
					THREADING_MACRO_DEBUGGING
#endif // DEBUGGING
				}
#ifdef _DEBUG
				if (ThreadRemainder==0)
				{
					Console::WriteLine("All Threads Started");
				}
#endif // DEBUGGING
				for (int tw=0;tw<NUMBER_OF_THREADS;tw++)
				{
					ActualThread=tw;
					THREADING_MACRO_JOINER
#ifdef _DEBUG
					THREADING_MACRO_JOINER_DEBUGGING
#endif // DEBUGGING
				}
			}
				if (ThreadRemainder>0){
					for (int tr=0;tr<ThreadRemainder;tr++)
					{
						ActualThread=tr;
						ActualPage =ThreadGroups*NUMBER_OF_THREADS+tr+1;
						THREADING_MACRO
#ifdef _DEBUG
						THREADING_MACRO_DEBUGGING
#endif // DEBUGGING
					}
#ifdef _DEBUG
					Console::WriteLine("All Threads Started");
#endif // DEBUGGING
					for (int trj=0;trj<ThreadRemainder;trj++)
					{
						ActualThread=trj;
						THREADING_MACRO_JOINER
#ifdef _DEBUG
						THREADING_MACRO_JOINER_DEBUGGING
#endif // DEBUGGING
					}
				}

#ifdef _DEBUG
			Console::WriteLine("All Threads terminated");
#endif // DEBUGGING
		} 
		else
		{
			Console::WriteLine("Nothing here but us Chickens!");
		}
		
		Console::ResetColor();
		return 0;
	}
#ifdef _DEBUG
	Console::ReadKey();
#endif // _DEBUG
}
