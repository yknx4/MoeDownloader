// DanbooruSitesLibrary.h
//#define Gelbooru
#pragma once
#include "Threading.h"

using namespace System;
using namespace System::Net;
using namespace System::Threading;
using namespace HtmlAgilityPack;

namespace DanbooruSitesLibrary {
	public ref class Danbooru_Containers
{
public:
	/*value struct ReadDanbooruParameters //Defines a Struc with YandereDownloader::ReadYande_re parameters needance
	{
		int page; 
		array<String^>^tags;
	};*/
	value struct ThreadGroup {
		array<Thread^>^Threads;
		//array<YandereDownloader^>^DownloaderReferences;
		array<int>^Pages;
	};
	value struct PostData {
		String^ Link;
		int ID;
		String^ Tags;
	};
	value struct SiteData {
		static int START_PAGE_INDEX;
		static int NUMBER_OF_THREADS;
		static int SEGMENTDEPTH_FOR_ID;
		static String^ SITE_DOMAIN;
		static String^ SITE_NAME;
		static String^ USER_AGENT_STRING;
		static String^ CHECKTAGS_STRING;
		static String^ ACCESSPAGE_STRING;
		static String^ POSTTAGS_STRING;
		static String^ PAGENUMBER_XPATH;
		static String^ POSTLINKS_XPATH;
		static String^ IMAGECONTAINER_XPATH;
		static String^ FILEPATH_JOINER;
		static bool DelayInConnections;
		static int DelayTime;
	};
};
	public ref class DanbooruDownloader //MainClass yandere downloader
{
private:
	static array<String^>^ args;
	static array<String^>^ tags;
	static int START_PAGE_INDEX;
	static int NUMBER_OF_THREADS;
	static int DelayTime;
	static int SEGMENTDEPTH_FOR_ID;
	static String^ SITE_DOMAIN;
	static String^ SITE_NAME;
	static String^ USER_AGENT_STRING;
	static String^ CHECKTAGS_STRING;
	static String^ ACCESSPAGE_STRING;
	static String^ POSTTAGS_STRING;
	static String^ PAGENUMBER_XPATH;
	static String^ POSTLINKS_XPATH;
	static String^ IMAGECONTAINER_XPATH;
	static String^ FILEPATH_JOINER;
	static bool DelayInConnections;
	static Text::RegularExpressions::Regex^ digitsOnly = gcnew Text::RegularExpressions::Regex("[^0-9]");
	static void DefineSite(Danbooru_Containers::SiteData^ SiteData){
		SITE_DOMAIN = SiteData->SITE_DOMAIN;
		SITE_NAME = SiteData->SITE_NAME;
		START_PAGE_INDEX=SiteData->START_PAGE_INDEX;
		NUMBER_OF_THREADS=SiteData->NUMBER_OF_THREADS;
		SEGMENTDEPTH_FOR_ID=SiteData->SEGMENTDEPTH_FOR_ID;
		USER_AGENT_STRING = SiteData->USER_AGENT_STRING;
		CHECKTAGS_STRING = SiteData->CHECKTAGS_STRING;
		ACCESSPAGE_STRING = SiteData->ACCESSPAGE_STRING;
		POSTTAGS_STRING = SiteData->POSTTAGS_STRING;
		PAGENUMBER_XPATH = SiteData->PAGENUMBER_XPATH;
		POSTLINKS_XPATH = SiteData->POSTLINKS_XPATH;
		IMAGECONTAINER_XPATH = SiteData->IMAGECONTAINER_XPATH;
		FILEPATH_JOINER = SiteData->FILEPATH_JOINER;
		DelayInConnections=SiteData->DelayInConnections;
		DelayTime=SiteData->DelayTime;
	};
	static String^ GetRawHtml(String^ Url){
		//Console::WriteLine("GetRawHtml Called with: "+Url);
		String^ OutputHtml;
		try
		{
#ifdef _DEBUG
		//	Console::WriteLine("GetRawHtml Called with: "+Url);
#endif // _DEBUG
			WebClient^ Host_Reader = gcnew WebClient; //Initialize the Webclient needed to parse HTML
			Host_Reader->Headers->Add("user-agent", USER_AGENT_STRING); //Add user agent header
			OutputHtml=Host_Reader->DownloadString(Url);	//Download Source Code and put it in PageData String
			//Console::WriteLine(OutputHtml);
		}
		catch (WebException^ e)
		{
			Console::ForegroundColor=ConsoleColor::Red;
			Console::WriteLine("GetRawHtml failed because {0}",e);
			Console::WriteLine("at: "+Url);
			Console::ResetColor();
			return nullptr;
		}
		catch (ArgumentNullException^ e){
			Console::ForegroundColor=ConsoleColor::Red;
			Console::WriteLine("You have to have an input {0}",e);
			Console::ResetColor();
			throw e;
		}
		return OutputHtml;
	};
	static String^ GetRawHtml(Uri^ Url){
		//Console::WriteLine("GetRawHtml Called with: "+Url);
		String^ OutputHtml;
		try
		{
#ifdef _DEBUG
			//Console::WriteLine("GetRawHtml Called with: "+Url);
#endif // _DEBUG
			WebClient^ Host_Reader = gcnew WebClient; //Initialize the Webclient needed to parse HTML
			Host_Reader->Headers->Add("user-agent", USER_AGENT_STRING); //Add user agent header
			OutputHtml=Host_Reader->DownloadString(Url);	//Download Source Code and put it in PageData String
			//Console::WriteLine(OutputHtml);
		}
		catch (WebException^ e)
		{
			Console::WriteLine("GetRawHtml failed because {0}",e);
			return nullptr;
		}
		catch (ArgumentNullException^ e){
			Console::WriteLine("You have to have an input {0}",e);
			throw e;
		}
		return OutputHtml;
	};
	static String^ ReadDanbooru(int page) //Function to get RAW html source code from site with 'page' and 'tags' as source
	{
		if (page<0)
		{
			throw gcnew System::ArgumentOutOfRangeException;
		}
		String^ Page_Data;//RAW Html string initialization
		String^ FinalUrl= gcnew String(SITE_DOMAIN+ACCESSPAGE_STRING);//URL String initialization
#ifndef Gelbooru
		FinalUrl += page.ToString() + POSTTAGS_STRING;//Add page and "&tags=" to URL
#endif
#ifdef Gelbooru
		FinalUrl += (page*PID_MULT).ToString() + POSTTAGS_STRING;//Add page and "&tags=" to URL
#endif
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
			Page_Data=GetRawHtml(FinalUrl);
		}
		catch (System::ArgumentNullException^ e)
		{
			Console::ForegroundColor=ConsoleColor::Red;
			Console::WriteLine("URL Must have a value");
			Console::WriteLine("Error: {0}",e);
			Console::ResetColor();
		}
		if (Page_Data == nullptr) //In case of Webexception the following code is run
		{
			Console::ForegroundColor=ConsoleColor::Red;
			Console::WriteLine("Cannot connect to Host");
			Console::WriteLine("Is Internet connection alive?");
			Console::ResetColor();
		}	
		
		return Page_Data; //Returns RAW HTLM string
	};
	static HtmlNodeCollection^ GetHtmlNodes(String^ RawHtml,String^ Xpath){
		if (RawHtml == nullptr || Xpath == nullptr)
		{
			throw gcnew ArgumentNullException;
		}
		try
		{
			HtmlDocument doc; //initialize HTML Documents
			doc.LoadHtml(RawHtml);//load Html from RAW Html string
			HtmlNode^ nodo_p = doc.DocumentNode; //Get Nodes from HTML Doc
			//Console::WriteLine(nodo_p->HasChildNodes);
			return nodo_p->SelectNodes(Xpath); //Select all nodes with direct image link
		}
		catch (System::ArgumentNullException^ e)
		{
			Console::ForegroundColor=ConsoleColor::Red;
			Console::WriteLine("GetHtmlNodes failed because {0]",e);
			Console::ResetColor();
			return nullptr;
		}
		
	};
	static Danbooru_Containers::ThreadGroup^ SetThreads(){
		Danbooru_Containers::ThreadGroup^ Threads=gcnew Danbooru_Containers::ThreadGroup;
		Threads->Threads = gcnew array<Thread^>(NUMBER_OF_THREADS);
		Threads->Pages = gcnew array<int>(NUMBER_OF_THREADS);
		return Threads; //Select all nodes with direct image link
	};
#ifndef Moebooru
	static array<Danbooru_Containers::PostData^>^ GetDownloadData(int page) //Function to get Download Links from site with 'page' and 'tags' as source
	{
		String^ RawHtml;
		try
		{
			RawHtml = ReadDanbooru(page);	
		}
		catch (ArgumentOutOfRangeException^ e)
		{
			Console::ForegroundColor=ConsoleColor::Red;
			Console::WriteLine("Page must be positive");
			Console::WriteLine("Error: {0}",e);
			Console::ResetColor();
		}
		if (RawHtml==nullptr)
		{
			throw gcnew ArgumentNullException;
		}
		//Console::WriteLine(RawHtml);
		HtmlNodeCollection^ HtmlNodes;
		HtmlNodes = GetHtmlNodes(RawHtml,POSTLINKS_XPATH);
		if (HtmlNodes==nullptr || HtmlNodes->Count==0)
		{
			Console::ForegroundColor=ConsoleColor::Red;
			Console::WriteLine("No nodes were found with the specified XPath");
			Console::ResetColor();
			return nullptr;
		}
#ifdef _DEBUG
		Console::WriteLine("Total nodes in GetDownloadLink are: "+HtmlNodes->Count);
#endif // DEBUGGING
		array<Danbooru_Containers::PostData^>^ LinkData = gcnew array<Danbooru_Containers::PostData^>(HtmlNodes->Count);
		int nodecount=0;
		for each (HtmlNode^ a_refs in HtmlNodes)
		{
			if (DelayInConnections)Thread::Sleep(DelayTime);
			Danbooru_Containers::PostData^ a_r_data;
			try
			{
				a_r_data=GetPostData(SITE_DOMAIN+FILEPATH_JOINER+a_refs->GetAttributeValue("href",""));
			}
			catch (ArgumentNullException^ e)
			{
				Console::ForegroundColor=ConsoleColor::Red;
				Console::WriteLine("Gotta give a valid url");
				Console::WriteLine("Error: {0}",e);
				Console::ResetColor();
			}
				LinkData[nodecount]=a_r_data;
			//Console::WriteLine(Links[nodecount]);
			nodecount++;
		}
#ifdef _DEBUG
		Console::WriteLine("The total nodes processed are "+nodecount);
#endif // DEBUGGING
		return LinkData;
	};
	static void DownloadFiles(Object^ data){//function to Parse Html Tags and call download
		if (data!=nullptr)
		{
			int Page =(int)data; //Convert input object point to a ReadYandereParameters Struct pointer and direct it to object
			array<Danbooru_Containers::PostData^>^ Links=GetDownloadData(Page);
			if (Links!=nullptr){
				int LinksCount = Links->Length;
				Console::WriteLine("Downloading "+LinksCount+" files."); //Write the number of downloadable files
				int TryCount=0;
				for (int i=0;i<LinksCount;i++)
				{
					if (Links[i]==nullptr)
					{
						Console::ForegroundColor=ConsoleColor::Red;
						Console::WriteLine("No data in position "+i);
						Console::ResetColor();
						continue;
						
					}
					//Console::WriteLine(Links[i]);
					Uri^ Link=gcnew Uri(Links[i]->Link);
				
						String^ FilePath = ParseFilePath(Links[i]->ID+" "+Links[i]->Tags);
#ifndef Gelbooru
						String^ FileExtension = ParseFileExtension(Links[i]->Link);
#endif
#ifdef Gelbooru
						String^ FileExtension = ParseFileExtension(Links[i]->Link,Convert::ToInt32(Links[i]->ID));
#endif
						FilePath=SITE_NAME+FILEPATH_JOINER+FilePath+FileExtension;
						if (!(IO::File::Exists(FilePath)))
						{
							try
							{
								WebClient^ Host_Reader = gcnew WebClient;
								Host_Reader->Headers->Add("user-agent", USER_AGENT_STRING);
								Console::WriteLine("Downloading "+FilePath);
#ifndef _DEBUG
								Host_Reader->DownloadFile(Link,FilePath);  
#endif // !DEBUGGING
							}
						catch (WebException^ e)
						{
							Console::ForegroundColor=ConsoleColor::Red;
							if (TryCount<3)
							{
								Console::WriteLine("Failed to fetch position "+i+"... Attempt "+(TryCount+1));
								Console::WriteLine("Because: {0}",e);
								Console::ResetColor();
								i-=1;
								continue;
							}
#ifdef _DEBUG
							Console::WriteLine("Cannot connect to Host {0}", e);
#endif
#ifndef _DEBUG
							Console::WriteLine("Cannot Download {0}", Links[i]->Link);
							Console::WriteLine("Because {0}", e);
#endif
							Console::ResetColor();
						}
						catch (ArgumentNullException^ e){
							Console::ForegroundColor=ConsoleColor::Red;
							Console::WriteLine("You have to have an input {0}",e);
							Console::ResetColor();
							throw e;
						}
							
						} 
						else
						{
							Console::WriteLine(Links[i]->ID+" Already Exists");
						}

					
				}
				Thread::Yield();
			}
				
		}
		
	};
	static String^ ParseFilePath(String^ FilePath){
		FilePath=FilePath->Join("",FilePath->Split(IO::Path::GetInvalidFileNameChars()));
		FilePath=FilePath->Replace("show ","");
		FilePath=Uri::UnescapeDataString(FilePath);
		if (FilePath->Length > 140)
		{
			FilePath=FilePath->Substring(0,140);
		}
		return FilePath;
	};
#endif
#ifndef Gelbooru
# ifndef Moebooru
	static Danbooru_Containers::PostData^ GetPostData(String^ PostUrl) //Function to get Download Link from page url
	{
		if (PostUrl == nullptr)
		{
			Console::ForegroundColor=ConsoleColor::Red;
			Console::WriteLine("URL Cannot be Null");
			Console::ResetColor();
			throw gcnew ArgumentNullException;
		}
		Uri^ Link=gcnew Uri(PostUrl);
		String^ RawHtml;
		try
		{
			RawHtml = GetRawHtml(PostUrl);
		}
		catch (ArgumentNullException^ e)
		{
			Console::ForegroundColor=ConsoleColor::Red;
			Console::WriteLine("Gotta specify an URL");
			Console::WriteLine("Error {0}",e);
			Console::ResetColor();
		}
		
		if (RawHtml==nullptr)
		{
			Console::ForegroundColor=ConsoleColor::Red;
			Console::WriteLine("No HTML Found, giving nullptr");
			Console::ResetColor();
			return nullptr;
		}
		Danbooru_Containers::PostData^ PostData = gcnew Danbooru_Containers::PostData;
		//Console::WriteLine(RawHtml);
		HtmlNodeCollection^ HtmlNodes = GetHtmlNodes(RawHtml,IMAGECONTAINER_XPATH);
		//Console::WriteLine("Total nodes in GetPostDirectLink are: "+HtmlNodes->Count);
		if (HtmlNodes!=nullptr)
		{
			for each (HtmlNode^ img in HtmlNodes)
			{
				PostData->Link=img->GetAttributeValue("src","false");
				PostData->Tags=img->GetAttributeValue("alt","false");
#ifdef _DEBUG
				Console::WriteLine("Direct link of post "+PostUrl+" is "+PostData->Link);
				Console::WriteLine("Tags of post "+PostUrl+" are "+PostData->Tags);
#endif // DEBUGGING
			}
			String^ IDasString =Link->Segments[SEGMENTDEPTH_FOR_ID];
			PostData->ID=Convert::ToInt32(digitsOnly->Replace(IDasString,""));
#ifdef _DEBUG
			Console::WriteLine("ID of post "+PostUrl+" is "+PostData->ID);
#endif // DEBUGGING
			return PostData;
		}
		return nullptr;
	};
# endif
	static String^ ParseFileExtension(String^ FileExtension){
		FileExtension=FileExtension->Replace("jpeg","jpg");
		FileExtension=FileExtension->Substring(FileExtension->Length-4,4);
		return FileExtension;
	};
	static int GetPagesNumber() //Function to get the number of pages the tags have
	{
		String^ texto = DanbooruDownloader::ReadDanbooru(START_PAGE_INDEX);//Put RAW HTML in the String Texto
		if (!(texto==nullptr)) //If the RAW HTML exist, do following code
		{
			String^ PageTemp; // Create string pointer to get inner text from Html Node
			HtmlNodeCollection^ nodos_a = DanbooruDownloader::GetHtmlNodes(texto,PAGENUMBER_XPATH); //Select 'a' nodes from 'div' with class='pagination'
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
				Console::ForegroundColor=ConsoleColor::DarkBlue;
				Console::WriteLine("Return 1 page because of {0}",e);
				Console::ResetColor();
				return 1;	//return 1 page
			}
		}
		return 1;//in any case return 1 page, to avoid program crash, further errors are handled after
	};
