#define _CRT_SECURE_NO_WARNINGS
#include<string>
#include"Config.h"
#include "HttpProcessor.h"
#include<iostream>
#include <fstream>
#include <ctime>
#include <unordered_map>
using namespace::std;

unordered_map<string, string> HttpProcessor::types{
	{"doc","application/msword"},
	{"bin","application/octet-stream"},
	{"dll","application/octet-stream"},
	{"exe","application/octet-stream"},
	{"pdf","application/pdf"},
	{"gz","application/x-gzip"},
	{"class","application/x-java-class"},
	{"js","application/x-javascript"},
	{"zip","application/zip"},
	{"mp3","audio/mpeg"},
	{"vox","audio/voxware"},
	{"wav","audio/wav"},
	{"mp4","video/mp4"},
	{"ra","audio/x-pn-realaudio"},
	{"ram","audio/x-pn-realaudio"},
	{"bmp","image/bmp"},
	{"gif","image/gif"},
	{"jpeg","image/jpeg"},
	{"jpg","image/jpeg"},
	{"tif","image/tiff"},
	{"tiff","image/tiff"},
	{"htm","text/html"},
	{"html","text/html"},
	{"c","text/plain"},
	{"cpp","text/plain"},
	{"def","text/plain"},
	{"h","text/plain"},
	{"txt","text/plain"},
	{"rtx","text/richtext"},
	{"rtf","text/richtext"},
	{"java","text/x-java-source"},
	{"css","text/css"}
};

HttpProcessor::HttpProcessor(){}
HttpProcessor::~HttpProcessor(){}
string HttpProcessor::parseHttpRequest(string request) {
	string requestType = request.substr(0, request.find(' '));
	int pathStartIndex = request.find(' ') + 1;
	int pathEndIndex = request.find(' ', pathStartIndex);
	string path = request.substr(pathStartIndex, pathEndIndex - pathStartIndex);
	if (path == "/")	path = Config::ROOT + "/Index.html";
	else path = Config::ROOT + path;
	return string("请求类型：") + requestType + "\n" + "请求文件:" +path + "\n";
}

string HttpProcessor::creatHttpResponse(string request) {
	string httpVersion(request.substr(request.find("HTTP"),request.find("\r\n", request.find("HTTP"))- request.find("HTTP")));
	string connection;
	if (request.find("Connection") != request.npos) {
		connection = request.substr(request.find("Connection"), request.find("\r\n", request.find("Connection")) - request.find("Connection"));
		connection += "\r\n";
	}
	string date;
	string content;
	string ctype;
	string state;
	//处理内容和状态
	int pathStartIndex = request.find(' ')+1;
	int pathEndIndex = request.find(' ',pathStartIndex);
	string path = request.substr(pathStartIndex, pathEndIndex-pathStartIndex);
	if (path == "/")	path = Config::ROOT + "/Index.html";
	else path = Config::ROOT + path;
	ctype = types[path.substr(path.rfind('.')+1)];
	ifstream myfile(path, ios::in | ios::binary);
	if (!myfile.is_open())
	{
		state = "404 Not Found";
		path = Config::ROOT + "/404.html";
		ifstream t(path, ios::in | ios::binary);
		t.seekg(0, t.end);
		int length = t.tellg();
		t.seekg(0, t.beg);
		char* buf = new char[length];
		t.read(buf, length);
		//buf[length] = 0;
		content = string(buf,length);
		delete[] buf;
		t.close();
		ctype = "text/html";
	}
	else {
		state = "200 OK";
		myfile.seekg(0, myfile.end);
		int length = myfile.tellg();
		myfile.seekg(0, myfile.beg);
		char* buf = new char[length];
		myfile.read(buf, length);
		content = string(buf,length);
		delete[] buf;
		myfile.close();
	}

	//处理时间
	char szDT[128];
	tm* newtime;
	time_t ltime;
	time(&ltime);
	newtime = gmtime(&ltime);
	strftime(szDT, 128,"%a, %d %b %Y %H:%M:%S GMT", newtime);
	date =string("Date: ")+ szDT + "\r\n";
	//test
	//content = string("你好");
	string ans=httpVersion +" "+ state + "\r\n"+
		connection +
		date +
		"Server: " + Config::SERVERADDRESS + "\r\n"+
		"Content-Length: "+to_string(content.length())+"\r\n" +
		"Content-Type: "+ctype+"\r\n\r\n"
		+content+"\r\n";
	return ans;
}