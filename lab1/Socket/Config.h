#pragma once
#include <string>
#include <winsock2.h>
#define CONFIG_PATH "config.ini"
using namespace std;

//����������Ϣ
class Config
{
public:
	static int MAXCONNECTION;		//���������
	static int BUFFERLENGTH;		//��������С
	static string SERVERADDRESS;  //��������ַ
	static int PORT;				//�������˿�
	static u_long BLOCKMODE;			//SOCKET����ģʽ
	static string ROOT;					//�����ַ��Ŀ¼
	static void loadConfig();			//��������
private:
	Config(void);
	~Config(void);
};