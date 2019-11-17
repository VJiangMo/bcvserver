#include <iostream>
#include <memory>
#include "http_server.h"
#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/driver.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/metadata.h>
#include <cppconn/exception.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/document.h>

using namespace rapidjson;

// 初始化HttpServer静态类成员
mg_serve_http_opts HttpServer::s_server_option;
std::string HttpServer::s_web_dir = "./web";
std::unordered_map<std::string, ReqHandler> HttpServer::s_handler_map;

bool handle_bcv_api(std::string url, std::string body, mg_connection *c, OnRspCallback rsp_callback)
{
	const char* user = "basta";
	const char* passwd = "yd0927HK#";
	const char* host = "tcp://123.56.8.254:3306";
	const char* database = "BusDB";
	try {
		sql::mysql::MySQL_Driver* driver = sql::mysql::get_mysql_driver_instance();
		sql::Connection* conn = driver->connect(host, user, passwd);
		conn->setSchema(database);
		sql::Statement *stmt = conn->createStatement();

		sql::ResultSet * res = stmt->executeQuery("CALL selectLastHourInfo()");
		std::list<std::string> phoneList;
		while (res->next()) {
			phoneList.push_back(res->getString("phone").c_str());
			printf("phone:%s,pwd:%s,timestamp:%s\r\n", res->getString("phone").c_str(), \
				res->getString("pwd").c_str(), res->getString("timeseconds").c_str());
		}
		delete res;
		delete stmt;
		delete conn;
		if (phoneList.empty()){
			rsp_callback(c, "{\"result\":\"nodata\"}");
			return false;
		}
		Document jsonDoc;
		Document::AllocatorType &allocator = jsonDoc.GetAllocator();
		jsonDoc.SetObject();

		//添加数组型数据
		Value phoneArray(kArrayType);
		for (auto _ite=phoneList.begin();_ite!=phoneList.end();++_ite)
		{
			Value _object(kObjectType);
			_object.SetString(_ite->c_str(), allocator);
			phoneArray.PushBack(_object, allocator);
		}
		jsonDoc.AddMember("result","ok",allocator);
		jsonDoc.AddMember("data", phoneArray, allocator);

		//生成字符串
		StringBuffer buffer;
		Writer<StringBuffer> writer(buffer);
		jsonDoc.Accept(writer);
		std::string strJson = buffer.GetString();

		rsp_callback(c, strJson);
		return true;
	}catch (sql::SQLException& e) {
		printf("#Error:%s\r\n", e.what());
		rsp_callback(c, "{\"result\":\"error\"}");
		return false;
	}
}

bool handle_update_api(std::string url, std::string body, mg_connection *c, OnRspCallback rsp_callback)
{
	//const char* user = "basta";
	//const char* passwd = "yd0927HK#";
	//const char* host = "tcp://123.56.8.254:3306";
	//const char* database = "BusDB";
	//try {
	//	sql::mysql::MySQL_Driver* driver = sql::mysql::get_mysql_driver_instance();
	//	sql::Connection* conn = driver->connect(host, user, passwd);
	//	conn->setSchema(database);
	//	sql::Statement *stmt = conn->createStatement();

	//	sql::ResultSet * res = stmt->executeQuery("CALL updateDo()");
	//	std::list<std::string> phoneList;
	//	while (res->next()) {
	//		phoneList.push_back(res->getString("phone").c_str());
	//		printf("phone:%s,pwd:%s,timestamp:%s\r\n", res->getString("phone").c_str(), \
	//			res->getString("pwd").c_str(), res->getString("timeseconds").c_str());
	//	}
	//	delete res;
	//	delete stmt;
	//	delete conn;
	//	if (phoneList.empty()) {
	//		rsp_callback(c, "{\"result\":\"nodata\"}");
	//		return false;
	//	}
	//	Document jsonDoc;
	//	Document::AllocatorType &allocator = jsonDoc.GetAllocator();
	//	jsonDoc.SetObject();

	//	jsonDoc.AddMember("result", "ok", allocator);

	//	//生成字符串
	//	StringBuffer buffer;
	//	Writer<StringBuffer> writer(buffer);
	//	jsonDoc.Accept(writer);
	//	std::string strJson = buffer.GetString();

	//	rsp_callback(c, strJson);
	//	return true;
	//}
	//catch (sql::SQLException& e) {
	//	printf("#Error:%s\r\n", e.what());
	//	rsp_callback(c, "{\"result\":\"error\"}");
	//	return false;
	//}
	return true;
}

int main(int argc, char *argv[]) 
{
	std::string port = "9090";
	auto http_server = std::shared_ptr<HttpServer>(new HttpServer);
	http_server->Init(port);
	// add handler
	http_server->AddHandler("/api/bcv", handle_bcv_api);
	http_server->AddHandler("/api/update", handle_update_api);
	http_server->Start();
	return 0;
}