#endif // !Gelbooru
#ifdef Moebooru
	static void DownloadFiles(Object^ data);
	static String^ ParseFilePath(String^ FilePath);
#endif // Moebooru
#ifdef Gelbooru
	static Danbooru_Containers::PostData^ GetPostData(String^ PostUrl);
	static String^ ParseFileExtension(String^ FileExtension,int ID);
	static int GetPagesNumber();
#endif // Gelbooru

public:
	static void StartDownloader(array<String^>^ args,Danbooru_Containers::SiteData^ SiteData){
		Console::Title=SiteData->SITE_NAME+" Batch image downloader";
		bool CheckTags=0;
		if ( args == nullptr || args->Length == 1 )
		{
			Console::ForegroundColor=ConsoleColor::Red;
			Console::WriteLine("Specify the tags to crawl");
			Console::WriteLine("Usage: "+SiteData->SITE_NAME+" moe suzumiya_haruhi");
			Console::WriteLine("To download pictures with tags moe and suzumiya_haruhi");
			Console::WriteLine("Remember Danbooru-like sites threats spaces as underline score");
		}else
		{
			DefineSite(SiteData);
			Array::Reverse(args);
			Array::Resize(args,args->Length -1);
			tags=args;
#ifndef Moebooru
			CheckTags=ReadDanbooru(START_PAGE_INDEX)->Contains(CHECKTAGS_STRING);
#endif
#ifdef Moebooru
			CheckTags=Moebooru_tag_evaluation;
#endif
#ifdef _DEBUG
			Console::WriteLine("Checking tags.... Returned "+CheckTags);
#endif // DEBUGGING
			if (CheckTags)
			{
				int TotalPages =GetPagesNumber();
				Console::WriteLine("Will download "+TotalPages+" pages.");
				int ThreadGroups = TotalPages/NUMBER_OF_THREADS;
				int ThreadRemainder = TotalPages % NUMBER_OF_THREADS;
				int ActualPage = 0;
				int ActualThread = 0;
				Danbooru_Containers::ThreadGroup^ Threads=SetThreads();
				for (int tg=0;tg<ThreadGroups;tg++){
					for (int th = 0; th < NUMBER_OF_THREADS; th++)
					{
						ActualThread=th;
#ifndef Gelbooru
						ActualPage=tg*NUMBER_OF_THREADS+th+1;
#endif
#ifdef Gelbooru
						ActualPage=tg*NUMBER_OF_THREADS+th;
#endif
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
#ifndef Gelbooru
						ActualPage =ThreadGroups*NUMBER_OF_THREADS+tr+1;
#endif
#ifdef Gelbooru
						ActualPage =ThreadGroups*NUMBER_OF_THREADS+tr;
#endif
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
				Console::WriteLine("All Downloads Finished");
			} 
			else
			{
				Console::WriteLine("Nothing here but us Chickens!");
			}

			
		}
	};
};
#ifdef Moebooru
String^ DanbooruDownloader::ParseFilePath(String^ FilePath){
	FilePath=FilePath->Replace(SITE_NAME+"%20","");
	FilePath=FilePath->Replace("Konachan.com%20-%20","");
	FilePath=Uri::UnescapeDataString(FilePath);
	FilePath=FilePath->Join("",FilePath->Split(IO::Path::GetInvalidFileNameChars()));
	if (FilePath->Length>140){ 
		FilePath=FilePath->Substring(0,140);
	}
	return FilePath;
};
void DanbooruDownloader::DownloadFiles(Object^ data){//function to Parse Html Tags and call download
	if (data!=nullptr)
	{
	int Page =(int)data; //Convert input object point to a ReadYandereParameters Struct pointer and direct it to object
	String^ HtmlContent=ReadDanbooru(Page); //Get Raw HTML with needed parameters
	if (HtmlContent!=nullptr)
	{
		String^ TempUrl; //Initialize string to store URL
		HtmlNodeCollection^ nodos_a = GetHtmlNodes(HtmlContent,"//a[@class='directlink largeimg'] | //a[@class='directlink smallimg']");; //Select all nodes with direct image link
		Console::WriteLine("Downloading "+nodos_a->Count+" files."); //Write the number of downloadable files
		for each (HtmlNode^ var in nodos_a)
		{
			if (var==nullptr)
			{
				continue;
			}
			//Console::WriteLine(Links[i]);
			TempUrl=var->GetAttributeValue("href","");
			Uri^ Link=gcnew Uri(TempUrl);
			String^ FilePath = ParseFilePath(Link->Segments[SEGMENTDEPTH_FOR_ID]);
			String^ FileExtension = ParseFileExtension(Link->Segments[SEGMENTDEPTH_FOR_ID]);
			FilePath=SITE_NAME+FILEPATH_JOINER+FilePath+FileExtension;
			if (!(IO::File::Exists(FilePath)))
			{
				try
				{
					WebClient^ Host_Reader = gcnew WebClient;
					Host_Reader->Headers->Add("user-agent", USER_AGENT_STRING);
					Console::WriteLine("Downloading "+FilePath);
#ifndef _DEBUG
					Host_Reader->DownloadFile(Link,FilePath);  
#endif // !DEBUGGING
				}
				catch (WebException^ e)
				{
					Console::ForegroundColor=ConsoleColor::Red;
#ifdef _DEBUG
					Console::WriteLine("Cannot connect to Host {0}", e);
#endif
#ifndef _DEBUG
					Console::WriteLine("Cannot Download {0}", Link);
					Console::WriteLine("Because {0}", e);
#endif
					Console::ResetColor();
				}
				catch (ArgumentNullException^ e){
					Console::ForegroundColor=ConsoleColor::Red;
					Console::WriteLine("You have to have an input {0}",e);
					Console::ResetColor();
					throw e;
				}
								} 
			else
			{
				Console::WriteLine(FilePath+" Already Exists");
			}

			
		}
	}
	
	Thread::Yield();
	}
	
};
#endif
#ifdef Gelbooru
Danbooru_Containers::PostData^ DanbooruDownloader::GetPostData(String^ PostUrl) //Function to get Download Link from page url
{
	if (PostUrl == nullptr)
	{
		Console::ForegroundColor=ConsoleColor::Red;
		Console::WriteLine("URL Cannot be Null");
		Console::ResetColor();
		throw gcnew ArgumentNullException;
	}
	PostUrl=Uri::UnescapeDataString(PostUrl);
	PostUrl=PostUrl->Replace("&amp;","&");
#ifdef _DEBUG
	System::Console::WriteLine("GetPostData function called with: "+PostUrl);
#endif
	Uri^ Link=gcnew Uri(PostUrl);
	String^ RawHtml;
	try
	{
		RawHtml = GetRawHtml(PostUrl);
	}
	catch (ArgumentNullException^ e)
	{
		Console::ForegroundColor=ConsoleColor::Red;
		Console::WriteLine("Gotta specify an URL");
		Console::WriteLine("Error {0}",e);
		Console::ResetColor();
	}
	if (RawHtml==nullptr)
	{
		Console::ForegroundColor=ConsoleColor::Red;
		Console::WriteLine("No HTML Found, giving nullptr");
		Console::ResetColor();
		return nullptr;
	}
	Danbooru_Containers::PostData^ PostData = gcnew Danbooru_Containers::PostData;
	//Console::WriteLine(RawHtml);
	HtmlNodeCollection^ HtmlNodes = GetHtmlNodes(RawHtml,IMAGECONTAINER_XPATH);
	//Console::WriteLine("Total nodes in GetPostDirectLink are: "+HtmlNodes->Count);
	if (HtmlNodes!=nullptr)
	{
		for each (HtmlNode^ img in HtmlNodes)
		{
			PostData->Link=img->GetAttributeValue("src","false");
			PostData->Tags=img->GetAttributeValue("alt","false");
#ifdef _DEBUG
			Console::WriteLine("Direct link of post "+PostUrl+" is "+PostData->Link);
			Console::WriteLine("Tags of post "+PostUrl+" are "+PostData->Tags);
#endif // DEBUGGING
		}
		PostData->ID=Convert::ToInt32(PostUrl->Substring((PostUrl->IndexOf("id=")+3)));
#ifdef _DEBUG
		Console::WriteLine("ID of post "+PostUrl+" is "+PostData->ID);
#endif // DEBUGGING
		return PostData;
	}
	return nullptr;
};
String^ DanbooruDownloader::ParseFileExtension(String^ FileExtension,int ID){
	FileExtension=FileExtension->Replace("jpeg","jpg");
	FileExtension=FileExtension->Replace("?"+ID,"");
	FileExtension=FileExtension->Substring(FileExtension->Length-4,4);
	return FileExtension;
};
int DanbooruDownloader::GetPagesNumber() //Function to get the number of pages the tags have
{
	String^ texto = DanbooruDownloader::ReadDanbooru(START_PAGE_INDEX);//Put RAW HTML in the String Texto
	if (!(texto==nullptr)) //If the RAW HTML exist, do following code
	{
		String^ PageTemp; // Create string pointer to get inner text from Html Node
		HtmlNodeCollection^ nodos_a = DanbooruDownloader::GetHtmlNodes(texto,PAGENUMBER_XPATH); //Select 'a' nodes from 'div' with class='pagination'
		try //Try to read nodes
		{
			for each (HtmlNode^ var in nodos_a) // Cycle for each node
				{
					PageTemp=var->GetAttributeValue("href","false"); //Put the data in PageTemp variable
					//Console::WriteLine(PageTemp);
					PageTemp=PageTemp->Substring((PageTemp->IndexOf("pid=")+4));
					//Console::WriteLine(PageTemp);
				}
				return ((Convert::ToInt32(PageTemp,10)/PID_MULT)+1);
		}
		catch (NullReferenceException^ e) //If no div availavable, means it just have 1 page
		{
			Console::ForegroundColor=ConsoleColor::DarkBlue;
			Console::WriteLine("Return 1 page because of {0}",e);
			Console::ResetColor();
			return 1;	//return 1 page
		}
	}
	return 1;//in any case return 1 page, to avoid program crash, further errors are handled after
};
#endif // Gelbooru





}
