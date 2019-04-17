#include"model.h"
#include<string>
using namespace std;
BillingNode* BillListInit(const string billingFilename);
bool updateCard(const Card* pCard, const string pPath, int nCardIndex);
bool saveBilling(const Billing* pBilling, const string pPath);
int logon(string strNo, string strPwd, LogonInfo* pInfo, CardNode* pCardNodeHead, BillingNode **ppBillingNodeHead, ReportNode** ppReportNodeHead);
int settle(string strNo, string strPwd, SettleInfo* pInfo, CardNode* const pCardNodeHead, BillingNode* const pBillingNodeHead, ReportNode** ppReportNodeHead);
int topup(string strNo, string strPwd, TopupInfo* pInfo,  CardNode* const pCardNodeHead, ReportNode** ppReportNodeHead);
int returnfee(string strNo, string strPwd, ReturnfeeInfo* pInfo, CardNode* const pCardNodeHead, ReportNode** ppReportNodeHead);