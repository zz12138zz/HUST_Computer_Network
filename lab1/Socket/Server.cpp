#pragma once
#include <iostream>
#include <string>
#include "Server.h"
#include "WinsockEnv.h"
#include "Config.h"
#include <winsock2.h>
#include <algorithm>
#include "HttpProcessor.h"
#include <thread>
#include "PrintMenu.h"
#pragma comment(lib, "Ws2_32.lib")

#pragma warning(disable:4996)

Server::Server(void) {
	this->recvBuf = new char[Config::BUFFERLENGTH]; //初始化接受缓冲区
	this->startUp();
	memset(this->recvBuf, '\0', Config::BUFFERLENGTH);
	this->sessions = new list<SOCKET>();
	this->closedSessions = new list<SOCKET>();
	this->clientRequestMaps = new map<SOCKET, string>();
}

Server::~Server(void)
{
	//释放接受缓冲区
	if (this->recvBuf != NULL) {
		delete this->recvBuf;
		this->recvBuf = NULL;
	}


	//关闭server socket
	if (this->srvSocket != NULL) {
		closesocket(this->srvSocket);
		this->srvSocket = NULL;
	}

	//关闭所有会话socket并释放会话队列
	if (this->sessions != NULL) {
		for (list<SOCKET>::iterator itor = this->sessions->begin(); itor != this->sessions->end(); itor++)
			closesocket(*itor); //关闭会话
		delete this->sessions;  //释放队列
		this->sessions = NULL;
	}
	//释放失效会话队列
	if (this->closedSessions != NULL) {
		for (list<SOCKET>::iterator itor = this->closedSessions->begin(); itor != this->closedSessions->end(); itor++)
			closesocket(*itor); //关闭会话
		delete this->closedSessions;//释放队列
		this->closedSessions = NULL;
	}

	//释放客户端地址列表
	if (this->clientRequestMaps != NULL) {
		this->clientRequestMaps->clear();
		delete this->clientRequestMaps;
		this->clientRequestMaps = NULL;
	}

	WSACleanup(); //清理winsock 运行环境
}

//标志服务器打开
void Server::startUp() { 
	this->on = true;
}
//标志服务器关闭
void Server::turnDown() { 
	this->on = false;
}

//初始化Winsock
int Server::winsockStartup() {
	if (WinsockEnv::Startup() == -1) return -1;	//初始化Winsock
	return 0;
}

//初始化Server，包括创建sockect，绑定到IP和PORT
int Server::serverStartup() {
	//创建 TCP socket
	this->srvSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (this->srvSocket == INVALID_SOCKET) {
		cout << "Server socket creare error !\n";
		WSACleanup();
		return -1;
	}
	cout << "Server socket create ok!\n";

	//设置服务器IP地址和端口号
	this->srvAddr.sin_family = AF_INET;
	this->srvAddr.sin_port = htons(Config::PORT);
	this->srvAddr.sin_addr.S_un.S_addr = inet_addr(Config::SERVERADDRESS.c_str()); //这是另外一种设置IP地址的方法

	//绑定 socket to Server's IP and port
	int rtn = bind(this->srvSocket, (LPSOCKADDR) & (this->srvAddr), sizeof(this->srvAddr));
	if (rtn == SOCKET_ERROR) {
		cout << "Server socket bind error!\n";
		closesocket(this->srvSocket);
		WSACleanup();
		return -1;
	}

	cout << "Server socket bind ok!\n";
	return 0;
}

//开始监听,等待客户的连接请求
int Server::listenStartup() {
	int rtn = listen(this->srvSocket, Config::MAXCONNECTION);
	if (rtn == SOCKET_ERROR) {
		cout << "Server socket listen error!\n";
		closesocket(this->srvSocket);
		WSACleanup();
		return -1;
	}

	cout << "Server socket listen ok!\n";
	return 0;
}

//获得客户端IP地址和端口号
string  Server::GetClientAddress(SOCKET s) {
	string clientAddress; //clientAddress是个空字符串， clientAddress.empty() is true
	sockaddr_in clientAddr;
	int nameLen, rtn;

	nameLen = sizeof(clientAddr);
	rtn = getsockname(s, (LPSOCKADDR)&clientAddr, &nameLen);
	if (rtn != SOCKET_ERROR) {
		clientAddress += inet_ntoa(clientAddr.sin_addr);
		clientAddress += " ";
		clientAddress += to_string(clientAddr.sin_port);
	}

	return clientAddress;
}

//将新的会话SOCKET加入队列
void Server::AddSession(SOCKET session) {
	if (session != INVALID_SOCKET) {
		this->sessions->insert(this->sessions->end(), session);
	}
}

//将失效的会话SOCKET加入队列
void Server::AddClosedSession(SOCKET session) {
	if (session != INVALID_SOCKET) {
		this->closedSessions->insert(this->closedSessions->end(), session);
	}
}


//将单个失效的SOCKET从session队列删除
void Server::RemoveClosedSession(SOCKET closedSession) {
	if (closedSession != INVALID_SOCKET) {
		list<SOCKET>::iterator itor = find(this->sessions->begin(), this->sessions->end(), closedSession);
		if (itor != this->sessions->end())
			this->sessions->erase(itor);
	}
}

//将所有失效的SOCKET从会话session队列删除
void Server::RemoveClosedSession() {
	for (list<SOCKET>::iterator itor = this->closedSessions->begin(); itor != this->closedSessions->end(); itor++) {
		this->RemoveClosedSession(*itor);
	}
}


