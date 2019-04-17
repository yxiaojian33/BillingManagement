#pragma once
#include<time.h>

struct Card
{
	char aName[8];
	char aPwd[8];
	int nStatus;                    // 卡状态(UNUSE-未上机；USING-正在上机；INVALID-已注销）
	time_t tStart;
	float fTotalUse;                 //累计金额
	time_t tLast;                    // 最后使用时间，long 
	int nUseCount;                   //使用次数
	float fBalance;                  //余额
};
struct CardNode
{
	Card data;
	struct CardNode *next;
};
struct Billing
{
	char aCardName[8];
	time_t tStart;
	time_t tEnd;
	float fAmount;
	float fTotalUse;
	int nStatus;
	int nDel;
};
struct BillingNode
{
	Billing data;
	struct BillingNode *next;
};
struct LogonInfo
{
	char aCardName[8];
	time_t tLogon;
	float fBalance;
};

struct SettleInfo
{
	char aCardName[8];
	int tStart;
	int tEnd;
	float fAmount;
	float fBalance;
	float fTotalUse;
};
struct TopupInfo
{
	char aCardName[8];
	float fBalance;
};
struct ReturnfeeInfo
{
	char aCardName[8];
	float fBalance;
};
struct CancelInfo
{
	char aCardName[8];
	float fBalance;
};
struct Report
{
	char aName[18]; // 卡号
	int Operate; // 卡操作(ADD-注册卡 LOGIN-上机 LOGOUT-下机 TOPUP-充值 RETURNFEE-退费)
	time_t operatetime; // 操作时间
	float OperateUse; // 操作金额
	float fBalance; // 余额
};
struct ReportNode {
	Report data;
	struct ReportNode *next;
};
