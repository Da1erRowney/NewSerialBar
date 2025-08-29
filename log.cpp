#include "StdAfx.h"
#include ".\log.h"
#include <stdio.h>


int WriteLogA(const char *f, ... )
{
	FILE *file;
	va_list args;
	va_start(args,f);
	SYSTEMTIME t;

	
	if ( !(file = fopen("C:\\SERIAL.LOG","a")) )
			return 0;
		GetSystemTime(&t);

		fprintf(file,"[%02d.%02d.%04d %02d:%02d:%02d.%03d]  ", t.wDay, t.wMonth, t.wYear, t.wHour, t.wMinute, t.wSecond, t.wMilliseconds);
		vfprintf(file,f,args);
		fprintf(file,"\n");
		fclose(file);
	
	
	return 0;
}

int Noop(const char*f,...)
{
	return 0;
}
