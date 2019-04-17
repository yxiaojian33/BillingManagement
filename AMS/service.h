#include"model.h"
void addCard(CardNode** ppCardNodeHead, ReportNode** ppReportNodeHead);
void queryCard(CardNode* const pCardNodeHead);
void loginCard(CardNode* pCardNodeHead, BillingNode **ppBillingNodeHead, ReportNode** ppReportNodeHead);
void clearData(CardNode* pCardNodeHead);
void logoutCard(CardNode* pCardNodeHead, BillingNode* pBillingNodeHead, ReportNode** ppReportNodeHead);
void top_up(CardNode* pCardNodeHead, ReportNode** ppReportNodeHead);
void return_fee(CardNode* pCardNodeHead, ReportNode** ppReportNodeHead);
void statis( ReportNode** ppReportNodeHead);
void DeleteOff(CardNode* pCardNodeHead, ReportNode** ppReportNodeHead);