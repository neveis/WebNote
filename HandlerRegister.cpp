#include <fstream>
#include <cstdio>
#include "HandlerRegister.h"
#include "HandlerConfigure.h"
#include <boost/filesystem.hpp>

using namespace std;
using namespace MyWeb;

HandlerRegister::HandlerRegister(MyWeb::Server<MyWeb::HTTP> & server)
{
	REGISTER_HANDLERS(server);

	server.resource["^/string/?$"]["POST"] = [](Response& response, Request& request) {
		stringstream ss;
		*request.content >> ss.rdbuf();
		string content = ss.str();
		response.body << content;
	};

	server.resource["^/match/([0-9a-zA-Z]+)/?$"]["GET"] = [](Response& response, Request& request) {
		string number = request.path_match[1];
		response.body << number;
	};

	server.resource["^/article/?$"]["GET"] = [](Response& response, Request& request) {
		string content;
		content = request.query["articleId"];
		response.body << content;
	};

	server.default_resource["^/?(.*)$"]["GET"] = [](Response& response, Request& request) {
		//string filename = "www/";

		string path = request.path_match[1];

		// 防止使用 `..` 来访问 web/ 目录外的内容
		size_t last_pos = path.rfind(".");
		size_t current_pos = 0;
		size_t pos;
		while ((pos = path.find('.', current_pos)) != string::npos && pos != last_pos) {
			current_pos = pos;
			path.erase(pos, 1);
			last_pos--;
		}

		boost::filesystem::path filename("www");
		filename.append(path);
		//filename += path;

		if (boost::filesystem::is_directory(filename)) {
			filename.append("index.html");
			//filename = pathtemp.string();
		}

		ifstream ifs;
		/*
		// 简单的平台无关的文件或目录检查
		if (filename.find('.') == string::npos) {
			if (filename[filename.length() - 1] != '/')
				filename += '/';
			filename += "index.html";
		}
		*/
		ifs.open(filename.string(), ifstream::in);

		if (ifs) {
			ifs.seekg(0, ios::end);
			size_t length = ifs.tellg();

			ifs.seekg(0, ios::beg);

			// 文件内容拷贝到 response-stream 中，不应该用于大型文件
			response.body << ifs.rdbuf();

			ifs.close();
		}
		else {
			// 文件不存在时，返回无法打开文件
			string content = "Could not open file " + filename.string();
			response.status = 400;
			response.body << content;
		}
	};
}

HandlerRegister::~HandlerRegister()
{
}
