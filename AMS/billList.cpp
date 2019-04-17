#include"stdafx.h"
#include "model.h"
#include"menu.h"
#include"cardlist.h"
#include"global.h"
#include"reportlist.h"
#include"tool.h"
#include <string>
#include<iostream>
#include<fstream>
using namespace std;
bool saveBilling(const Billing* pBilling, const  string filename)//保存金额
{
	ofstream ofile(filename, ios::binary | ios::app);
	ofile.write((char*)pBilling, sizeof(Billing));
	ofile.close();
	return true;
}
bool updateBilling(Billing* pBilling, const  string filename, int nBillingIndex)//更新金额信息
{
	ofstream ofile(filename, ios::binary | ios::app);
	ofile.write((char*)pBilling, sizeof(Billing));
	ofile.close();
	return true;
}
bool updateCard(const Card* pCard, const string pPath, int nCardIndex)
{
	fstream ofile;
	ofile.open(pPath, ios_base::in | ios_base::out);
	ofile.seekp(sizeof(Card)*nCardIndex, ios_base::beg);
	ofile.write((char*)pCard, sizeof(Card));
	ofile.close();
	return true;
}
BillingNode* BillListInit(const string billingFilename) {
	ifstream billingfile(billingFilename);
	BillingNode *pBillingNode, *pBillingNodeHead, *pBillingNodeTail;
	Billing billing;
	if (!billingfile.is_open()) { return NULL; }
	pBillingNodeHead = NULL;
	pBillingNodeTail = NULL;
	while (1) {
		billingfile.read((char*)&billing, sizeof(Billing));
		if (billingfile.eof())
		{
			break;
		}
		pBillingNode = new BillingNode;
		pBillingNode->data = billing;
		if (pBillingNodeHead == NULL)
		{
			pBillingNodeHead = pBillingNode;
			pBillingNodeTail = pBillingNode;
		}
		else
		{
			pBillingNodeTail->next = pBillingNode;
			pBillingNodeTail = pBillingNode;
		}
	}

	pBillingNodeTail->next = NULL;
	billingfile.close();
	return pBillingNodeHead;
}
Billing* billingIsExist(string strNo, int& nBillingIndex, BillingNode *pBillingNodeHead) {
	BillingNode *pBillingNode = pBillingNodeHead;

	nBillingIndex = 0;
	while (pBillingNode != NULL)
	{
		if (strcmp(pBillingNode->data.aCardName, strNo.c_str()) == 0 && pBillingNode->data.nStatus == NOSETTLEMENT)
		{
			return &(pBillingNode->data);
		}
		pBillingNode = pBillingNode->next;
		nBillingIndex++;
	}
	return NULL;
}
double getAmount(time_t tStart) {//计算费用
	double dbAmount = 0;
	int nCount = 0;            // 上机的时间单元数，每个单元15分钟 
	int nSec = 0;              // 消费时间(单位：秒)  
	int nMinutes = 0;           // 消费时间(单位：分钟)
	time_t tEnd = time(NULL);  // 结算时间为当前时间 
	// 计算消费时长
	nSec = (int)(tEnd - tStart);
	nMinutes = nSec / 60;

	// 计算消费的时间单元数 
	if (nMinutes % UNIT == 0)
	{
		nCount = nMinutes / UNIT;
	}
	else
	{
		nCount = nMinutes / UNIT + 1;
	}

	if (nCount == 0)
	{
		nCount = 1;
	}

	// 计算消费金额  
	dbAmount = nCount * CHARGE;
	return dbAmount;
}
int logon(string strNo, string strPwd, LogonInfo* pInfo, CardNode* pCardNodeHead, BillingNode **ppBillingNodeHead, ReportNode** ppReportNodeHead)
{
	Billing billing;
	int nCardIndex;
	Card* pCard = cardIsExist(strNo, nCardIndex, pCardNodeHead);
	if (pCard == NULL)
		return NOFINDCARD;//未查询到卡

	if (strcmp(pCard->aPwd, strPwd.c_str()) != 0)
		return NOMATCH;//密码不匹配

	if (pCard->nStatus == USING)
	{
		return USING;//使用中
	}

	if (pCard->nStatus == INVALID)
	{
		return INVALID;//卡已注销
	}

	if (pCard->fBalance <= 0)
	{
		return ENOUGHMONEY;//余额为0
	}
	pCard->nStatus = USING;
	pCard->nUseCount++;
	pCard->tLast = time(NULL);


	updateCard(pCard, CARDPATH, nCardIndex);

	strcpy(billing.aCardName, strNo.c_str());
	billing.tStart = time(NULL);
	billing.tEnd = 0;
	billing.nStatus = NOSETTLEMENT;
	billing.fAmount = 0;
	saveBilling(&billing, BILLINGPATH);
	BillingNode* pBillingNode = new BillingNode;
	pBillingNode->data = billing;
	BillingNode* pBillingNodeTail = *ppBillingNodeHead;

	if (*ppBillingNodeHead == NULL)
	{
		*ppBillingNodeHead = pBillingNode;
		pBillingNodeTail = pBillingNode;
	}
	else
	{
		while (pBillingNodeTail->next != NULL)
			pBillingNodeTail = pBillingNodeTail->next;
		pBillingNodeTail->next = pBillingNode;
		pBillingNodeTail = pBillingNode;
	}
	pBillingNodeTail->next = NULL;
	strcpy(pInfo->aCardName, strNo.c_str());
	pInfo->fBalance = pCard->fBalance;
	pInfo->tLogon = billing.tStart;
	addNewReport(strNo, LOGIN, 0, pCard->fBalance, ppReportNodeHead);
	return SUCCESS;
}

