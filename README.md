# RSA-Encryption-Algorithms
# ——基于boost库实现RSA算法加密库

### 简介

本项目是实现RSA加密算法，将以.hpp的形式，提供文档加密/解密，消息加密/解密。

### 特点

由于是.hpp形式，使用简单，只用include就可以使用

完全开源，方便优化

接口简单，易使用

使用了boost大数运算，几乎无法被解密

### 用法

项目是在VS2017开发，由于项目使用了boost库，所以需要引入boost库

![Image text](https://github.com/Be-doing/RSA-Encryption-Algorithms/blob/master/Screenshot/Step1.png)

![Image text](https://github.com/Be-doing/RSA-Encryption-Algorithms/blob/master/Screenshot/Step2.png)

直接clone《RSA加密算法》，即可得到.hpp，在项目中包含本.hpp，在本库中clone与当前项目匹配的boost1_58版本，引入boost库后就可以使用。

### Demo

下面是测试代码。有兴趣可以跑跑。

```C++
void StringTest()
{
	RSA rsa;
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
void FileTest()
{
	RSA rsa;
	std::string filenameIn, fileecreptOut, filedcreptOut;
	filenameIn = "file.jpg";//记得要有一个文件名为file.jpg的文件。
	fileecreptOut += "ecrept.txt";
	filedcreptOut += "dcrept.jpg";
	rsa.FileEcrept(filenameIn.c_str(), fileecreptOut.c_str(), rsa.GetKey().ekey, rsa.GetKey().nkey);
	rsa.FileDecrept(fileecreptOut.c_str(), filedcreptOut.c_str(), rsa.GetKey().dkey, rsa.GetKey().nkey);
}
```

