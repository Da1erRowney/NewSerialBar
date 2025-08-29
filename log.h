#pragma  once

#ifdef _DEBUG
	#define WriteLog WriteLogA
#else 
#define WriteLog Noop
#endif

int WriteLogA(const char *f, ... );
int Noop(const char *f,...);


