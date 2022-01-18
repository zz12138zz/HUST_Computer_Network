#pragma once
#include <string>
#include <unordered_map>
using namespace::std;
class HttpProcessor {
private:
	HttpProcessor();
	~HttpProcessor();
	static unordered_map<string, string> types;
public:
	static string parseHttpRequest(string request);
	static string creatHttpResponse(string request);
};
