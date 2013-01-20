#ifdef DANBOORUSITES_EXPORTS
#define DANBOORUSITES_API __declspec(dllexport) 
#else
#define DANBOORUSITES_API __declspec(dllimport) 
#endif