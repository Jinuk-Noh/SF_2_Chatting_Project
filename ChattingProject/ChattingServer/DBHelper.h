#pragma once
#ifndef __DBHelper_H__
#define __DBHelper_H__

#include <iostream>
#include <mysql/jdbc.h>
#include <string>

using std::cout;
using std::endl;
using std::string;

const string server = "tcp://127.0.0.1:3306"; // 데이터베이스 주소
const string username = "root";//"chatUser"; // 데이터베이스 사용자
const string password = "1234"; // 데이터베이스 접속 비밀번호

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

		// 데이터베이스 선택
		con->setSchema("ChattingDB");

		// db 한글 저장을 위한 셋팅 
		stmt = con->createStatement();
		stmt->execute("set names euckr");
		if (stmt) { delete stmt; stmt = nullptr; }
	}
	
public:
	static DBHelper*  CreateInstance() {
		if (dbHelper == nullptr) {
			dbHelper = new DBHelper;
		}

		return dbHelper;
	}

	//처음 DB에 테이블을 생성할 때 사용
	void InitTable() {
		
		string query = "";

		sql::Statement* stmt;
		stmt = con->createStatement();

		//cout << "Creating table..." << endl;

		//User Table 생성
		query = "CREATE TABLE IF NOT EXISTS User ( \
			 id VARCHAR(20) PRIMARY KEY \
			,pw VARCHAR(10) NOT NULL \
			,name VARCHAR(10) NOT NULL \
			);";
		stmt->execute(query);

		// Chatting Table 생성
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

	// PreparedStatement는 ? 값을 넣어줘야해서 인자로 전달받음
	sql::ResultSet* SelectQueryPSTMT(sql::PreparedStatement* pstmt) {
		sql::ResultSet* result = pstmt->executeQuery();

		return result;
	}

	// SelectQueryPSTMT인자로 전달하기 위해 사용
	sql::PreparedStatement* CreatePreoaredStatement(string query) {
		sql::PreparedStatement* pstmt = con->prepareStatement(query);
		
		return pstmt;
	}
};

DBHelper* DBHelper::dbHelper = nullptr;

//Server 종료 시 DBHelper 자원해제
void ReleaseDBHelper() {
	DBHelper* dbHelper = DBHelper::CreateInstance();

	if (dbHelper != nullptr) {
		
		delete dbHelper;		
		dbHelper = nullptr;
	}
}

#pragma region SELECT
bool UserDupleCheck(DBHelper* dbHelper, const char* id) {

	string query = "SELECT COUNT(*) FROM User WHERE id = ?";
	sql::PreparedStatement* pstmt = dbHelper->CreatePreoaredStatement(query);
	pstmt->setString(1, id);
	sql::ResultSet* result = pstmt->executeQuery();

	int cnt = 0;
	while (result->next()) {
		cnt = result->getInt(1);
	}

	delete pstmt;

	return (cnt > 0);
}
#pragma endregion

#pragma region UPDATE

#pragma endregion

#pragma region DELETE

#pragma endregion



#endif // !__
