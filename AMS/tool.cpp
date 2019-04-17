//Ê±¼ä
#include"stdafx.h"
#include <time.h> 
#include <stdio.h>  
using namespace std;
void timeToString(time_t t, char* pBuf) {
	struct tm * timeinfo;
	timeinfo =  localtime(&t);
	strftime(pBuf, 20, "%Y-%m-%d %H:%M", timeinfo);
}
time_t stringToTime(int year,int month,int day,int hour,int min) {
	struct tm tm1; 
	time_t time1;
	tm1.tm_year = year-1900;
	tm1.tm_mon = month-1;
	tm1.tm_mday = day;
	tm1.tm_hour = hour;
	tm1.tm_min = min;
	tm1.tm_sec = 0;
	tm1.tm_isdst = -1;
	time1 = mktime(&tm1);
	return time1;
}
