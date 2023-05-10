#pragma once
#ifndef __DBHelper_H__
#define __DBHelper_H__

#include <iostream>
#include <mysql/jdbc.h>
#include <string>
#include <vector>

using std::cout;
using std::endl;
using std::string;

const string server = "tcp://127.0.0.1:3306"; // �����ͺ��̽� �ּ�
const string username = "root";//"chatUser"; // �����ͺ��̽� ������
const string password = "1234"; // �����ͺ��̽� ���� ���й�ȣ

class DBHelper {
private:
	static DBHelper* dbHelper;
	sql::mysql::MySQL_Driver* driver;
	sql::Connection* con;

	DBHelper() {
		sql::Statement* stmt;
		sql::PreparedStatement* pstmt;

		try {
			driver = sql::mysql::get_mysql_driver_instance();
			con = driver->connect(server, username, password);
		}
		catch (sql::SQLException& e) {
			cout << "Could not connect to server. Error message: " << e.what() << endl;
			exit(1);
		}

		// �����ͺ��̽� ����
		con->setSchema("ChattingDB");

		// db �ѱ� ������ ���� ���� 
		stmt = con->createStatement();
		stmt->execute("set names euckr");
		if (stmt) { delete stmt; stmt = nullptr; }
	}

public:
	static DBHelper* CreateInstance() {
		if (dbHelper == nullptr) {
			dbHelper = new DBHelper;
		}

		return dbHelper;
	}

	//ó�� DB�� ���̺��� ������ �� ����
	void InitTable() {

		string query = "";

		sql::Statement* stmt;
		stmt = con->createStatement();

		//cout << "Creating table..." << endl;

		//User Table ����
		query = "CREATE TABLE IF NOT EXISTS User ( \
			 id VARCHAR(20) PRIMARY KEY \
			,pw VARCHAR(50) NOT NULL \
			,name VARCHAR(10) NOT NULL \
			);";
		stmt->execute(query);

		// Chatting Table ����
		query = "CREATE TABLE IF NOT EXISTS Chatting_Log("
			"id VARCHAR(20) NOT NULL"
			", content VARCHAR(400) NOT NULL"
			", date DATETIME NOT NULL"
			", FOREIGN KEY (id) REFERENCES user (id)"
			" ON UPDATE CASCADE"
			" ON DELETE CASCADE"
			");";

		stmt->execute(query);

		//cout << "Finished creating table" << endl;

		delete stmt;
	}

	sql::ResultSet* SelectQuerySTMT(string query) {
		sql::Statement* stmt = con->createStatement();

		sql::SQLString* sqlStr = new sql::SQLString(query);

		sql::ResultSet* result = stmt->executeQuery(*sqlStr);

		delete sqlStr;

		return result;
	}

	// PreparedStatement�� ? ���� �־������ؼ� ���ڷ� ���޹���
	sql::ResultSet* SelectQueryPSTMT(sql::PreparedStatement* pstmt) {
		sql::ResultSet* result = pstmt->executeQuery();

		return result;
	}

	// SelectQueryPSTMT���ڷ� �����ϱ� ���� ����
	sql::PreparedStatement* CreatePreoaredStatement(string query) {
		sql::PreparedStatement* pstmt = con->prepareStatement(query);

		return pstmt;
	}
};

DBHelper* DBHelper::dbHelper = nullptr;

//Server ���� �� DBHelper �ڿ�����
void ReleaseDBHelper() {
	DBHelper* dbHelper = DBHelper::CreateInstance();
	if (dbHelper != nullptr) {
		delete dbHelper;
		dbHelper = nullptr;
	}
}

#pragma region SELECT
string UserDupleCheck(DBHelper* dbHelper, const char* id) {

	string query = "SELECT COUNT(*) FROM User WHERE id = ?";
	sql::PreparedStatement* pstmt = dbHelper->CreatePreoaredStatement(query);
	pstmt->setString(1, id);
	sql::ResultSet* result = pstmt->executeQuery();

	int cnt = 0;
	while (result->next()) {
		cnt = result->getInt(1);
	}

	delete pstmt;

	if (cnt > 0) {
		return "true";
	}
	else {
		return "false";
	}
}


string GetUserInfo(DBHelper* dbHelper, const char* id, const char* pw) {
	string query = "SELECT id, name FROM User WHERE id =? AND pw =?";
	sql::PreparedStatement* pstmt = dbHelper->CreatePreoaredStatement(query);
	pstmt->setString(1, id);
	pstmt->setString(2, pw);
	sql::ResultSet* result = pstmt->executeQuery();


	string resultStr = "";
	while (result->next()) {
		resultStr = result->getString(1) + "|" + result->getString(2);
	}


	return resultStr;
}

std::vector<string> GetChattingLog(DBHelper* dbHelper) {
	std::vector<string> v;
	string query = "SELECT content FROM "
		" ( "
		" SELECT concat(u.name, \"(\", l.id, \") : \", l.content, \" - [\", l.date, \"]\") AS content, l.date "
		" FROM chatting_log l "
		" LEFT JOIN user u "
		" ON l.id = u.id "
		" ORDER BY l.date DESC "
		" LIMIT 10 "
		" ) A "
		" ORDER BY date ASC;"
		;

	sql::ResultSet* result =  dbHelper->SelectQuerySTMT(query);

	v.push_back("--------------------이전 채팅---------------------");

	while (result->next()) {
		v.push_back(result->getString(1));
	}
	
	v.push_back("");

	return v;
}

#pragma endregion

#pragma region UPDATE

#pragma endregion

#pragma region DELETE

#pragma endregion

#pragma region INSERT

void UploadSignUp(DBHelper* dbHelper, const char* id, const char* pw, const char* name) {

	string query = "INSERT INTO User (id, pw, name) VALUES(?,?,?)";
	sql::PreparedStatement* pstmt = dbHelper->CreatePreoaredStatement(query);
	pstmt->setString(1, id);
	pstmt->setString(2, pw);
	pstmt->setString(3, name);
	sql::ResultSet* result = pstmt->executeQuery();

	delete pstmt;
}

void InsertChatLog(DBHelper* dbHelper, const char* id, const char* content, const char* date) {
	string query = "INSERT INTO Chatting_Log VALUE (?, ?, ?)";
	sql::PreparedStatement* pstmt = dbHelper->CreatePreoaredStatement(query);
	pstmt->setString(1, id);
	pstmt->setString(2, content);
	pstmt->setString(3, date);

	pstmt->execute();

	delete pstmt;
}

#pragma endregion

#endif // !__