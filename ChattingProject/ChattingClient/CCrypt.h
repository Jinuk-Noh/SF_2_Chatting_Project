#pragma once
#ifndef __CCRYPT_H__
#define __CCRYPT_H__
#include <Windows.h>
const INT C1 = 52845;
const INT C2 = 22719;
const INT KEY = 78695;

class CCrypt
{
private:


public:
	CCrypt(void) {};
	~CCrypt(void) {};
	

	static vector<int> Encrypt(char* source, char* destination, DWORD length) {
		DWORD i;
		int key = KEY;
		vector<int> temp;
		if (!source || !destination || length <= 0)
		{
			return temp;
		}
		for (i = 0; i < length; i++) {
			destination[i] = source[i] ^ key >> 8;
			key = (destination[i] + key) * C1 + C2;

			temp.push_back(destination[i]);
		}
		return temp;
	};
};

std::string PwEncrypt(std::string pw) {
	char origin[15];
	strcpy_s(origin, 15, pw.c_str());

	char encrypt[15];
	ZeroMemory(encrypt, sizeof(encrypt));
	string str_array;
	vector<int> v = CCrypt::Encrypt(origin, encrypt, sizeof(origin));
	for (int i : v) {
		str_array += to_string(i);
	}
	//string str_array(reinterpret_cast<char const*>(encrypt));
	//cout << str_array;
	if (strlen(str_array.c_str()) > 50) {
		str_array = str_array.substr(0, 50);
	}

	return str_array;
}
#endif // !__CCRYPT_H__