int settle(string strNo, string strPwd, SettleInfo* pInfo, CardNode* const pCardNodeHead, BillingNode* const pBillingNodeHead, ReportNode** ppReportNodeHead) {
	int nCardIndex;
	Card* pCard = cardIsExist(strNo, nCardIndex, pCardNodeHead);
	if (pCard == NULL)
	{
		return NOFINDCARD;
	}
	if (strcmp(pCard->aPwd, strPwd.c_str()) != 0)
	{
		return NOMATCH;
	}
	if (pCard->nStatus == INVALID)
	{
		return INVALID;//卡已注销
	}
	if (pCard->nStatus != USING)
	{
		return UNUSE;
	}
	int nBillingIndex;
	Billing* pBilling = billingIsExist(strNo, nBillingIndex, pBillingNodeHead);
	if (pBilling == NULL)
	{
		cout << "计费信息为空" << endl;   return UNUSE;
	}
	double dbAmount = getAmount(pCard->tLast);
	float fBalance = pCard->fBalance - (float)dbAmount;
	pCard->fTotalUse+ (float)dbAmount;
	if (fBalance < 0)
	{
		return ENOUGHMONEY;
	}
	pCard->fTotalUse=pCard->fTotalUse + (float)dbAmount;
	pCard->fBalance = fBalance;
	pCard->nStatus = UNUSE;
	pBilling->tStart = pCard->tLast;
	pCard->tLast = time(NULL);
	updateCard(pCard, CARDPATH, nCardIndex);
	pBilling->fAmount = (float)dbAmount;
	pBilling->nStatus = YESSETTLEMENT;
	pBilling->tEnd = time(NULL);
	updateBilling(pBilling, BILLINGPATH, nBillingIndex);
	strcpy(pInfo->aCardName, strNo.c_str());
	pInfo->fAmount = (float)dbAmount;
	pInfo->fBalance = fBalance;
	pInfo->tStart = pBilling->tStart;
	pInfo->tEnd = time(NULL);
	addNewReport(strNo, LOGOUT, (float)dbAmount, pCard->fBalance, ppReportNodeHead);
	return SUCCESS;
}

int topup(string strNo, string strPwd, TopupInfo* pInfo,CardNode* const pCardNodeHead, ReportNode** ppReportNodeHead) {
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
	if (strcmp(pCard->aPwd, strPwd.c_str()) != 0)
	{
		return NOMATCH;
	}
	double fee;
	while (true)
	{
		cout << "请输入充值金额，单位：元：";
		cin >> fee;
		cin.clear();
		cin.sync();
		if ((fee * 100 != int(fee * 100)) && fee >= 0)//控制输入最多小数点后两位
			cout << "输入金额格式有误" << endl;
		else break;
	}
	float fBalance = pCard->fBalance + fee;
	pCard->fBalance = fBalance;
	updateCard(pCard, CARDPATH, nCardIndex);
	strcpy(pInfo->aCardName, strNo.c_str());
	pInfo->fBalance = fBalance;
	addNewReport(strNo, TOPUP,fee, pCard->fBalance, ppReportNodeHead);
	return SUCCESS;
}

int returnfee(string strNo, string strPwd, ReturnfeeInfo* pInfo, CardNode* const pCardNodeHead, ReportNode** ppReportNodeHead) {
	int nCardIndex;
	Card* pCard = cardIsExist(strNo, nCardIndex, pCardNodeHead);
	if (pCard == NULL)
	{
		return NOFINDCARD;
	}
	if (strcmp(pCard->aPwd, strPwd.c_str()) != 0)
	{
		return NOMATCH;
	}
	if (pCard->nStatus == INVALID)
	{
		return INVALID;//卡已注销
	}
	if (pCard->nStatus == USING)
	{
		return USING;//使用中
	}
	double fee;
	while (true)
	{
		cout << "请输入退费金额，单位：元：";
		cin >> fee;
		cin.clear();
		cin.sync();
		if ((fee * 100 != int(fee * 100)) && fee >= 0)
			cout << "输入金额格式有误" << endl;
		else break;
	}
	if (pCard->fBalance - fee < 0)
	{
		return ENOUGHMONEY;
	}
	float fBalance = pCard->fBalance -fee;
	pCard->fBalance = fBalance;
	updateCard(pCard, CARDPATH, nCardIndex);
	strcpy(pInfo->aCardName, strNo.c_str());
	pInfo->fBalance = fBalance;
	addNewReport(strNo, RETURNFEE, fee, pCard->fBalance, ppReportNodeHead);
	return SUCCESS;
}
