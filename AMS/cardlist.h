#include"model.h"
#include<string>
using namespace std;
CardNode*CardListInit(const string filenname);
int addNewCard(string strNo, string strPwd, float fBalance, CardNode ** ppCardNodeHead, ReportNode** ppReportNodeHead);
Card* cardIsExist(string strNo, int &nCardIndex, CardNode* pCardNodeHead);
void displayCard(CardNode* pCardNodeHead);
void invalid(CardNode* pCardNodeHead, ReportNode** ppReportNodeHead);
void displayCardone(CardNode* pCardNodeHead, string strNo);
bool saveCard(const Card* pCard, const string filename);
void DeleteCard(CardNode* pCardNodeHead);