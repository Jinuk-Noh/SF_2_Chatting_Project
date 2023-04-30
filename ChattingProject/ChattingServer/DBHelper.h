#pragma once
#ifndef __DBHelper_H__
#define __DBHelper_H__

#include <iostream>
#include <mysql/jdbc.h>
#include <string>

using namespace std;

const string server = "tcp://127.0.0.1:3306"; // �����ͺ��̽� �ּ�
const string username = "root";//"chatUser"; // �����ͺ��̽� �����
const string password = "1234"; // �����ͺ��̽� ���� ��й�ȣ



class DBHelper {
private:
	static DBHelper* dbHelper;
	sql::mysql::MySQL_Driver* driver;
	sql::Connection* con;

	DBHelper() {
		sql::Statement* stmt;
		sql::PreparedStatement* pstmt;
		cout << "DB ���� ��";
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
	static DBHelper*  CreateInstance() {
		if (dbHelper == nullptr) {
			dbHelper = new DBHelper;
		}

		return dbHelper;
	}

	//ó�� DB�� ���̺��� ������ �� ���
	void InitTable() {
		
		

		sql::Statement* stmt;
		stmt = con->createStatement();
		stmt->execute("CREATE TABLE User (id varchar(20) primary key, pw varchar(10) not null, name varchar(10) not null);");
		stmt->execute("CREATE TABLE Chatting_Log(id varchar(20) foreign key not null, content varchar(400) not null, date datetime not null); "); // CREATE
		cout << "Finished creating table" << endl;
		delete stmt;


	}


};

DBHelper* DBHelper::dbHelper = nullptr;

#endif // !__
