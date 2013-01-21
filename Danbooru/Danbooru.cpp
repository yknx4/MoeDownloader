// MoeDownloader.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
//#define DEBUGGING //Uncomment this line to enable special debugging procedures
using namespace System;
using namespace DanbooruSitesLibrary;

int _tmain(int argc, _TCHAR* argv[])
{
	/*Console::BackgroundColor=ConsoleColor::White;
	Console::ForegroundColor=ConsoleColor::Black;*/
	Console::Title="Batch image downloader";
	Danbooru_Containers::SiteData^ SiteData = gcnew Danbooru_Containers::SiteData;
	SiteData->SITE_DOMAIN = gcnew String("http://danbooru.donmai.us");
	SiteData->SITE_NAME = gcnew String("Danbooru");
	SiteData->START_PAGE_INDEX=1;
	SiteData->NUMBER_OF_THREADS=3;
	SiteData->SEGMENTDEPTH_FOR_ID=4;
	SiteData->USER_AGENT_STRING = gcnew String("Mozilla/5.0 (Windows NT 5.1) AppleWebKit/536.5 (KHTML, like Gecko) Chrome/19.0.1084.56 Safari/536.5");
	SiteData->CHECKTAGS_STRING = gcnew String("<img  class=\"preview    \"");
	SiteData->ACCESSPAGE_STRING = gcnew String("/post?page=");
	SiteData->POSTTAGS_STRING = gcnew String("&commit=Search&tags=");
	SiteData->PAGENUMBER_XPATH = gcnew String("//div[@class='pagination']/a");
	SiteData->POSTLINKS_XPATH = gcnew String("//span[@class='thumb blacklisted']/a");
	SiteData->IMAGECONTAINER_XPATH = gcnew String("//div[@class='content']/div/img[@id='image']");
	SiteData->FILEPATH_JOINER = gcnew String("");
	SiteData->DelayInConnections=0;
	array<String^>^args = Environment::GetCommandLineArgs();
	DanbooruDownloader::StartDownloader(args,SiteData);
#ifdef _DEBUG
	Console::ReadKey();
#endif // _DEBUG
}