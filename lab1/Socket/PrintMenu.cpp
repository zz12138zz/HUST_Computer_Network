#pragma once
#include "PrintMenu.h"
using namespace::std;
void printMenu() {
	cout << endl;
	cout << "*******************************************" << endl;
	cout << "*                �����빦��               *" << endl;
	cout << "*    1������������       2���رշ�����    *" << endl;
	cout << "*    3���鿴����������   4���޸�������Ϣ  *" << endl;
	cout << "*    5���˳�����                          *" << endl;
	cout << "*******************************************" << endl;
}

void printConfig() {
	cout << "�������������£�" << endl;
	cout << "������ip��ַ��" <<Config::SERVERADDRESS << "\n"
  		 << "�������˿ںţ�" <<Config::PORT << "\n"
		 << "������Ϣ��󻺳������ȣ�" <<Config::BUFFERLENGTH << "\n"
		 << "�����������" <<Config::MAXCONNECTION << "\n"
		 << "�����ַ��Ŀ¼��" <<Config::ROOT << "\n" << endl;
} 