#include"RSA.h"

void StringTest()
{
	RSA rsa;
	//std::cout << 1 << std::endl;
	std::string test;
	std::string rec;
	std::vector<long> res;
	std::cout << "请输入信息：";
	std::cin >> test;
	res = rsa.Ecrept(test, rsa.GetKey().ekey, rsa.GetKey().nkey);
	std::cout << "密文：";
	rsa.PrintInfo(res);
	rec = rsa.Decrept(res, rsa.GetKey().dkey, rsa.GetKey().nkey);
	std::cout << "解密：";
	std::cout << rec << std::endl;
}
void FileTest()
{
	RSA rsa;
	std::string filenameIn, fileecreptOut, filedcreptOut;
	filenameIn = "file.jpg";
	fileecreptOut += "ecrept.txt";
	filedcreptOut += "dcrept.jpg";
	rsa.FileEcrept(filenameIn.c_str(), fileecreptOut.c_str(), rsa.GetKey().ekey, rsa.GetKey().nkey);
	rsa.FileDecrept(fileecreptOut.c_str(), filedcreptOut.c_str(), rsa.GetKey().dkey, rsa.GetKey().nkey);
}

int main()
{
	StringTest();
	//FileTest();
	system("pause");
	return 0;
}