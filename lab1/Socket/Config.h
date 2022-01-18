#pragma once
#include <string>
#include <winsock2.h>
#define CONFIG_PATH "config.ini"
using namespace std;

//保存配置信息
class Config
{
public:
	static int MAXCONNECTION;		//最大连接数
	static int BUFFERLENGTH;		//缓冲区大小
	static string SERVERADDRESS;  //服务器地址
	static int PORT;				//服务器端口
	static u_long BLOCKMODE;			//SOCKET阻塞模式
	static string ROOT;					//虚拟地址根目录
	static void loadConfig();			//加载配置
private:
	Config(void);
	~Config(void);
};