// GelbooruSitesLibrary.h
#define Moebooru
#define Moebooru_tag_evaluation ReadDanbooru(START_PAGE_INDEX)->Contains("<a class=\"directlink largeimg\"")||ReadDanbooru(START_PAGE_INDEX)->Contains("<a class=\"directlink smallimg\"")
#include "../DanbooruSitesLibrary/DanbooruSitesLibrary.h"