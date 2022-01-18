#pragma once
#pragma once
#include <winsock2.h>
#include <list>
#include <string>
#include <map>

using namespace std;

//������

class Server {
private:
	SOCKET srvSocket;			//����������socket
	bool on;
	char* recvBuf;				//���ܻ�����
	fd_set rfds;				//���ڼ��socket�Ƿ������ݵ����ĵ��ļ�������
	fd_set wfds;				//���ڼ��socket�Ƿ���Է��͵��ļ�������
	sockaddr_in srvAddr;		//��������IP��ַ
	int sockNum;				//ͬʱ��������Ŀͻ�������
	list<SOCKET>* sessions;							//��ǰ�ĻỰsocket����
	list<SOCKET>* closedSessions;					//������ʧЧ�ĻỰsocket����
	map<SOCKET, string>* clientRequestMaps;				//�ͻ��˵�ַ�б���ַ��(key,value)����ʽ���棬keyΪSOCKET���ͣ�valueΪstring����
protected:
	virtual string  GetClientAddress(SOCKET s);
	virtual void AddSession(SOCKET session);								//���µĻỰsocket�������
	virtual void AddClosedSession(SOCKET session);							//��ʧЧ�ĻỰsocket�������
	virtual void RemoveClosedSession(SOCKET closedSession);					//��ʧЧ��SOCKET�ӻỰsocket����ɾ��
	virtual void RemoveClosedSession();										//��ʧЧ��SOCKET�ӻỰsocket����ɾ��
	virtual int AcceptRequestionFromClient();								//�ȴ��ͻ�����������
	virtual void recvMessage(SOCKET s);										//��SOCKET s������Ϣ
	virtual void  ReceieveMessageFromClients();								//���ܿͻ��˷�������Ϣ
	virtual void sendMessage(SOCKET s, string msg);							//��SOCKET s������Ϣ
	virtual void ForwardMessage();										//�������ͻ�ת����Ϣ
public:
	Server(void);
	virtual ~Server(void);
	virtual void startUp();
	virtual void turnDown();
	virtual int winsockStartup();		//��ʼ��Winsock
	virtual int serverStartup();		//��ʼ��Server����������SOCKET���󶨵�IP��PORT
	virtual int listenStartup();		//��ʼ�����ͻ�������
	virtual int loop();					//ѭ��ִ��"�ȴ��ͻ�������"->���������ͻ�ת����Ϣ��->"�ȴ��ͻ�����Ϣ"
};