//从SOCKET s接受消息
void Server::recvMessage(SOCKET socket) {
	int receivedBytes = recv(socket, this->recvBuf, Config::BUFFERLENGTH, 0);
	if (receivedBytes == SOCKET_ERROR || receivedBytes == 0) {//接受数据错误，把产生错误的会话socekt加入sessionsClosed队列
		this->AddClosedSession(socket);
	}
	else {
		recvBuf[receivedBytes] = '\0';
		string ipAndPort(this->GetClientAddress(socket));
		string s("\n\n接收到一个请求\n");
		s += string("ip地址: ") + ipAndPort.substr(0, ipAndPort.find(' '));
		s += string("\n端口号: ") + ipAndPort.substr(ipAndPort.find(' ') + 1)+"\n";
		s += HttpProcessor::parseHttpRequest(string(recvBuf));
		cout << s << endl;
		this->clientRequestMaps->insert(make_pair(socket,string(this->recvBuf)));
		memset(this->recvBuf, '\0', Config::BUFFERLENGTH);//清除接受缓冲区
	}
}

//向SOCKET s发送消息
void Server::sendMessage(SOCKET socket, string msg) {
	int rtn = send(socket, msg.c_str(), msg.length(), 0);
	string ipAndPort(this->GetClientAddress(socket));
	if (rtn == SOCKET_ERROR) {//发送数据错误，把产生错误的会话socekt加入sessionsClosed队列
		this->AddClosedSession(socket);
	}
	string state = msg.substr(msg.find(' '), msg.find('\r\n') - msg.find(' '));
	if (state == " 200 OK") {
		cout <<"向"<< ipAndPort.substr(0, ipAndPort.find(' ')) << "发送响应成功！" << endl;
	}
	else if (state == " 404 Not Found") {
		cout << "未找到资源！向"<< ipAndPort.substr(0, ipAndPort.find(' ')) <<"发送404页面！" << endl;
	}
}

int Server::AcceptRequestionFromClient() {
	sockaddr_in clientAddr;		//客户端IP地址
	int nAddrLen = sizeof(clientAddr);
	u_long blockMode = Config::BLOCKMODE;//将session socket设为非阻塞模式以监听客户连接请求

	//检查srvSocket是否收到用户连接请求
	if (this->sockNum > 0) {
		if (FD_ISSET(this->srvSocket, &rfds)) {  //有客户连接请求到来
			this->sockNum--;

			//产生会话socket
			SOCKET newSession = accept(this->srvSocket, (LPSOCKADDR) & (clientAddr), &nAddrLen);
			if (newSession == INVALID_SOCKET) {
				cout << "Server accept connection request error!\n";
				return -1;
			}
			//将新的会话socket设为非阻塞模式，
			if (ioctlsocket(newSession, FIONBIO, &blockMode) == SOCKET_ERROR) {
				cout << "ioctlsocket() for new session failed with error!\n";
				return -1;
			}

			//将新的session加入会话队列
			this->AddSession(newSession);
		}
	}
	return 0;
}

void Server::ReceieveMessageFromClients() {
	if (this->sockNum > 0) {
		//遍历会话列表中的所有socket，检查是否有数据到来
		for (list<SOCKET>::iterator itor = this->sessions->begin(); itor != this->sessions->end(); itor++) {
			if (FD_ISSET(*itor, &rfds)) {  //某会话socket有数据到来
				//接受数据
				this->recvMessage(*itor);
			}
		}//end for
	}
}

//向其他客户转发信息
void Server::ForwardMessage() {
	if (this->sockNum > 0) {
		if (!this->clientRequestMaps->empty()) {//如果消息队列不为空
			for (auto msgItor = this->clientRequestMaps->begin(); msgItor != this->clientRequestMaps->end(); msgItor++) {//对消息队列中的每一条消息
				this->sendMessage(msgItor->first, HttpProcessor::creatHttpResponse(msgItor->second));
			}
			printMenu();
		}
	}
		this->clientRequestMaps->clear(); //向其他客户转发消息后，清除消息队列
}

int Server::loop() {
	u_long blockMode = Config::BLOCKMODE;//将srvSock设为非阻塞模式以监听客户连接请求
	int rtn;
	if ((rtn = ioctlsocket(this->srvSocket, FIONBIO, &blockMode) == SOCKET_ERROR)) { //FIONBIO：允许或禁止套接口s的非阻塞模式。
		cout << "ioctlsocket() failed with error!\n";
		return -1;
	}
	cout << "ioctlsocket() for server socket ok!Waiting for client connection and data\n";
	
	while (true) {
		if (this->on == false)	return -1;
		this->RemoveClosedSession();
		//Prepare the read and write socket sets for network I/O notification.
		FD_ZERO(&this->rfds);
		FD_ZERO(&this->wfds);

		//把srvSocket加入到rfds，等待用户连接请求
		FD_SET(this->srvSocket, &this->rfds);

		//把当前的会话socket加入到rfds,等待用户数据的到来;加到wfds，等待socket可发送数据
		for (list<SOCKET>::iterator itor = this->sessions->begin(); itor != this->sessions->end(); itor++) {
			FD_SET(*itor, &rfds);
			FD_SET(*itor, &wfds);
		}
		struct timeval timeout = { 0, 200 };
		//等待用户连接请求或用户数据到来或会话socke可发送数据
		if ((this->sockNum = select(0, &this->rfds, &this->wfds, NULL, &timeout)) == SOCKET_ERROR) { //select函数返回有可读或可写的socket的总数，保存在rtn里.最后一个参数设定等待时间，如为NULL则为阻塞模式
			cout << "select() failed with error!\n";
			return -1;
		}
		//当程序运行到这里，意味着有用户连接请求到来，或有用户数据到来，或有会话socket可以发送数据

		//首先检查是否有客户请求连接到来
		if (this->AcceptRequestionFromClient() != 0) return -1;

		//然后尝试接受数据
		this->ReceieveMessageFromClients();

		//最后转发数据
		this->ForwardMessage();
	}
	return 0;
}