#include"stdafx.h"
#include<string>
#include<iostream>
#include"model.h"
#include"global.h"
#include"menu.h"
#include <fstream>
#include<iomanip> 
#include"tool.h"
#include"billList.h"
#include"reportlist.h"
using namespace std;
//遍历返回地址
Card* cardIsExist(string strNo, int &nCardIndex, CardNode*  pCardNodeHead)
{
	CardNode *pCardNode = pCardNodeHead;
	nCardIndex = 0;
	while (pCardNode != NULL)
	{
		if (strcmp(pCardNode->data.aName, strNo.c_str()) == 0)
		{
			return &(pCardNode->data);
		}
		pCardNode = pCardNode->next;
		nCardIndex++;
	}
	return NULL;
}
//从文件导入
CardNode* CardListInit(const string cardFilename)
{
	ifstream cardfile(cardFilename);
	CardNode *pCardNode, *pCardNodeHead, *pCardNodeTail;
	Card card;
	if (!cardfile.is_open()) { return NULL; }
	pCardNodeHead = NULL;
	pCardNodeTail = NULL;
	while (1) {
		cardfile.read((char*)&card, sizeof(Card));
		if (cardfile.eof()) { break; }
		pCardNode = new CardNode;
		pCardNode->data = card;
		if (pCardNodeHead == NULL)
		{
			pCardNodeHead = pCardNode;
			pCardNodeTail = pCardNode;
		}
		else
		{
			pCardNodeTail->next = pCardNode;
			pCardNodeTail = pCardNode;
		}
	}

	pCardNodeTail->next = NULL;
	cardfile.close();
	return pCardNodeHead;
}
//保存卡
bool saveCard(const Card* pCard, const string filename)
{
	ofstream ofile(filename, ios::binary | ios::app);
	ofile.write((char*)pCard, sizeof(Card));
	ofile.close();
	return true;
}
bool updateDeleteCard(const Card* pCard, const string pPath, string strNo)
{
	fstream ofile;
	ofile.open(pPath, ios_base::in | ios_base::binary);
	fstream temp; 
	temp.open("temp.dat", ios_base::out | ios_base::binary);
    ofile.read((char*)pCard, sizeof(Card)); 
 while (!ofile.eof()) {
	 if (strcmp(pCard->aName, strNo.c_str())!=0)
     temp.write((char*)pCard, sizeof(Card)) ;
	 ofile.read((char*)pCard, sizeof(Card));
 } 
 ofile.close();
 temp.close(); //修改完成，将原文件清空，并将临时文件内容复制回原文件 
 temp.open("temp.dat",ios_base::in|ios_base::binary);
  ofstream out; 
  out.open(pPath,ios_base::out|ios_base::binary);
  temp.read((char*)pCard,sizeof(Card));
  while(!temp.eof()) { 
	  out.write((char*)pCard,sizeof(Card));
	  temp.read((char*)pCard,sizeof(Card));
  }//完成复制
  out.close();
  temp.close(); 
return true;
}
bool updateDeleteReport(const Report* pReport, const string pPath, string strNo)
{
	fstream ofile;
	ofile.open(pPath, ios_base::in | ios_base::binary);
	fstream temp; temp.open("temp.dat", ios_base::out | ios_base::binary);
	ofile.read((char*)pReport, sizeof(Report));
	while (!ofile.eof()) {
		if (strcmp(pReport->aName, strNo.c_str()) != 0)
			temp.write((char*)pReport, sizeof(Report));
		ofile.read((char*)pReport, sizeof(Report));
	}
	ofile.close();
	temp.close(); //修改完成，将原文件清空，并将临时文件内容复制回原文件 
	temp.open("temp.dat", ios_base::in | ios_base::binary);
	ofstream out;
	out.open(pPath, ios_base::out | ios_base::binary);
	temp.read((char*)pReport, sizeof(Report));
	while (!temp.eof()) {
		out.write((char*)pReport, sizeof(Report));
		temp.read((char*)pReport, sizeof(Report));
	}//完成复制
	out.close();
	temp.close();
	return true;
}
//添加新卡
int addNewCard(string strNo, string strPwd, float fBalance, CardNode** ppCardNodeHead, ReportNode** ppReportNodeHead)
{
	int nCardIndex = 0;
	if (cardIsExist(strNo, nCardIndex, *ppCardNodeHead) != NULL)
	{ 
		return FINDCARD;
	}
	Card card;
	strcpy(card.aName, strNo.c_str());   
	strcpy(card.aPwd, strPwd.c_str());
	card.fBalance = fBalance;
	card.fTotalUse = 0;
	card.nStatus = UNUSE;           
	card.nUseCount = 0;               
	card.tStart = card.tLast = time(NULL);   
	CardNode* pCardNode = new CardNode; 
	pCardNode->data = card;
	CardNode *pCardNodeHead = *ppCardNodeHead;  CardNode *pCardNodeTail = *ppCardNodeHead;
	if (pCardNodeHead == NULL) 
	{        
		pCardNodeHead = pCardNode;
		pCardNodeTail = pCardNode;   
		*ppCardNodeHead = pCardNode;  
	}  else    
	{        
		pCardNodeTail = pCardNodeHead;    
		while(pCardNodeTail->next != NULL)    
		pCardNodeTail = pCardNodeTail->next; 
		pCardNodeTail->next = pCardNode; 
		pCardNodeTail = pCardNode; 
	} 
		pCardNodeTail->next = NULL;
		saveCard(&card, CARDPATH);
		addNewReport(strNo, ADD, fBalance, fBalance, ppReportNodeHead);
		return SUCCESS;
	}
