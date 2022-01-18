#pragma once
#include "PrintMenu.h"
using namespace::std;
void printMenu() {
	cout << endl;
	cout << "*******************************************" << endl;
	cout << "*                请输入功能               *" << endl;
	cout << "*    1、启动服务器       2、关闭服务器    *" << endl;
	cout << "*    3、查看服务器配置   4、修改配置信息  *" << endl;
	cout << "*    5、退出程序                          *" << endl;
	cout << "*******************************************" << endl;
}

void printConfig() {
	cout << "服务器配置如下：" << endl;
	cout << "服务器ip地址：" <<Config::SERVERADDRESS << "\n"
  		 << "服务器端口号：" <<Config::PORT << "\n"
		 << "接收信息最大缓冲区长度：" <<Config::BUFFERLENGTH << "\n"
		 << "最大链接数：" <<Config::MAXCONNECTION << "\n"
		 << "虚拟地址根目录：" <<Config::ROOT << "\n" << endl;
} 