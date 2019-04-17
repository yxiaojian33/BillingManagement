//系统菜单主函数
#include"stdafx.h"
#include<string>
#include "menu.h"
#include<vector>
#include<iostream>
#include<iomanip>
#include"model.h"
#include"global.h"
#include"cardlist.h"
#include"billList.h"
#include"reportlist.h"
#include"tool.h"
using namespace std;
//添加卡
void addCard(CardNode** ppCardNodeHead, ReportNode** ppReportNodeHead)
{
	string strNo;
	string strPwd;   
	float fBalance;
	if (inputNoPwdBalance(strNo, strPwd, fBalance))
	{
		int nResult = addNewCard(strNo, strPwd, fBalance, ppCardNodeHead, ppReportNodeHead);
		switch (nResult)
		{
		case FINDCARD:
		{
			cout << endl << endl << "卡【" << strNo << "】已经存在，添加新 卡失败！" << endl << endl;
			break;
		}
		case SUCCESS:
		{
			cout << endl << endl << "添加新卡成功!" << endl << endl;
			cout << "新卡卡号：" << strNo << endl;
			cout << "新卡金额：" << setiosflags(ios::fixed) << setprecision(2) <<fBalance << endl;
			break;
		}
		default:
		{
			cout << endl << endl << "系统错误!" << endl << endl;
			break;
		}
		}
	}
	else 
	{
		cout << endl << endl << "输入的【卡号，密码，金额】等信息格式不符号要求， 添加卡失败!" << endl << endl;
	}
}
//查询卡
void queryCard(CardNode* const pCardNodeHead)
{
	string strNo;
	cout << "请输入卡号，若输入0或！则显示全部卡号信息\n";
	cin >> strNo;
	if (strNo == "!"|| strNo == "0") displayCard(pCardNodeHead);
	else displayCardone(pCardNodeHead, strNo);
	}
//上机
void loginCard(CardNode* pCardNodeHead, BillingNode **ppBillingNodeHead, ReportNode** ppReportNodeHead) {
	string strNo;
	string strPwd;
	LogonInfo* pInfo = new LogonInfo;
	char aTime[TIMELENGTH] = { 0 };
	if (inputNoPwd(strNo, strPwd)) {
		cout << endl << "----------上机信息----------" << endl;
		int nResult = logon(strNo, strPwd, pInfo, pCardNodeHead, ppBillingNodeHead, ppReportNodeHead);
		switch (nResult)
		{
		case NOFINDCARD:
		case NOMATCH: {
			cout << endl << endl << "卡不存在，或卡信息不对，不能上机！" << endl << endl;
			break;
		}
		case SUCCESS:
		{
			timeToString(pInfo->tLogon, aTime);
			cout << endl;
			cout << "卡号: \t" << strNo << endl;
			cout << "余额: \t" << pInfo->fBalance << endl;
			cout << "上机时间：\t" << aTime << endl;
			break;
		}
		case USING:
		{
			cout << endl << "该卡正在使用，不能重复上机！" << endl;
			break;
		}
		case INVALID:
		{
			cout << endl << "该卡已注销，不能上机！" << endl;
			break;
		}
		case ENOUGHMONEY:
		{
			cout << endl << "卡余额为0，不能上机！" << endl;
			break;
		}
		default:break;
		}
	}
	else
	{
		cout << endl << endl << "卡号或者密码格式不正确，上机失败!" << endl << endl;
	}
	delete pInfo;
}
//下机
void logoutCard(CardNode* pCardNodeHead, BillingNode* pBillingNodeHead, ReportNode** ppReportNodeHead) {
	string strNo;
	string strPwd;
	SettleInfo* pInfo = new SettleInfo;
	char aStartTime[TIMELENGTH] = { 0 };
	char aEndTime[TIMELENGTH] = { 0 };
	if (inputNoPwd(strNo, strPwd)) {
		cout << "----------下机信息如下----------\n";
		int nResult = settle(strNo, strPwd, pInfo, pCardNodeHead, pBillingNodeHead, ppReportNodeHead);
		switch (nResult)
		{
		case NOFINDCARD:
		case NOMATCH:
		{
			cout << endl << endl << "卡不存在，或卡信息不对，下机失败！" << endl << endl;
			break;
		}
		case SUCCESS:
		{
			timeToString(pInfo->tStart, aStartTime);
			timeToString(pInfo->tEnd, aEndTime);
			cout << endl;
			cout << "卡号: \t" << strNo << endl;
			cout << "消费: \t" << pInfo->fAmount << endl;
			cout << "余额: \t" << setiosflags(ios::fixed) << setprecision(2)<<pInfo->fBalance << endl;
			cout << "上机时间：\t" << aStartTime << endl;
			cout << "下机时间：\t" << aEndTime << endl;
			break;
		}
		case INVALID:
		{
			cout << endl << "该卡已注销，不能执行该操作！" << endl;
			break;
		}
		case UNUSE:
		{
			cout << endl << endl << "该卡没有上机！" << endl << endl;
			break;
		}
		case ENOUGHMONEY:
		{
			cout << endl << endl << "卡余额不足，请先充值再下机！" << endl << endl;
		}
		default:break;
		}
	}
	else
	{
		cout << endl << endl << "卡号或者密码格式不正确，下机失败!" << endl << endl;
	}

	delete pInfo;
}

