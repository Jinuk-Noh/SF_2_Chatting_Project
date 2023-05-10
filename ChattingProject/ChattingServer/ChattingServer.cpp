#include <iostream>
#include "DBHelper.h"
#include "ChattingHelper.h"


bool severClose = false;

int main()
{
	//처음 서버를 실행할 시 아래 주석을 해제해주세요(DB 테이블 구조 생성)
	/*DBHelper* dbHelper = DBHelper::CreateInstance();
	dbHelper->InitTable();*/

	//DB에서 조회한 정보를 Client에 보내기 위해 사용되는 DB 소켓 함수입니다.
	std::thread th = std::thread(RunDB);
	th.detach();

	//채팅에서 사용하는 소켓 함수입니다.
	Run();

	ReleaseDBHelper();
}

