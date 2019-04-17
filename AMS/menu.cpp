//功能类函数
#include"stdafx.h"
#include<string>
#include<iostream>
#include<vector>
#include"model.h"
#include"global.h"
#include"tool.h"
#include<iomanip>
#include <fstream> 
using namespace std;

//输出菜单
void outputMenu()
{
	cout << "------计费系统菜单------" << endl << endl;
	cout << "1.添加卡" << endl;
	cout << "2.查询卡" << endl;
	cout << "3.上机" << endl;
	cout << "4.下机" << endl;
	cout << "5.充值" << endl;
	cout << "6.退费" << endl;
	cout << "7.查询统计" << endl;
	cout << "8.注销卡" << endl;
	cout << "0.退出" << endl;
	cout << "请选择菜单项编号（0~8):";
}
//输入卡号
bool inputcard(string& strNo) {
	while(true) 
	{
		cout << "请输入卡号（5到8位）：";
		cin >> strNo;
		cin.clear();
		cin.sync();
		if(strNo.length() > 8|| strNo.length()<5) {
			cout << "卡号长度不正确！" << endl;
			char ch = 'M';
			while (ch != 'N'&&ch != 'Y') {
				cout << "是否重新输入？是（Y）/否（N)";
				cin >> ch;
				ch = toupper(ch);
				cin.clear();
				cin.sync();
			}
			if(ch == 'N') { return false; }
		}
		else { break; }
	}
}
//输入密码
bool inputCardPwd(string& strPwd) {
	while (true)
	{
		cout << "请设置密码（5到8位）：";
		cin >> strPwd;
		cin.clear();
		cin.sync();
		if (strPwd.length() < 5|| strPwd.length()>8) {
			cout << "密码长度不正确！" << endl;
			char ch = 'M';
			while (ch != 'N'&&ch != 'Y') {
				cout << "是否重新设置？是（Y） / 否（N)";
				cin >> ch;
				ch = toupper(ch);
				cin.clear();
				cin.sync();
			}
			if (ch == 'N') { return false; }
		}
		else { break; }
	}
}
//输入金额
bool inputCardBalance(float& fBalance) {
	while (true)
	{
		cout << "请输入充值金额，单位：元：";
		cin >> fBalance;
		cin.clear();
		cin.sync();
		if ((fBalance*100!=int(fBalance * 100))&&fBalance>=0) {
			cout << "输入金额格式有误" << endl;
			char ch = 'M';
			while (ch != 'N'&&ch != 'Y') {
				cout << "是否重新输入？是（Y） / 否（N)";
				cin >> ch;
				ch = toupper(ch);
				cin.clear();
				cin.sync();
			}
			if (ch == 'N') { return false; }
		}
		else { break; }
	}
}
//是否输入账号及密码
bool inputNoPwd(string& strNo, string& strPwd) 
{
	if (inputcard(strNo) && inputCardPwd(strPwd)) return true;
	else return false;
}
//是否输入全部信息
bool inputNoPwdBalance(string& strNo, string& strPwd, float& fBalance) {
	if (inputcard(strNo)&& inputCardPwd(strPwd)&& inputCardBalance(fBalance)) return true; 
	else return false;
}
