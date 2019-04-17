#include"model.h"
#include<string>
using namespace std;
ReportNode* ReportListInit(const string reportFilename);
bool saveReport(const Report* pReport, const string filename);
void  addNewReport(string strNo, int operate, float operateuse, float fBalance, ReportNode** ppReportNodeHead);
void find(ReportNode* pReportNodeHead);
void sum(ReportNode* pReportNodeHead);
void DeleteReport(ReportNode* pReportNodeHead);
Report* reportIsExist(string strNo, int &nReportIndex, ReportNode*  pReportNodeHead);