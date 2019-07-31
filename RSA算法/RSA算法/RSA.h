#pragma once
#include<iostream>
#include <string>
#include <vector>
#include<time.h>
#include<Windows.h>
#include<fstream>

struct Key
{
	//公钥(ekey, pkey): (e,n)
	long nkey;
	long ekey;
	//私钥(dkey, pkey): (d, n)
	long dkey;
};

class RSA
{
public:
	RSA();
	Key GetKey();
	void FileEcrept(const char* plain_file_in, const char* ecrept_file_out,
		long ekey, long pkey);
	void FileDecrept(const char* ecrept_file_in, const char* plain_file_out,
		long dkey, long pkey);

	std::vector<long> Ecrept(std::string& str_in, long ekey, long pkey);
	std::string Decrept(std::vector<long>& ecrept_str, long dkey, long pkey);

	void PrintInfo(std::vector<long>& ecrept_str);
private:
	//加密与解密单个信息
	long ecrept(long msg, long key, long pkey);
	//产生素数
	long ProducePrime();
	//产生两把钥匙
	void ProduceKeys();
	//产生n
	long Producenkey(long prime1, long prime2);
	//欧拉公式的运用
	long ProduceOrla(long prime1, long prime2);
	//求最大公约数
	long ProduceGcd(long ekey, long orla);
	//产生公钥
	long ProduceEkey(long orla);

	//产生私钥
	long producedkey(long ekey, long orla);

private:
	Key key_;
};