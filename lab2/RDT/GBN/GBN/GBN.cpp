// StopWait.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "Global.h"
#include "RdtSender.h"
#include "RdtReceiver.h"
#include "GBNRdtSender.h"
#include "GBNRdtReceiver.h"


int main(int argc, char* argv[])
{
	RdtSender *ps = new GBNRdtSender();
	RdtReceiver * pr = new GBNRdtReceiver();
//	pns->setRunMode(0);  //VERBOSģʽ
	pns->setRunMode(1);  //����ģʽ
	pns->init();
	pns->setRtdSender(ps);
	pns->setRtdReceiver(pr);
	pns->setInputFile("D:\\workfile\\ComputerTelecommunicationsAndNetwork\\lab\\lab2\\RDT\\GBN\\input.txt");
	pns->setOutputFile("D:\\workfile\\ComputerTelecommunicationsAndNetwork\\lab\\lab2\\RDT\\GBN\\output.txt");

	pns->start();

	delete ps;
	delete pr;
	delete pUtils;									//ָ��Ψһ�Ĺ�����ʵ����ֻ��main��������ǰdelete
	delete pns;										//ָ��Ψһ��ģ�����绷����ʵ����ֻ��main��������ǰdelete
	
	return 0;
}
