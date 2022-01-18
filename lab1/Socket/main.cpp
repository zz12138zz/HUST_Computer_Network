#include "Server.h"
#include "HttpProcessor.h"
#include <iostream>
#include <thread>
#include "PrintMenu.h"
#include "Config.h"
int runServer(Server *srv) {
		if (srv->winsockStartup() == -1) return 1;
		if (srv->serverStartup() == -1) return 1;
		if (srv->listenStartup() == -1) return 1;
		if (srv->loop() == -1) {
			cout << "�������ѹر�" << endl;
			return 1;
		}
		return 0;
}
int shutDownAppWhenRunningServer(thread &t,Server *&srv) {
	cout << "���������������У��Ƿ�رճ���" << endl;
	cout << "1.ȷ��\n2.ȡ��"<<endl;
	int id;
	while (1) {
		cin >> id;
		switch (id)
		{
			case 1:
				srv->turnDown();
				t.join();
				delete srv;
				srv = nullptr;
				return 1;
			case 2:
				return 0;
			default:
				cout << "��������ȷ�����" << endl;
				break;
		}
	}
}

int main() {
	Server *srv;
	Config::loadConfig();
	while (1) {
		//������δ����ǰ
		printMenu();
		int id;
		cin >> id;
		if (id != 1) {
			if (id == 2) {
				cout << "����������������" << endl;
				continue;
			}
			if (id == 3) {
				printConfig();
				continue;
			}
			if (id == 4) {
				system(CONFIG_PATH);
				Config::loadConfig();
				continue;
			}
			if (id == 5) {
				cout << "��лʹ�ã��ټ�" << endl;
				system("pause");
				return 0;
			}
			else cout << "��������ȷ����ţ�" << endl;
			continue;
		}
		//����������
		srv = new Server();
		thread t(runServer, ref(srv));
		std::this_thread::sleep_for(std::chrono::milliseconds(200)); 
		cout << "�����������ɹ�" << endl;
		while (srv) {
			printMenu();
			cin >> id;
			switch (id){
				case 1:
					cout << "��������������" << endl;
					break;
				case 2:
					srv->turnDown();
					//�ȴ����߳̽�����ɾ������
					t.join();
					delete srv;
					srv = nullptr;
					break;
				case 3:
					printConfig();
					break;
				case 4:
					cout << "�������������У��޷��޸����ã����ȹرշ�������" << endl;
					break;
				case 5:
					if (shutDownAppWhenRunningServer(t,srv)) {
						cout << "��лʹ�ã��ټ�" << endl;
						system("pause");
						return 0;
					}
					break;
				default:
					cout << "��������ȷ����ţ�" << endl;
					break;
			}
		}
	}
	return 0;
}