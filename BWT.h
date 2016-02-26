#include <string>
#include <fstream>
#include <ctime>
#include <iostream>
#include <cstdlib>
#include <list>
#include <sstream>
#include <divsufsort.h>
#include "MulIncrease.h"
//#include "suffix.h"
#include "LM.h"
//#define readSize (1000)
#define maxSize (1024 *1024 *1024)
#define MaxPatternSize (200)
typedef unsigned char BYTE;
using namespace std;

class BWT{
private:
	string inFileName;
	string outFileName;
//	string midFileName[3];
	ifstream in;
//	unsigned int length;
	unsigned int LMNum;
//	unsigned int count[AlphaNum];
	char* cc;
	string s;
	//suffixNum* sn;
//	ofstream* out[3];
	LM*  lm[maxSize / handSize];

	void read(bool flag){
		in.open(inFileName.c_str(), ios::binary);
		time_t t = time(0);
//		ofstream out(outFileName.c_str(), ios::binary);
		cc = new char[handSize + 5]; //多余四个字节，存放0用于后缀串排序
		memset(cc, 0, sizeof(char[handSize + 4]));
		unsigned int sc = 0;
		unsigned long start = 0, read = 0, lmstart = 0;
		unsigned int *rank = new unsigned int[handSize + 1];
		unsigned int first_Rank = 0; //原串在排完序的字符串数组中的位置
		char* c = new char[handSize + 4];
	//	in >> length;
	//	cout << "length:" << length << endl;
	//	in.read(cc, 2);
		read = 0;
		while (!in.eof()){
			while (!in.eof() && (read + readSize) <= handSize ){
				in.read(cc + read,readSize);
			//	read += readSize;
                read += in.gcount();
			}
     //       ofstream newout("mynewout", ios::binary);
        //    newout.write(cc, read);
         //   newout.close();
		//	MulIncrease m;
			unsigned long size = read;
			if (!in.eof()){
    //            cout << "Pattern" << endl;
			//	size = read;  /**注意，这里有一个默认，即再读取MaxPatternSize这么多字符，不会超过handsize*/
				in.read(cc + size, MaxPatternSize);
				size += in.gcount();
			}
		//	else{
		//		size = length - start;
		//	}
		//	m.sort(cc, size, 4, rank);
            cc[size] = 0;
			cout << "start: " << size << endl;
            divsufsort((unsigned char *)cc, (int *)rank, size + 1);
			cout << "sort end";
			c[0] = cc[size - 1];
			for (unsigned int i = 1; i <= size; i++){
				if (rank[i] == 0){
					first_Rank = i;
					c[i] = 0;
				}
				else{
					c[i] = cc[(rank[i] - 1)];
				}
		//		out.write(cc + rank[i], size - rank[i] + 1);
		//		out << endl;
			}
		//	out.write(c, size + 1);
		//	out.flush();
		//	(this->out[0])->write(cc, size + 1);
		//	(this->out[0])->flush();
			cout << "write end!" << time(0) - t << endl;
			this->lm[LMNum++] = new LM(c, size, first_Rank, start, flag);
			start = start + read;
	//		lmstart += size + 1;
			if (size > MaxPatternSize){
				memcpy(cc, cc + size - MaxPatternSize + 1, MaxPatternSize - 1);
				read = MaxPatternSize - 1;
			}
			read = MaxPatternSize - 1;
			cout << "use time:" << time(0) - t << endl << endl;
		//	out.write(cc, 100 * 1024);
		//	out.flush();
		}
		in.close();
	//	out.close();
		delete[] c;
		delete[] cc;
		delete[] rank;

		//剩下的操作是对L字符串的操作
//		in.open(outFileName.c_str(), ios::binary);
	}

public:
	BWT(string name, bool flag){
		this->inFileName = name + ".txt";
		this->outFileName = name + ".out";
//		this->midFileName[0] = name + "0.midout";
//		this->midFileName[1] = name + "1.midout";
//		this->midFileName[2] = name + "2.midout";
		time_t t = time(0);
	//	sn = new suffixNum[handSize];
		for (unsigned int i = 0; i < 3; i++){
		//	out[i] = new ofstream(midFileName[i].c_str(), ios::app);
		}
		LMNum = 0;
		read(flag);
		cout << "use time:" << time(0) - t << endl;
	}

	unsigned int strCount(char obj[], unsigned int length){
		unsigned int num = 0, sp = 0, ep = 0;
		for (unsigned int i = 0; i < LMNum; i++){
			num += (this->lm[i])->strCount(obj, length, sp, ep);
		//	cout <<"BWT : num : " << num <<endl;
		}
		return num;
	}
	unsigned int strLocate(char obj[], unsigned int length){
        unsigned int num = 0;
		ofstream findout((inFileName + ".out").c_str(), ios::binary|ios::app);
		findout  << endl;
		for (unsigned int i = 0; i < LMNum; i++){
			num += (this->lm[i])->strLocate (obj, length, findout);
		}
		findout.close();
		return num;
	}
	~BWT(){
		for (unsigned int i = 0; i < 3; i++){
		//	out[i]->close();
		//	delete out[i];
		}
	//	in.close();
		for (unsigned int i = 0; i < LMNum; i++){
			delete this->lm[i];
		}
	//	delete[] sn;
	}
};
