int CheckIdInfo();
void UploadSignUpData();
int main() {


	string id, pw, pw_1, name;

	while (1) {

		cout << "ȸ������" << endl << "����� ID�� �Է����ֽʽÿ�: ";
		cin >> id;
		cout << endl;

		WSADATA wsa;

		// Winsock�� �ʱ�ȭ�ϴ� �Լ�. MAKEWORD(2, 2)�� Winsock�� 2.2 ������ ����ϰڴٴ� �ǹ�.
		// ���࿡ �����ϸ� 0��, �����ϸ� �� �̿��� ���� ��ȯ.
		// 0�� ��ȯ�ߴٴ� ���� Winsock�� ����� �غ� �Ǿ��ٴ� �ǹ�.
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
				cout << "������ ������ �߻��Ͽ����ϴ�. ��õ� ���ֽʽÿ�." << endl;
			}
			else {

				if (check == 0) {
					while (1) {
						cout << "����� ��й�ȣ�� �Է����ֽʽÿ�: ";
						cin >> pw;
						cout << endl << "��й�ȣ�� Ȯ�����ֽʽÿ�: ";
						cin >> pw_1;

						if (pw != pw_1) {
							cout << endl << "�Է��� ��й�ȣ�� ���� �ٸ��ϴ�! \n�ٽ� �Է����ֽʽÿ�." << endl << endl;
							continue;
						}
						else {
							cout << endl << "����� �г����� �Է��� �ֽʽÿ�: ";
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
					cout << "�̹� �����ϴ� ���̵��Դϴ�. �ٸ� ���̵� �Է����ֽʽÿ�." << endl;
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