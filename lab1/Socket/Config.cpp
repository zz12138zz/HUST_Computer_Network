#define _CRT_SECURE_NO_WARNINGS
#include "Config.h"
#include <string>
#include <Windows.h>
#include <tchar.h>
#include <iostream>
#include <cstdio>;
#define count_of(exp)  (sizeof(exp) / sizeof(exp[0]))
using namespace std;

/*��ini�ļ��ж�����TCHAR����ת��Ϊstring*/
string tChar2String(TCHAR* STR){
	int iLen = WideCharToMultiByte(CP_ACP, 0,STR, -1, NULL, 0, NULL, NULL);
	char* chRtn = new char[iLen * sizeof(char)];
	WideCharToMultiByte(CP_ACP, 0, STR, -1, chRtn, iLen, NULL, NULL);
	string str(chRtn);
	return str;

}

Config::Config(void)
{
}

Config::~Config(void)
{
}

string Config::SERVERADDRESS;	//������IP��ַ
int Config::MAXCONNECTION;				//���������
int Config::BUFFERLENGTH;				//��������С
int Config::PORT;						//�������˿�
u_long Config::BLOCKMODE=1;					//SOCKETΪ������ģʽ
string Config::ROOT ;
void Config::loadConfig() {
	TCHAR szIp[25] = { 0 };
	TCHAR szRoot[100] = { 0 };
	GetPrivateProfileString(_T("ServerInfo"), _T("SERVERADDRESS"),  _T("127.0.0.1"),szIp,count_of(szIp),_T(CONFIG_PATH));
	GetPrivateProfileString(_T("ServerInfo"), _T("ROOT"), _T(""), szRoot, count_of(szRoot), _T(CONFIG_PATH));
	Config::SERVERADDRESS = tChar2String(szIp);
	Config::ROOT = tChar2String(szRoot);
	Config::MAXCONNECTION = GetPrivateProfileInt(_T("ServerInfo"), _T("MAXCONNECTION"), 50, _T(CONFIG_PATH));
	Config::BUFFERLENGTH = GetPrivateProfileInt(_T("ServerInfo"), _T("BUFFERLENGTH"), 2048, _T(CONFIG_PATH));
	Config::PORT = GetPrivateProfileInt(_T("ServerInfo"), _T("PORT"), 5050, _T(CONFIG_PATH));
}