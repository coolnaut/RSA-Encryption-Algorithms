#include"RSA.h"

//核心算法#######################################
//核心算法#######################################
RSA::RSA()
{
	ProduceKeys();
}
//加密与解密单个信息
//这里只是密钥不同，函数则是相同的
bm::int1024_t RSA::ecrept(bm::int1024_t msg, bm::int1024_t key, bm::int1024_t pkey)
{
	//加密公式为：ret = 信息^key%n
	//DO优化，快速幂取模
	//最终的结果为：ret = 信息^key%n == (A0 * A1 * A2 * …… * An)
	bm::int1024_t retmsg = 1;
	bm::int1024_t a = msg;
	bm::int1024_t c = pkey;
	while (key)
	{
		if (key & 1)//从第一个不是0的位开始乘
		{
			//A0 == a----> A0 == a^2^0 %n ----> a%n%n == a% n
			//A1 == A0 * A0 % n
			//第1个信息位a%n
			//第二个为 A0 * A1
			//第n个则是 (A0 * A1 * A2 * …… *A(n - 1) *  An)
			//即就是 (A0 * A1 * A2 * …… *A(n - 1) *   A(n - 1) * A(n - 1) )
			//9当前结果
			retmsg = (retmsg * a) % c;
		}
		//下一次
		key >>= 1;
		a = (a * a) % c;
	}
	return retmsg;

}
bm::int1024_t RSA::ProducePrime()
{
	br::mt19937 ran(time(nullptr));
	br::uniform_int_distribution<bm::int1024_t> dist(1, bm::int1024_t(1) << 50);// 随机大数
	while (1)
	{
		bm::int1024_t tmp = dist(ran);
		if (is_prime(tmp))
		{
			return tmp;
		}
	}
	return 0;
}
bool RSA::is_prime(bm::int1024_t prime)
{
	//std::cout << "is prime" << std::endl;
	br::mt11213b gen(time(nullptr));
	if (miller_rabin_test(prime, 25, gen))
	{
		if (miller_rabin_test((prime - 1) / 2, 25, gen))
		{
			return true;
		}
	}
	return false;
}
void RSA::ProduceKeys()
{
	std::ifstream inputfile("./CONFIG.txt");
	if (!inputfile.is_open())
	{
		bm::int1024_t prime1 = ProducePrime();
		bm::int1024_t prime2 = ProducePrime();
		while (prime1 == prime2)
		{
			//Sleep(1000);
			prime2 = ProducePrime();
		}
		std::ofstream outputfile("./CONFIG.txt", std::ios::binary|std::ios::app);
		outputfile << prime1 << "\n";
		outputfile << prime2;

		outputfile.close();
	}
	std::string buff;
	std::string tmp;
	std::getline(inputfile, buff);
	std::getline(inputfile, tmp);
	bm::int1024_t prime1(buff.c_str());
	bm::int1024_t prime2(tmp.c_str());
	inputfile.close();

	//std::cout << prime1 << std::endl;
	//std::cout << prime2 << std::endl;
	key_.nkey = ProduceNkey(prime1, prime2);

	bm::int1024_t orla = ProduceOrla(prime1, prime2);
	key_.ekey = ProduceEkey(orla);

	key_.dkey = producedkey(key_.ekey, orla);
}
bm::int1024_t RSA::ProduceNkey(bm::int1024_t prime1, bm::int1024_t prime2)
{
	//选择两个不相等的质数p，q，得 n = pq
	return prime1 * prime2;
}

bm::int1024_t RSA::ProduceOrla(bm::int1024_t prime1, bm::int1024_t prime2)
{
	//欧拉公式，f(n)
	//如果a是制数：f(n) = n - 1
	//如果a, b都是制数：f(ab) = (a - 1)(b - 1)
	//如果a, b是互质：f(ab) = f(a)f(b)
	return (prime1 - 1)*(prime2 - 1);
}


bm::int1024_t RSA::ProduceEkey(bm::int1024_t orla)
{
	//得到f(n)，选择一个1 < E < f(n)，且E与n互质，得公钥(E, n)
	bm::int1024_t i = 2;
	for (i; i < orla; ++i)
	{
		if (ProduceGcd(i, orla) == 1)
			break;
	}
	return i;
}