//展示卡
void displayCard(CardNode* pCardNodeHead)
{
	CardNode *pCardNode = pCardNodeHead;
	cout << "卡号\t\t状态\t余额\t\t累计使用\t\t使用次数\t\t上次使用时间\n";
	while (pCardNode != NULL)
	{
		Card *it= &pCardNode->data;
		char aLastTime[TIMELENGTH] = { 0 };
		timeToString(it->tLast, aLastTime);
		cout << setiosflags(ios::left) << setw(8) << it->aName << "\t";
		if (it->nStatus == USING) cout << setiosflags(ios::left) << setw(8) << "上机\t";
		else if (it->nStatus == UNUSE) cout << setiosflags(ios::left) << setw(8) << "未上机\t";
		else if (it->nStatus == INVALID) cout << setiosflags(ios::left) << setw(8) << "注销\t";
		else cout << setw(8) << "错误\t";
		cout << setiosflags(ios::left) << setw(8) << setiosflags(ios::fixed) << setprecision(2) << it->fBalance << "\t";
		cout << setiosflags(ios::left) << setw(8) << it->fTotalUse << "\t\t";
		cout << setiosflags(ios::left) << setw(8) << it->nUseCount << "\t\t";
		cout << setiosflags(ios::left) << setw(8) << aLastTime << endl;
		pCardNode = pCardNode->next;
	}
}
void displayCardone(CardNode* pCardNodeHead, string strNo)
{
	Card*it;
	int nCardIndex = 0;
	if (cardIsExist(strNo, nCardIndex, pCardNodeHead) == NULL)  cout << "卡号不存在" << endl;
	else {
		it = cardIsExist(strNo, nCardIndex, pCardNodeHead);
		cout << "卡号\t\t状态\t余额\t\t累计使用\t\t使用次数\t\t上次使用时间\n";
		char aLastTime[TIMELENGTH] = { 0 };
		timeToString(it->tLast, aLastTime);
		cout << setiosflags(ios::left) << setw(8) << it->aName << "\t";
		if (it->nStatus == USING) cout << setiosflags(ios::left) << setw(8) << "上机\t";
		else if (it->nStatus == UNUSE) cout << setiosflags(ios::left) << setw(8) << "未上机\t";
		else if (it->nStatus == INVALID) cout << setiosflags(ios::left) << setw(8) << "注销\t";
		else cout << setw(8) << "错误\t";
		cout << setiosflags(ios::left) << setw(8) << setiosflags(ios::fixed) << setprecision(2) << it->fBalance << "\t";
		cout << setiosflags(ios::left) << setw(8) << it->fTotalUse << "\t\t";
		cout << setiosflags(ios::left) << setw(8) << it->nUseCount << "\t\t";
		cout << setiosflags(ios::left) << setw(8) << aLastTime << endl;
	}
}
int cancel(string strNo, string strPwd, CancelInfo* pInfo, CardNode* const pCardNodeHead, ReportNode** ppReportNodeHead) {
	int nCardIndex;
	Card* pCard = cardIsExist(strNo, nCardIndex, pCardNodeHead);
	if (pCard == NULL)
	{
		return NOFINDCARD;
	}
	if (pCard->nStatus == INVALID)
	{
		return INVALID;//卡已注销
	}
	if (pCard->nStatus == USING)
	{
		return USING;
	}
	if (strcmp(pCard->aPwd, strPwd.c_str()) != 0)
	{
		return NOMATCH;
	}
	pInfo->fBalance = pCard->fBalance;
	pCard->fBalance = 0;
	pCard->nStatus = INVALID;
	updateCard(pCard, CARDPATH, nCardIndex);
	strcpy(pInfo->aCardName, strNo.c_str());
	addNewReport(strNo, CANCEL, pInfo->fBalance, pCard->fBalance, ppReportNodeHead);
	return SUCCESS;
}
void invalid(CardNode* pCardNodeHead, ReportNode** ppReportNodeHead) {
	string strNo;
	string strPwd;
	CancelInfo* pInfo = new CancelInfo;
	if (inputNoPwd(strNo, strPwd)) {
		cout << "----------退费信息如下----------\n";
		int nResult =cancel(strNo, strPwd, pInfo, pCardNodeHead, ppReportNodeHead);
		switch (nResult)
		{
		case NOFINDCARD:
		case NOMATCH:
		{
			cout << endl << endl << "卡不存在，或卡信息不对，注销失败！" << endl << endl;
			break;
		}
		case INVALID:
		{
			cout << endl << "重复注销！！！！" << endl;
			break;
		}
		case USING:
		{
			cout << endl << "该卡正在使用，请下机后注销！" << endl;
			break;
		}
		case SUCCESS:
		{
			cout << endl;
			cout << "注销成功" << endl;
			cout << "卡号: \t" << strNo << endl;
			cout << "退费金额: \t" << setiosflags(ios::fixed) << setprecision(2) << pInfo->fBalance << endl;
			break;
		}
		default:break;
		}
	}
	else
	{
		cout << endl << endl << "卡号或者密码格式不正确，退费失败!" << endl << endl;
	}
}
CardNode* deleteList(CardNode* pCardNodeHead, string strNo) {
	CardNode* q, *p;
	if (pCardNodeHead == NULL) { return pCardNodeHead; }
	if (strcmp(pCardNodeHead->data.aName, strNo.c_str()) == 0) {
		p = pCardNodeHead;
		pCardNodeHead = pCardNodeHead->next;
		free (p);
		delete p;
		return pCardNodeHead;
	}
	for (q = pCardNodeHead; q->next; q = q->next)
		if (strcmp(q->next->data.aName, strNo.c_str()) == 0) {
			p = q->next;
			q->next = p->next;
			free(p);
			delete p;
			return pCardNodeHead;
		}
	return (pCardNodeHead);
}
void DeleteCard(CardNode* pCardNodeHead) {
	string strNo;
	string strPwd;
	int nCardIndex;
	if (inputNoPwd(strNo, strPwd)) {
		Card* pCard = cardIsExist(strNo, nCardIndex, pCardNodeHead);
		if (pCard == NULL)
		{
			cout << "该卡不存在\n";
		}
		else if (pCard->nStatus != INVALID)
		{
			cout << "该卡未注销，请注销后删除\n";
		}
		else if (strcmp(pCard->aPwd, strPwd.c_str()) != 0)
		{
			cout << "卡号密码不匹配\n";
		}
		else {
			updateDeleteCard(pCard, CARDPATH, strNo);
			pCardNodeHead = deleteList(pCardNodeHead, strNo);/*CardListInit(CARDPATH);*/
			cout << "删除成功" << endl;
		}
	}
	else
	{
		cout << endl << endl << "卡号或者密码格式不正确!" << endl << endl;
	}
}
void DeleteReport(ReportNode* pReportNodeHead) {
	string strNo;
	int nReportIndex;
	if (inputcard(strNo)) {
		Report* pReport = reportIsExist(strNo, nReportIndex, pReportNodeHead);
		if (pReport == NULL)
		{
			cout << "该卡不存在\n";
		}
		else {
			updateDeleteReport(pReport, REPORTPATH, strNo);
			pReportNodeHead = ReportListInit(REPORTPATH);
			cout << "删除成功" << endl;
		}
	}
	else
	{
		cout << endl << endl << "卡号或者密码格式不正确!" << endl << endl;
	}
}