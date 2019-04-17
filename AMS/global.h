#ifndef GLOBAL_H_INCLUDED
#define GLOBAL_H_INCLUDED   
#include <string> 
using namespace std; 
const string CARDPATH = "card.dat";     
const string BILLINGPATH = "billing.dat";   
const string REPORTPATH = "report.dat";    
const int TIMELENGTH=20; 
const int ADD = 11;// -注册卡
const int LOGIN = 12;//- 上机
const int LOGOUT = 13;// -下机 
const int TOPUP = 14;// -充值 
const int RETURNFEE = 15;// -退费
const int CANCEL = 16;// -注销卡
const int SUCCESS =0;       
const int FINDCARD    = 1;       
const int NOFINDCARD  = 2;      
const int NOMATCH     = 3;       
const int ENOUGHMONEY = 4;      
const int NOFARE = 5;      
const int USING = 6;      
const int UNUSE       = 7;      
const int INVALID     = 8;       
const int NOSETTLEMENT = 0;     
const int YESSETTLEMENT = 1;   
const int UNIT = 15;           
const float CHARGE = 0.5f;        

#endif 
