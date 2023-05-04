int CheckIdInfo();
void UploadSignUpData();
int main() {


	string id, pw, pw_1, name;

	while (1) {

		cout << "회원가입" << endl << "사용할 ID를 입력해주십시오: ";
		cin >> id;
		cout << endl;

		WSADATA wsa;

		// Winsock를 초기화하는 함수. MAKEWORD(2, 2)는 Winsock의 2.2 버전을 사용하겠다는 의미.
		// 실행에 성공하면 0을, 실패하면 그 이외의 값을 반환.
		// 0을 반환했다는 것은 Winsock을 사용할 준비가 되었다는 의미.
		int code = WSAStartup(MAKEWORD(2, 2), &wsa);

		if (!code) {
			client_sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP); // 

			SOCKADDR_IN client_addr = {};
			client_addr.sin_family = AF_INET;
			client_addr.sin_port = htons(7720);
			InetPton(AF_INET, TEXT("127.0.0.1"), &client_addr.sin_addr);

			while (1) {
				if (!connect(client_sock, (SOCKADDR*)&client_addr, sizeof(client_addr))) {
					cout << "Server Connect" << endl;
					send(client_sock, ("duple|" + id).c_str(), ("duple|" + id).length(), 0);
					break;
				}
				cout << "Connecting..." << endl;
			}
			int check = CheckIdInfo();
			closesocket(client_sock);
			WSACleanup();

			if (check < 0) {
				cout << "서버에 오류가 발생하였습니다. 재시도 해주십시오." << endl;
			}
			else {

				if (check == 0) {
					while (1) {
						cout << "사용할 비밀번호를 입력해주십시오: ";
						cin >> pw;
						cout << endl << "비밀번호를 확인해주십시오: ";
						cin >> pw_1;

						if (pw != pw_1) {
							cout << endl << "입력한 비밀번호가 서로 다릅니다! \n다시 입력해주십시오." << endl << endl;
							continue;
						}
						else {
							cout << endl << "사용할 닉네임을 입력해 주십시오: ";
							cin >> name;

							string uploadSignUp = "upload|" + id + "|" + pw + "|" + name;

							int code = WSAStartup(MAKEWORD(2, 2), &wsa);

							if (!code) {
								client_sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP); // 

								SOCKADDR_IN client_addr = {};
								client_addr.sin_family = AF_INET;
								client_addr.sin_port = htons(7720);
								InetPton(AF_INET, TEXT("127.0.0.1"), &client_addr.sin_addr);

								while (1) {
									if (!connect(client_sock, (SOCKADDR*)&client_addr, sizeof(client_addr))) {
										cout << "Server Connect" << endl;
										send(client_sock, uploadSignUp.c_str(), uploadSignUp.length(), 0);
										break;
									}
									cout << "Connecting..." << endl;
								}
								UploadSignUpData();
								closesocket(client_sock);
								WSACleanup();

								break;
							}
						}
						break;
					}
				}
				else {
					cout << "이미 존재하는 아이디입니다. 다른 아이디를 입력해주십시오." << endl;
					continue;
				}
			}
		}
	}
}

int CheckIdInfo() {
	char buf[MAX_SIZE] = {};
	string msg;
	if (recv(client_sock, buf, MAX_SIZE, 0) > 0) {
		msg = buf;
		std::stringstream check(msg);
		return msg == "true" ? 1 : 0;
	}
	else {
		return -1;
	}
}

void UploadSignUpData() {
	char buf[MAX_SIZE] = {};
	string msg;
	recv(client_sock, buf, MAX_SIZE, 0);
	msg = buf;
	cout << msg;

}