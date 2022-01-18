#pragma once
#pragma once
#include <winsock2.h>
#include <list>
#include <string>
#include <map>

using namespace std;

//服务器

class Server {
private:
	SOCKET srvSocket;			//服务器监听socket
	bool on;
	char* recvBuf;				//接受缓冲区
	fd_set rfds;				//用于检查socket是否有数据到来的的文件描述符
	fd_set wfds;				//用于检查socket是否可以发送的文件描述符
	sockaddr_in srvAddr;		//服务器端IP地址
	int sockNum;				//同时发送请求的客户端数量
	list<SOCKET>* sessions;							//当前的会话socket队列
	list<SOCKET>* closedSessions;					//所有已失效的会话socket队列
	map<SOCKET, string>* clientRequestMaps;				//客户端地址列表，地址以(key,value)对形式保存，key为SOCKET类型，value为string类型
protected:
	virtual string  GetClientAddress(SOCKET s);
	virtual void AddSession(SOCKET session);								//将新的会话socket加入队列
	virtual void AddClosedSession(SOCKET session);							//将失效的会话socket加入队列
	virtual void RemoveClosedSession(SOCKET closedSession);					//将失效的SOCKET从会话socket队列删除
	virtual void RemoveClosedSession();										//将失效的SOCKET从会话socket队列删除
	virtual int AcceptRequestionFromClient();								//等待客户端连接请求
	virtual void recvMessage(SOCKET s);										//从SOCKET s接受消息
	virtual void  ReceieveMessageFromClients();								//接受客户端发来的信息
	virtual void sendMessage(SOCKET s, string msg);							//向SOCKET s发送消息
	virtual void ForwardMessage();										//向其他客户转发信息
public:
	Server(void);
	virtual ~Server(void);
	virtual void startUp();
	virtual void turnDown();
	virtual int winsockStartup();		//初始化Winsock
	virtual int serverStartup();		//初始化Server，包括创建SOCKET，绑定到IP和PORT
	virtual int listenStartup();		//开始监听客户端请求
	virtual int loop();					//循环执行"等待客户端请求"->“向其他客户转发信息”->"等待客户端消息"
};
