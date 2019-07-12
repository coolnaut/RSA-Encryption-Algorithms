#pragma once
#include<iostream>
#include <string>
#include <vector>
#include<time.h>
#include<Windows.h>

//boost库的大数
#include <boost/multiprecision/cpp_int.hpp>
#include <boost/multiprecision/random.hpp>
#include <boost/multiprecision/miller_rabin.hpp>//素数

#include<fstream>
namespace bm = boost::multiprecision;
namespace br = boost::random;
struct Key
{
	//公钥(ekey, pkey): (e,n)
	bm::int1024_t nkey;
	bm::int1024_t ekey;
	//私钥(dkey, pkey): (d, n)
	bm::int1024_t dkey;
};

class RSA
{
public:
	RSA();
	Key GetKey();
	void FileEcrept(const char* plain_file_in, const char* ecrept_file_out,
		bm::int1024_t ekey, bm::int1024_t pkey);
	void FileDecrept(const char* ecrept_file_in, const char* plain_file_out,
		bm::int1024_t dkey, bm::int1024_t pkey);

	std::vector<bm::int1024_t> Ecrept(std::string& str_in, bm::int1024_t ekey, bm::int1024_t pkey);
	std::string Decrept(std::vector<bm::int1024_t>& ecrept_str, bm::int1024_t dkey, bm::int1024_t pkey);

	void PrintInfo(std::vector<bm::int1024_t>& ecrept_str);
private:
	//加密与解密单个信息
	bm::int1024_t ecrept(bm::int1024_t msg, bm::int1024_t key, bm::int1024_t pkey);
	bm::int1024_t ProducePrime();
	bool is_prime(bm::int1024_t prime);
	void ProduceKeys();
	bm::int1024_t Producenkey(bm::int1024_t prime1, bm::int1024_t prime2);
	bm::int1024_t ProduceOrla(bm::int1024_t prime1, bm::int1024_t prime2);
	bm::int1024_t ProduceEkey(bm::int1024_t orla);
	bm::int1024_t ProduceGcd(bm::int1024_t ekey, bm::int1024_t orla);
	bm::int1024_t ProduceGcd(bm::int1024_t ekey, bm::int1024_t orla,bm::int1024_t& x, bm::int1024_t& y);
	bm::int1024_t producedkey(bm::int1024_t ekey, bm::int1024_t orla);

private:
	Key key_;
};