void top_up(CardNode* pCardNodeHead, ReportNode** ppReportNodeHead)
{
	string strNo;
	string strPwd;
	TopupInfo* pInfo = new TopupInfo;
	if (inputNoPwd(strNo, strPwd)) {
		cout << "----------充值信息如下----------\n";
		int nResult = topup(strNo, strPwd, pInfo, pCardNodeHead, ppReportNodeHead);
		switch (nResult)
		{
		case NOFINDCARD:
		case NOMATCH:
		{
			cout << endl << endl << "卡不存在，或卡信息不对，充值失败！" << endl << endl;
			break;
		}
		case INVALID:
		{
			cout << endl << "该卡已注销，不能执行该操作！" << endl;
			break;
		}
		case SUCCESS:
		{
			cout << endl;
			cout << "卡号: \t" << strNo << endl;
			cout << "余额: \t" << setiosflags(ios::fixed) << setprecision(2) << pInfo->fBalance << endl;
			break;
		}
		default:break;
		}
	}
	else
	{
		cout << endl << endl << "卡号或者密码格式不正确，充值失败!" << endl << endl;
	}

	delete pInfo;
}
//退费
void return_fee(CardNode* pCardNodeHead, ReportNode** ppReportNodeHead) {
	string strNo;
	string strPwd;
	ReturnfeeInfo* pInfo = new ReturnfeeInfo;
	if (inputNoPwd(strNo, strPwd)) {
		cout << "----------退费信息如下----------\n";
		int nResult = returnfee(strNo, strPwd, pInfo, pCardNodeHead, ppReportNodeHead);
		switch (nResult)
		{
		case NOFINDCARD:
		case NOMATCH:
		{
			cout << endl << endl << "卡不存在，或卡信息不对，退费失败！" << endl << endl;
			break;
		}
		case INVALID:
		{
			cout << endl << "该卡已注销，不能执行该操作！" << endl;
			break;
		}
		case USING:
		{
			cout << endl << "该卡正在使用，不能退费！" << endl;
			break;
		}
		case ENOUGHMONEY:
		{
			cout << endl << endl << "卡余额不足，请重新输入！" << endl << endl;
			break;
		}
		case SUCCESS:
		{
			cout << endl;
			cout << "卡号: \t" << strNo << endl;
			cout << "余额: \t" << setiosflags(ios::fixed) << setprecision(2) << pInfo->fBalance << endl;
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
//查询统计
void statis(ReportNode** ppReportNodeHead) {
	char selection;
	cout << "1.查询 2.统计" << endl;
	do {
		selection = 'a';
		cin >> selection;
		cin.clear();
		cin.sync();
		switch (selection) {
		case '1':
		{ cout << endl << "-------查询 --------" << endl << endl;
		find(*ppReportNodeHead);
		break; }
		case '2':
		{ cout << endl << "-------统计--------" << endl << endl;
		sum(*ppReportNodeHead);
		break; }
		default:
		{cout << "输入的菜单编号错误！\n"; break; }
		}
		cout << endl;
	} while (selection != '0');
}
void DeleteOff(CardNode* pCardNodeHead, ReportNode** ppReportNodeHead) {
	char selection;
	cout << "1.注销 2.删除卡 3.删除使用记录" << endl;
	do {
		selection = 'a';
		cin >> selection;
		cin.clear();
		cin.sync();
		switch (selection) {
		case '1':
		{ cout << endl << "-------注销 --------" << endl << endl;
		invalid(pCardNodeHead, ppReportNodeHead);
		break; }
		case '2':
		{ cout << endl << "-------删除卡--------" << endl << endl;
		DeleteCard(pCardNodeHead);
		break; }
		case '3':
		{ cout << endl << "-------删除使用记录--------" << endl << endl;
		DeleteReport(*ppReportNodeHead);
		break; }
		default:
		{cout << "输入的菜单编号错误！\n"; break; }
		}
		cout << endl;
	} while (selection != '0');
}
//删除节点
void clearData(CardNode* pCardNodeHead) 
{
	CardNode *pCardNode;
	while (pCardNodeHead != NULL)
	{
		pCardNode = pCardNodeHead; 
		pCardNodeHead = pCardNodeHead->next;
		delete pCardNode;
	}
}
