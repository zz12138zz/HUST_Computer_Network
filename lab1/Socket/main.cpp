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
			cout << "服务器已关闭" << endl;
			return 1;
		}
		return 0;
}
int shutDownAppWhenRunningServer(thread &t,Server *&srv) {
	cout << "服务器正在运行中，是否关闭程序？" << endl;
	cout << "1.确定\n2.取消"<<endl;
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
				cout << "请输入正确的序号" << endl;
				break;
		}
	}
}

int main() {
	Server *srv;
	Config::loadConfig();
	while (1) {
		//服务器未启动前
		printMenu();
		int id;
		cin >> id;
		if (id != 1) {
			if (id == 2) {
				cout << "请先启动服务器！" << endl;
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
				cout << "感谢使用！再见" << endl;
				system("pause");
				return 0;
			}
			else cout << "请输入正确的序号！" << endl;
			continue;
		}
		//服务器启动
		srv = new Server();
		thread t(runServer, ref(srv));
		std::this_thread::sleep_for(std::chrono::milliseconds(200)); 
		cout << "服务器启动成功" << endl;
		while (srv) {
			printMenu();
			cin >> id;
			switch (id){
				case 1:
					cout << "服务器已启动！" << endl;
					break;
				case 2:
					srv->turnDown();
					//等待子线程结束在删除对象
					t.join();
					delete srv;
					srv = nullptr;
					break;
				case 3:
					printConfig();
					break;
				case 4:
					cout << "服务器正在运行，无法修改配置，请先关闭服务器！" << endl;
					break;
				case 5:
					if (shutDownAppWhenRunningServer(t,srv)) {
						cout << "感谢使用！再见" << endl;
						system("pause");
						return 0;
					}
					break;
				default:
					cout << "请输入正确的序号！" << endl;
					break;
			}
		}
	}
	return 0;
}