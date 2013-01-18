// MoeDownloader.cpp : Defines the entry point for the console application.
//
//
//

#include "stdafx.h"
//#define DEBUGGING //Uncomment this line to enable special debugging procedures
using namespace System;
using namespace System::Net;
using namespace System::Threading;
using namespace HtmlAgilityPack;
value struct ReadYande_reParameters //Defines a Struc with YandereDownloader::ReadYande_re parameters needance
{
	int page; 
	array<String^>^tags;
};
ref class YandereDownloader //MainClass yandere downloader
{
public:
	static String^ ReadYande_re(int page, array<String^>^tags) //Function to get RAW html source code from site with 'page' and 'tags' as source
	{
		String^ Page_Data;//RAW Html string initialization
		String^ FinalUrl= gcnew String("https://yande.re/post?page=");//URL String initialization
		FinalUrl += page.ToString() + "&tags=";//Add page and "&tags=" to URL
		for each (String^ tag in tags)//Cycle to add each tag to URL
		{
			FinalUrl+=tag+"+";//each tags must be separated by '+' so each tag is added to URL after '+'
		}
		FinalUrl=FinalUrl->Remove(FinalUrl->Length-1);//removes the extra '+' from URL output
#ifdef DEBUGGING
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
#ifdef DEBUGGING
			Console::WriteLine("Cannot connect to Host {0}", e); //Detailed error code
#endif
#ifndef DEBUGGING
			Console::WriteLine("Cannot connect to Host");
			Console::WriteLine("Is Internet connection alive?");
#endif
		}

		return Page_Data; //Returns RAW HTLM string
	}
	static int GetPagesNumber(array<String^>^tags) //Function to get the number of pages the tags have
	{
		String^ texto = YandereDownloader::ReadYande_re(1,tags);//Put RAW HTML in the String Texto
		if (!(texto==nullptr)) //If the RAW HTML exist, do following code
		{
			String^ PageTemp; // Create string pointer to get inner text from Html Node
			HtmlDocument doc; //initialize HTML Documents
			doc.LoadHtml(texto);//load Html from RAW Html string
			HtmlNode^ nodo_p = doc.DocumentNode; //Get Nodes from HTML Doc
			HtmlNodeCollection^ nodos_a = nodo_p->SelectNodes("//div[@class='pagination']/a"); //Select 'a' nodes from 'div' with class='pagination'
			try //Try to read nodes
			{
				nodos_a->RemoveAt(nodos_a->Count-1); //Remove an extra node that doesn't fullfill our needance
				for each (HtmlNode^ var in nodos_a) // Cycle for each node
				{
					PageTemp=var->InnerText; //Put the data in PageTemp variable
#ifdef DEBUGGING
					Console::WriteLine(TempUrl);
					Console::WriteLine();
#endif
				}
				return Convert::ToInt32(PageTemp,10); //Return the last PageTemp, and so the biggest number, meanning the last page
			}
			catch (NullReferenceException^ e) //If no div availavable, means it just have 1 page
			{
				return 1;	//return 1 page
			}
			return 1;//in any case return 1 page, to avoid program crash, further errors are handled after
		}
	}
	void ParseHtmlTags(Object^ data){//function to Parse Html Tags and call download
		ReadYande_reParameters^ Parameteres =(ReadYande_reParameters^)data; //Convert input object point to a ReadYandereParameters Struct pointer and direct it to object
		String^ HtmlContent=YandereDownloader::ReadYande_re(Parameteres->page,Parameteres->tags); //Get Raw HTML with needed parameters
		String^ TempUrl; //Initialize string to store URL
		HtmlDocument doc; //initialize HTML Documents
		doc.LoadHtml(HtmlContent);//load Html from RAW Html string
		HtmlNode^ nodo_p = doc.DocumentNode; //Get Nodes from HTML Doc
		HtmlNodeCollection^ nodos_a = nodo_p->SelectNodes("//a[@class='directlink largeimg'] | //a[@class='directlink smallimg']"); //Select all nodes with direct image link
		Console::WriteLine("Downloading "+nodos_a->Count+" files.");
		for each (HtmlNode^ var in nodos_a)
		{
			TempUrl=var->GetAttributeValue("href","");
#pragma region Make Extra Function
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
					Console::WriteLine("Error {0}", e);
					Console::WriteLine(FilePath);
#endif

				}
				FilePath="files/"+FilePath+FileExtension;
				if (!(IO::File::Exists(FilePath)))
				{
					WebClient^ Host_Reader = gcnew WebClient;
					Host_Reader->Headers->Add("user-agent", USER_AGENT_STRING);
					Console::WriteLine("Downloading "+FilePath);
#ifndef DEBUGGING
					Host_Reader->DownloadFile(Link,FilePath);  
#endif // !DEBUGGING

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
#pragma endregion Make Extra Function
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
			int ActualThread = 0;
			array<Thread^>^Threads = gcnew array<Thread^>(0);
			array<YandereDownloader^>^DownloaderReferences = gcnew array<YandereDownloader^>(0);
			array<ReadYande_reParameters>^Parameters = gcnew array<ReadYande_reParameters>(0);
			for (int tg=0;tg<ThreadGroups;tg++){
				for (int th = 0; th < NUMBER_OF_THREADS; th++)
				{
					ActualPage=tg*NUMBER_OF_THREADS+th+1;
					THREADING_MACRO
#ifdef DEBUGGING
					THREADING_MACRO_DEBUGGING
#endif // DEBUGGING
				}
#ifdef DEBUGGING
				if (ThreadRemainder==0)
				{
					Console::WriteLine("All Threads Started");
				}
#endif // DEBUGGING
				for (int tw=0;tw<NUMBER_OF_THREADS;tw++)
				{
					ActualThread=tg*NUMBER_OF_THREADS+tw;
					THREADING_MACRO_JOINER
#ifdef DEBUGGING
					THREADING_MACRO_JOINER_DEBUGGING
#endif // DEBUGGING
				}
			}
				if (ThreadRemainder>0){
					for (int tr=0;tr<ThreadRemainder;tr++)
					{
						ActualPage =ThreadGroups*NUMBER_OF_THREADS+tr+1;
						THREADING_MACRO
#ifdef DEBUGGING
						THREADING_MACRO_DEBUGGING
#endif // DEBUGGING
					}
#ifdef DEBUGGING
					Console::WriteLine("All Threads Started");
#endif // DEBUGGING
					for (int trj=0;trj<ThreadRemainder;trj++)
					{
						ActualThread=ThreadGroups*NUMBER_OF_THREADS+trj;
						THREADING_MACRO_JOINER
#ifdef DEBUGGING
						THREADING_MACRO_JOINER_DEBUGGING
#endif // DEBUGGING
					}
				}

#ifdef DEBUGGING
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
	
}
