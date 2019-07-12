#include"RSA.h"

//void PrimeTest()
//{
//	RSA rsa;
//	std::cout << rsa.ProducePrime() << std::endl;
//}
void KeyTest()
{
	RSA rsa;
}
void StringTest()
{
	RSA rsa;
	//std::cout << 1 << std::endl;
	std::string test;
	std::string rec;
	std::vector<bm::int1024_t> res;
	std::cout << "请输入信息：";
	std::cin >> test;
	res = rsa.Ecrept(test, rsa.GetKey().ekey, rsa.GetKey().nkey);
	std::cout << "密文：";
	rsa.PrintInfo(res);
	rec = rsa.Decrept(res, rsa.GetKey().dkey, rsa.GetKey().nkey);
	std::cout << "解密：";
	std::cout << rec << std::endl;
}


//大数的测试
void BigNumTest()
{
	const char* bigno = "1234567899879656132134849651186483758138177753648216481276371639";

	bm::cpp_int s(bigno);

	std::cout << s << std::endl;

	std::cout << s / 2 << std::endl;
}

void BigRandTest()
{
	br::mt19937 ran(time(nullptr));
	br::uniform_int_distribution<bm::int1024_t> dist(2, bm::int1024_t(1) << 768);

	std::cout << dist(ran) << std::endl;
}

void FileTest()
{
	RSA rsa;
	//std::cout << "请输入文件名" << std::endl;
	std::string filenameIn, fileecreptOut, filedcreptOut;
	//std::cin >> filenameIn;
	filenameIn = "file.jpg";
	fileecreptOut += "ecrept.txt";
	filedcreptOut += "dcrept.jpg";
	rsa.FileEcrept(filenameIn.c_str(), fileecreptOut.c_str(), rsa.GetKey().ekey, rsa.GetKey().nkey);
	rsa.FileDecrept(fileecreptOut.c_str(), filedcreptOut.c_str(), rsa.GetKey().dkey, rsa.GetKey().nkey);
}

void Menu()
{
	
}
int main()
{
	//PrimeTest();
	//KeyTest();
	//StringTest();
	//BigNumTest();
	//BigRandTest();


	FileTest();
	system("pause");
	return 0;
}