bm::int1024_t RSA::ProduceGcd(bm::int1024_t ekey, bm::int1024_t orla)
{
	if (orla == 0)
		return ekey;

	return RSA::ProduceGcd(orla, ekey%orla);
}
bm::int1024_t RSA::ProduceGcd(bm::int1024_t ekey, bm::int1024_t orla, bm::int1024_t& x, bm::int1024_t& y)
{
	//扩展的欧几里得算法
	if (orla == 0)
	{
		x = 1;
		y = 0;
		return ekey;
	}
	bm::int1024_t ret = ProduceGcd(orla, ekey%orla, x, y);
	bm::int1024_t tmpx = x;
	bm::int1024_t tmpy = y;
	x = tmpy;
	y = tmpx - (ekey / orla)*tmpy;
	return ret;
	//if (orla == 0) 
	//	return ekey;
	//return RSA::ProduceGcd(orla, ekey%orla);
}
bm::int1024_t RSA::producedkey(bm::int1024_t ekey, bm::int1024_t orla)
{
	//计算e与f(n)的逆元 D，得密钥(D, n)


	//由扩展的欧几里得算法

	bm::int1024_t dkey = 1;
	bm::int1024_t Y = 3;
	bm::int1024_t tmp = ProduceGcd(ekey, orla, dkey, Y);
	dkey = (dkey%orla + orla) % orla;
	return dkey;
}






//加密过程############################
//加密过程############################
Key RSA::GetKey()
{
	return key_;
}
std::vector<bm::int1024_t> RSA::Ecrept(std::string& str_in, bm::int1024_t ekey, bm::int1024_t pkey)
{
	std::vector<bm::int1024_t> retv;
	for (size_t i = 0; i < str_in.size(); ++i)
	{
		retv.push_back(ecrept(str_in[i], ekey, pkey));
	}
	return retv;
}
std::string RSA::Decrept(std::vector<bm::int1024_t>& ecrept_str, bm::int1024_t dkey, bm::int1024_t pkey)
{
	std::string retstr;
	for (size_t i = 0; i < ecrept_str.size(); ++i)
	{
		retstr += (char)ecrept(ecrept_str[i], dkey, pkey);
	}
	return retstr;
}





const int NUM = 256;//每次读256字节

void RSA::FileEcrept(const char* plain_file_in, const char* ecrept_file_out,
	bm::int1024_t ekey, bm::int1024_t pkey)
{
	std::ifstream file_in(plain_file_in, std::ios::binary);
	std::ofstream file_out(ecrept_file_out, std::ios::binary | std::ios::app);

	if (!(file_in.is_open() && file_out.is_open()))
	{
		std::cout << "open error" << std::endl;
		return;
	}
	char buff_in[NUM];
	bm::int1024_t buff_out[NUM];
	int curnum;//当前读取的字节
	while (!file_in.eof())
	{
		file_in.read(buff_in, NUM);
		curnum = file_in.gcount();
		for (int i = 0; i < curnum; ++i)
		{
			buff_out[i] = ecrept((bm::int1024_t)buff_in[i], ekey, pkey);
		}
		file_out.write((char*)buff_out, curnum * sizeof(bm::int1024_t));
	}

	file_in.close();
	file_out.close();
}
void RSA::FileDecrept(const char* ecrept_file_in, const char* plain_file_out,
	bm::int1024_t dkey, bm::int1024_t pkey)
{
	std::ifstream file_in(ecrept_file_in, std::ios::binary);
	std::ofstream file_out(plain_file_out, std::ios::binary | std::ios::app);

	if (!(file_in.is_open() && file_out.is_open()))
	{
		std::cout << "open error" << std::endl;
		return;
	}
	bm::int1024_t buff_in[NUM];
	char buff_out[NUM];
	int curnum;//当前读取的字节
	while (!file_in.eof())
	{
		file_in.read((char*)buff_in, NUM * sizeof(bm::int1024_t));
		curnum = file_in.gcount();
		curnum /= sizeof(bm::int1024_t);
		for (int i = 0; i < curnum; ++i)
		{
			buff_out[i] = (char)ecrept(buff_in[i], dkey, pkey);
		}
		file_out.write(buff_out, curnum);
	}

	file_in.close();
	file_out.close();
}
void RSA::PrintInfo(std::vector<bm::int1024_t>& ecrept_str)
{
	for (const auto e : ecrept_str)
	{
		std::cout << e << " ";
	}
	std::cout << std::endl;
}