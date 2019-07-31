#include"RSA.h"




//核心算法#######################################
//核心算法#######################################
RSA::RSA()
{
	ProduceKeys();
}
//加密与解密单个信息
//这里只是密钥不同，函数则是相同的
long RSA::ecrept(long msg, long key, long pkey)
{
	//加密公式为：ret = 信息^key%n
	//TODO优化，快速幂取模
	//最终的结果为：ret = 信息^key%n == (A0 * A1 * A2 * …… * An)
	long retmsg = 1;
	long a = msg;
	long c = pkey;
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
	//long tmp = pow(msg, key) ;
	//return tmp % pkey;
}
long RSA::ProducePrime()
{
	//弄一个简单的素数表。
	long primearr[] = { 2, 3,5 ,7,11, 13,17,19,23,29,31,37,41,43,47,53,59,61,67,71 };
	srand(time(nullptr));
	int t = rand() % (sizeof(primearr)/sizeof(long) - 1);

	return primearr[t];
}

void RSA::ProduceKeys()
{
	long prime1 = ProducePrime();
	long prime2 = ProducePrime();
	while (prime1 == prime2)
	{
		prime2 = ProducePrime();
	}
	key_.nkey = Producenkey(prime1, prime2);

	long orla = ProduceOrla(prime1, prime2);
	key_.ekey = ProduceEkey(orla);

	key_.dkey = producedkey(key_.ekey, orla);
}
long RSA::Producenkey(long prime1, long prime2)
{
	//选择两个不相等的质数p，q，得 n = pq
	return prime1 * prime2;
}
long RSA::ProduceOrla(long prime1, long prime2)
{
	//欧拉公式，f(n)
	//如果a是制数：f(n) = n - 1
	//如果a, b都是制数：f(ab) = (a - 1)(b - 1)
	//如果a, b是互质：f(ab) = f(a)f(b)
	return (prime1 - 1)*(prime2 - 1);
}

long RSA::ProduceGcd(long ekey, long orla)
{
	if (orla == 0)
		return ekey;

	return RSA::ProduceGcd(orla, ekey%orla);
}
long RSA::ProduceEkey(long orla)
{
	//得到f(n)，选择一个1 < E < f(n)，且E与n互质，得公钥(E, n)
	//公约数为 1 的两个数是互质的，因此暴力搜索。一个E出来即可。
	long i = 2;
	for ( i; i < orla; ++i)
	{
		if (ProduceGcd(i, orla) == 1)
			break;
	}
	return i;
}


long RSA::producedkey(long ekey, long orla)
{

	long D = orla / ekey;
	for (D; D < orla; ++D)
	{
		if ((D*ekey) % orla == 1)
			break;
	}
	return D;
}


//加密过程############################
//加密过程############################
Key RSA::GetKey()
{
		return key_;
}
std::vector<long> RSA::Ecrept(std::string& str_in, long ekey, long pkey)
{
	std::vector<long> retv;
	for (size_t i = 0; i < str_in.size(); ++i)
	{
		retv.push_back(ecrept(str_in[i], ekey, pkey));
	}
	return retv;
}
std::string RSA::Decrept(std::vector<long>& ecrept_str, long dkey, long pkey)
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
	long ekey, long pkey)
{
	std::ifstream file_in(plain_file_in, std::ios::binary);
	std::ofstream file_out(ecrept_file_out, std::ios::binary | std::ios::app);

	if (!(file_in.is_open() && file_out.is_open()))
	{
		std::cout << "open error" << std::endl;
		return;
	}
	char buff_in[NUM];
	long buff_out[NUM];
	int curnum;//当前读取的字节
	while (!file_in.eof())
	{
		file_in.read(buff_in, NUM);
		curnum = file_in.gcount();
		for (int i = 0; i < curnum; ++i)
		{
			buff_out[i] = ecrept((long)buff_in[i], ekey, pkey);
		}
		file_out.write((char*)buff_out, curnum * sizeof(long));
	}

	file_in.close();
	file_out.close();
}
void RSA::FileDecrept(const char* ecrept_file_in, const char* plain_file_out,
	long dkey, long pkey)
{
	std::ifstream file_in(ecrept_file_in , std::ios::binary);
	std::ofstream file_out(plain_file_out, std::ios::binary | std::ios::app);

	if (!(file_in.is_open()&&file_out.is_open()))
	{
		std::cout << "open error" << std::endl;
		return;
	}
	long buff_in[NUM];
	char buff_out[NUM];
	int curnum;//当前读取的字节
	while (!file_in.eof())
	{
		file_in.read((char*)buff_in, NUM * sizeof(long));
		curnum = file_in.gcount();
		curnum /= sizeof(long);
		for (int i = 0; i < curnum; ++i)
		{
			buff_out[i] = (char)ecrept(buff_in[i], dkey, pkey);
		}
		file_out.write(buff_out, curnum);
	}

	file_in.close();
	file_out.close();
}
void RSA::PrintInfo(std::vector<long>& ecrept_str)
{
	for (const auto e : ecrept_str)
	{
		std::cout << e << " ";
	}
	std::cout << std::endl;
}