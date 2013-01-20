// MoeDownloader.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
//#define DEBUGGING //Uncomment this line to enable special debugging procedures
using namespace System;
using namespace System::Net;
using namespace System::Threading;
using namespace HtmlAgilityPack;
ref class AditionalFunctions
{
public:
	static String^ GetRawHtml(String^ Url){
		//Console::WriteLine("GetRawHtml Called with: "+Url);
		String^ OutputHtml;
		try
		{
			WebClient^ Host_Reader = gcnew WebClient; //Initialize the Webclient needed to parse HTML
			Host_Reader->Headers->Add("user-agent", USER_AGENT_STRING); //Add user agent header
			OutputHtml=Host_Reader->DownloadString(Url);	//Download Source Code and put it in PageData String
			//Console::WriteLine(OutputHtml);
		}
		catch (WebException^ e)
		{
			throw e;
		}
		return OutputHtml;
	};
	static String^ GetRawHtml(Uri^ Url){
		//Console::WriteLine("GetRawHtml Called with: "+Url);
		String^ OutputHtml;
		try
		{
			WebClient^ Host_Reader = gcnew WebClient; //Initialize the Webclient needed to parse HTML
			Host_Reader->Headers->Add("user-agent", USER_AGENT_STRING); //Add user agent header
			OutputHtml=Host_Reader->DownloadString(Url);	//Download Source Code and put it in PageData String
			//Console::WriteLine(OutputHtml);
		}
		catch (WebException^ e)
		{
			throw e;
		}
		return OutputHtml;
	};

};
ref class Danbooru_Containers
{
public:
	value struct ReadDanbooruParameters //Defines a Struc with YandereDownloader::ReadYande_re parameters needance
	{
		int page; 
		array<String^>^tags;
	};
	value struct ThreadGroup {
		array<Thread^>^Threads;
		//array<YandereDownloader^>^DownloaderReferences;
		array<ReadDanbooruParameters>^Parameters;
	};
	value struct PostData {
		String^ Link;
		String^ ID;
		String^ Tags;
	};
};
ref class DanbooruDownloader //MainClass yandere downloader
{
public:
	//static String^ st_dm;
	static String^ ReadDanbooru(int page, array<String^>^tags) //Function to get RAW html source code from site with 'page' and 'tags' as source
	{
		//Console::WriteLine(st_dm);
		String^ Page_Data;//RAW Html string initialization
		String^ FinalUrl= gcnew String(SITE_DOMAIN+"/index.php?page=post&pid=");//URL String initialization
		FinalUrl += (page*PID_MULT).ToString() + "&s=list&tags=";//Add page and "&tags=" to URL
		for each (String^ tag in tags)//Cycle to add each tag to URL
		{
			FinalUrl+=tag+"+";//each tags must be separated by '+' so each tag is added to URL after '+'
		}
		FinalUrl=FinalUrl->Remove(FinalUrl->Length-1);//removes the extra '+' from URL output
#ifdef _DEBUG
		System::Console::WriteLine("ReadDanbooru function called with: "+FinalUrl);
#endif
		try
		{
			Page_Data=AditionalFunctions::GetRawHtml(FinalUrl);
			//Console::WriteLine(Page_Data);
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
		String^ texto = DanbooruDownloader::ReadDanbooru(0,tags);//Put RAW HTML in the String Texto
		if (!(texto==nullptr)) //If the RAW HTML exist, do following code
		{
			String^ PageTemp; // Create string pointer to get inner text from Html Node
			HtmlNodeCollection^ nodos_a = DanbooruDownloader::GetHtmlNodes(texto,"//a[@alt='last page']"); //Select 'a' nodes from 'div' with class='pagination'
			try //Try to read nodes
			{
				for each (HtmlNode^ var in nodos_a) // Cycle for each node
				{
					PageTemp=var->GetAttributeValue("href","false"); //Put the data in PageTemp variable
					//Console::WriteLine(PageTemp);
					PageTemp=PageTemp->Substring((PageTemp->IndexOf("pid=")+4));
					//Console::WriteLine(PageTemp);
				}
				return (Convert::ToInt32(PageTemp,10))/PID_MULT; //Return the last PageTemp, and so the biggest number, meanning the last page
			}
			catch (NullReferenceException^ e) //If no div availavable, means it just have 1 page
			{
				return 1;	//return 1 page
			}
		}
		return 1;//in any case return 1 page, to avoid program crash, further errors are handled after
	};
	static void DownloadFiles(Object^ data){//function to Parse Html Tags and call download
		Danbooru_Containers::ReadDanbooruParameters^ Parameteres =(Danbooru_Containers::ReadDanbooruParameters^)data; //Convert input object point to a ReadYandereParameters Struct pointer and direct it to object
		array<Danbooru_Containers::PostData^>^ Links=GetDownloadData(Parameteres->page,Parameteres->tags);
		int LinksCount = Links->Length;
		Console::WriteLine("Downloading "+LinksCount+" files."); //Write the number of downloadable files
		for (int i=0;i<LinksCount;i++)
		{
			if (Links[i]==nullptr)
			{
				continue;
			}
			//Console::WriteLine(Links[i]);
			Uri^ Link=gcnew Uri(Links[i]->Link);
			try
			{
				String^ FilePath = gcnew String("");
				String^ FileExtension = gcnew String("");
				FilePath=Links[i]->ID+" "+Links[i]->Tags;
				FilePath=FilePath->Join("",FilePath->Split(IO::Path::GetInvalidFileNameChars()));
				FileExtension=Links[i]->Link;
				FileExtension=FileExtension->Replace("jpeg","jpg");
				FileExtension=FileExtension->Replace("?"+Links[i]->ID,"");
				FileExtension=FileExtension->Substring(FileExtension->Length-4,4);
				//FilePath=FilePath->Replace("yande.re%20","");
				//FilePath=Uri::UnescapeDataString(FilePath);
				//FilePath=Links[i]->Link->Substring(0,FilePath->Length-4);
				try
				{
					FilePath=FilePath->Substring(0,140);
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
				Console::WriteLine("Cannot Download {0}", Links[i]->Link);
				Console::WriteLine("Because {0}", e);
#endif
			}
		}
		Thread::Yield();
	};
	static HtmlNodeCollection^ GetHtmlNodes(String^ RawHtml,String^ Xpath){
		HtmlDocument doc; //initialize HTML Documents
		doc.LoadHtml(RawHtml);//load Html from RAW Html string
		HtmlNode^ nodo_p = doc.DocumentNode; //Get Nodes from HTML Doc
		//Console::WriteLine(nodo_p->HasChildNodes);
		return nodo_p->SelectNodes(Xpath); //Select all nodes with direct image link
	};
	static Danbooru_Containers::ThreadGroup^ SetThreads(array<String^>^tags){
		Danbooru_Containers::ThreadGroup^ Threads=gcnew Danbooru_Containers::ThreadGroup;
		Threads->Threads = gcnew array<Thread^>(NUMBER_OF_THREADS);
		Threads->Parameters = gcnew array<Danbooru_Containers::ReadDanbooruParameters>(NUMBER_OF_THREADS);
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
	static array<Danbooru_Containers::PostData^>^ GetDownloadData(int page, array<String^>^tags) //Function to get Download Links from site with 'page' and 'tags' as source
	{
		String^ RawHtml = ReadDanbooru(page,tags);
		//Console::WriteLine(RawHtml);
		HtmlNodeCollection^ HtmlNodes = GetHtmlNodes(RawHtml,"//span[@class='thumb']/a");
#ifdef _DEBUG
		Console::WriteLine("Total nodes in GetDownloadLink are: "+HtmlNodes->Count);
#endif // DEBUGGING
		array<Danbooru_Containers::PostData^>^ LinkData = gcnew array<Danbooru_Containers::PostData^>(HtmlNodes->Count);
		int nodecount=0;
		for each (HtmlNode^ a_refs in HtmlNodes)
		{
			Danbooru_Containers::PostData^ a_r_data =GetPostData(SITE_DOMAIN+"/"+a_refs->GetAttributeValue("href",""));
			if (a_r_data!=nullptr){
				LinkData[nodecount]=a_r_data;
			}else
			{
				LinkData[nodecount]=nullptr;
			}
			//Console::WriteLine(Links[nodecount]);
			nodecount++;
		}
#ifdef _DEBUG
		Console::WriteLine("The total nodes processed are "+nodecount);
#endif // DEBUGGING
		return LinkData;
};
	static Danbooru_Containers::PostData^ GetPostData(String^ PostUrl) //Function to get Download Link from page url
	{
		PostUrl=Uri::UnescapeDataString(PostUrl);
		PostUrl=PostUrl->Replace("&amp;","&");
#ifdef _DEBUG
		System::Console::WriteLine("GetPostData function called with: "+PostUrl);
#endif
		Uri^ Link=gcnew Uri(PostUrl);
		String^ RawHtml = AditionalFunctions::GetRawHtml(Link);
		Danbooru_Containers::PostData^ PostData = gcnew Danbooru_Containers::PostData;
		//Console::WriteLine(RawHtml);
		HtmlNodeCollection^ HtmlNodes = GetHtmlNodes(RawHtml,"//div[@class='content']/div/img[@id='image']");
		//Console::WriteLine("Total nodes in GetPostDirectLink are: "+HtmlNodes->Count);
		if (HtmlNodes!=nullptr)
		{
			for each (HtmlNode^ img in HtmlNodes)
			{
				PostData->Link=img->GetAttributeValue("src","false");
				PostData->Tags=img->GetAttributeValue("alt","false");
#ifdef _DEBUG
				Console::WriteLine("Direct link of post "+PostUrl+" is "+PostData->Link);
				Console::WriteLine("Tags of post "+PostUrl+" is "+PostData->Tags);
#endif // DEBUGGING
			}
			PostData->ID=PostUrl->Substring((PostUrl->IndexOf("id=")+3));;
#ifdef _DEBUG
			Console::WriteLine("ID of post "+PostUrl+" is "+PostData->ID);
#endif // DEBUGGING
			return PostData;
		}
		return nullptr;
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
		//DanbooruDownloader::st_dm=gcnew String("Static dentro de class");
		CheckTags=DanbooruDownloader::ReadDanbooru(0,args)->Contains("class=\"preview\"/>");
#ifdef _DEBUG
		Console::WriteLine("Checking tags.... Returned "+CheckTags);
#endif // DEBUGGING
		if (CheckTags)
		{
			int TotalPages =DanbooruDownloader::GetPagesNumber(args);
#ifdef _DEBUG
			Console::WriteLine("Total number of pages is "+TotalPages);
#endif // DEBUGGING
			int ThreadGroups = TotalPages/NUMBER_OF_THREADS;
			int ThreadRemainder = TotalPages % NUMBER_OF_THREADS;
			int ActualPage = 0;
			int ActualThread = 0;
			Danbooru_Containers::ThreadGroup^ Threads=DanbooruDownloader::SetThreads(args);
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
	
}
