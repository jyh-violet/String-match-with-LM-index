#include <string>
#include <fstream>
#include <ctime>
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <list>
#include <sstream>
#include <ctime>
//#include "superBucket.h"
//#include "myMath.h"
#define AlphaNum 4
#define readSize (125 * 1024)
#define superSize (64 * 1024)
#define handSize (100* 1024 * 1024)
#define checkSize (100 * 1024)
//#define handSize (10 * 1024 )

typedef unsigned char BYTE;
using namespace std;
class LM{

private:
    string occFileName, posFileName;
	unsigned int size;
	unsigned int startIndex;
	unsigned int first_Rank; //原串在排完序的字符串数组中的位置
	unsigned int count[AlphaNum + 1];  //存储在部分中<= c的个数
	unsigned int* superCount;
	unsigned short* occ; // 只存ACG的occ数组
	unsigned int* Pos;
	unsigned int getNum(char c){
		switch (c)
		{
		case 'A':return 0;
		case 'C':return 1;
		case 'G':return 2;
		case 'T':return 3;
		default:return 4;
		}
	}
	unsigned int getPos(char* cc, unsigned int i, unsigned int lmFlag[], unsigned int p[]){
        unsigned int t1 = LF(cc, i, lmFlag), t2 = 1;
	//	ofstream out("myout", ios::binary);
		while (cc[t1] != 0)
		{
			if (p[t1] != 0){
				cout << "recycle :" << t1  << ", pos: " << p[t1] << endl;
				return 0;
			}
            p[t1] = this->size + 1 - t2 - 1;
			t2++;
			t1 = LF(cc, t1, lmFlag);
		}
        p[t1] =  this->size + 1 - t2 % (size + 1) - 1;
		return t2;
	}
	unsigned int LF(char* cc, unsigned int index, unsigned int lmFlag[]){
		if (cc[index] == 0){
			return 0;
		}
		if (lmFlag[index]){
			return lmFlag[index];
		}
		unsigned int num = Occ(cc[index], index, 0);
		num += count[getNum(cc[index])];
		lmFlag[index] = num;
		return num;
	}

public:
	LM(char *cc, unsigned int size, unsigned int fr, unsigned int startIndex, bool flag){
		this->size = size;
		this->first_Rank = fr;
		this->startIndex = startIndex + 1;
		this->posFileName = (char)((startIndex + handSize / 2)/ handSize + '0');
		this->posFileName += "pos.txt";
		this->occFileName = (char)((startIndex + handSize / 2)/ handSize + '0');
		this->occFileName += "occ.txt";
		cout << "size:" << size << ", first_rank:" << first_Rank << ", startIndex:" << startIndex
            << ", filename:" << posFileName<<  endl;
		//处理桶 和分块
		for (unsigned int i = 0; i < AlphaNum + 1; i++){
			count[i] = 0;
		}
        this -> superCount = new unsigned int[(size + 1) / superSize * 4+ 5];
        this -> occ = new unsigned short[(size + 5) * 3];  // superSize 要小于2^16
		memset(this ->superCount, 0, ((size + 1) / superSize * 4 + 5)  * sizeof(int));
		memset(this ->occ, 0, (size + 5) * 3 * sizeof(short));
        unsigned int i = 0;
        unsigned int superNum = 0;
        unsigned int index = 0, j = 0;
        occ[getNum(cc[0])] ++;
        i ++;
        superCount[0] = superCount[1] = superCount[2] = superCount[3] = 0;
        superNum ++ ;
        unsigned int newCount[AlphaNum];
		memset(newCount, 0, AlphaNum * sizeof(int));
		ofstream out(occFileName.c_str(), ios::binary);
        newCount[getNum(cc[0])] ++;
        unsigned int num1, num2;
        while(i <= size){
            j = getNum(cc[i]);
            if(cc[i] != 0){
                newCount[j] ++;
            }else{
            }
            num2 = (i - 1) * 3;
            if(i % superSize == 0){  //一个superbucket计数结束, occ计数从0,开始,superCount计数
                num1 = superNum * 4;
                superCount[num1] = superCount[num1 - 4] + occ[num2];
                superCount[num1 + 1] = superCount[num1 - 3]  + occ[num2 + 1];
                superCount[num1 + 2] = superCount[num1 - 2]  + occ[num2 + 2];
                superCount[num1 + 3] = superCount[num1 - 1] + superSize - occ[num2] - occ[num2 + 1] - occ[num2 + 2];
                if(superNum == (first_Rank / superSize + 1)){ // 把null也算在T里面了
                    superCount[num1 + 3] --;
                }
				superNum ++;
				occ[num2] = occ[num2 + 3] = 0;
				occ[num2 + 1] = occ[num2 + 4] = 0;
				occ[num2 + 2] = occ[num2 + 5] = 0;
                if( j < 3){
                    occ[i * 3 + j] ++;
                }
            }else{
                occ[num2 + 3] = occ[num2];
                occ[num2 + 4] = occ[num2 + 1];
                occ[num2 + 5] = occ[num2 + 2];
                if( j < 3){
                    occ[num2 + 3 + j] ++;
                }
			}
            i ++;
        }
        count[0] = 1;
		for (unsigned int j = 1; j < AlphaNum + 1; j++){
			count[j] = count[j - 1] + newCount[j - 1];
		}
        out.write((char*)occ, (size + 1)* 3 * sizeof(short));
        out.close();
        cout << "occ end!" << endl;
        cout << count[0] << " " << count[1] << " " << count[2] << "  " << count[3] <<  " " << count[4]<< endl;
        cout <<"superNum:" << superNum <<  ", superCount: "<< superCount[(superNum -1)* 4] << " " << superCount[(superNum - 1) * 4 + 1] << " "
					<< superCount[(superNum - 1) * 4 + 2] << " " << superCount[(superNum - 1) * 4 + 3] << endl;
        cout << "first:" << first_Rank << " is:"<< (int)cc[first_Rank] << endl;
		unsigned int* lmFlag = new unsigned int[size + 2];
		this->Pos = new unsigned int[size + 2];
		memset(lmFlag, 0, sizeof(int) * (size + 2));
		memset(this->Pos, 0, (size + 2) * sizeof(int));
        if(flag){
            getPos(cc, first_Rank, lmFlag, this -> Pos);
            ofstream out(posFileName.c_str(), ios::binary);
            out.write((char*)Pos, (size + 2)*sizeof(int));
            out.close();
		}
		cout << "first:" << first_Rank << ", pos: " << Pos[first_Rank] << endl;
		delete[] lmFlag;
		delete[] this->Pos;
        delete[] this->occ;
	}
	unsigned int strLocate(char obj[], unsigned int length,  ofstream& out){
		unsigned int sp = 0, ep = 0;
		strCount(obj, length, sp, ep);
		ifstream posin(posFileName.c_str(), ios::binary);
		this->Pos = new unsigned int[size + 2];
		posin.seekg(sp * sizeof(int),  ios::beg);
		posin.read((char*)(Pos + sp), (ep - sp + 1) * sizeof(int));
		posin.close();
		for (unsigned int i = sp; i <= ep; i++){
			out << (Pos[i] + 1)%(size + 1) + this -> startIndex << endl;
		}
		delete[] this->Pos;
        if(ep >= sp){
            return ep - sp + 1;
		}else{
            return 0;
		}
	}

    // 0~index -1 有多少c,, 调用Occ函数之前,要确保occ数组在内存里
    unsigned int Occ(char c, unsigned int index, unsigned int base){
   //     myNewMath math;
		if ((index + base) == 0){
			return 0;
		}else if(c == 0){
			if ((index + base)  > first_Rank){
				return 1;
			}
			else{
				return 0;
			}
        }else if(c < 'T'){
            return this->superCount[(index + base) / superSize * 4 + getNum(c)] + this->occ[(index - 1 ) * 3 + getNum(c)];
        }else{
            unsigned int num = 0;
			if ((index + base) > first_Rank){
				num = 1;
			}
            for(unsigned int i = 0; i < 3; i ++){
                num += this->superCount[((index + base) )/ superSize * 4 + i] + this->occ[(index - 1) * 3 + i];
            }
            return (index + base) - num;
        }
    }
    unsigned int getOcc(char c, unsigned index, unsigned short myocc[]){
        if ((index) == 0){
			return 0;
		}else if(c == 0){
			if ((index)  > first_Rank){
				return 1;
			}
			else{
				return 0;
			}
        }else if(c < 'T'){
            return this->superCount[(index) / superSize * 4 + getNum(c)] + myocc[getNum(c)];
        }else{
            unsigned int num = 0;
			if ((index ) > first_Rank){
				num = 1;
			}
            for(unsigned int i = 0; i < 3; i ++){
                num += this->superCount[((index) )/ superSize * 4 + i] + myocc[i];
            }
            return (index) - num;
        }
    }
	unsigned int strCount(char obj[], unsigned int length,unsigned int &sp, unsigned int &ep){
	//	myNewMath math;
		ifstream occin(occFileName.c_str(), ios::binary);
        unsigned int t = time(0);
		unsigned int base = 0;
		unsigned int i = length - 1;
		char c = obj[i];
		//sp：第一个下标， ep：最后一个下标
		sp = count[getNum(c)] , ep = count[getNum(c) + 1] - 1 ;
		unsigned short myspocc[3], myepocc[3];
      //  ofstream out("myout", ios::app);
		while ((sp <= ep) && (i >= 1))
		{
            base = sp>0?(sp - 1) : 0;
            occin.seekg(base * 3 * sizeof(short), ios::beg);
            occin.read((char*)(myspocc), 3* sizeof(short));
            occin.seekg(ep * 3 * sizeof(short), ios::beg);
            occin.read((char*)(myepocc), 3* sizeof(short));
         //   out << i << ":" <<  c << " : sp= " << sp  << ", Occ(c, sp):" << Occ(c, sp) << ",  ep=" << ep  << ", Occ(c, ep + 1):" << Occ(c, ep + 1)<<endl;
			c = obj[i - 1];
			sp = count[getNum(c)] + getOcc(c, sp, myspocc);
			ep = count[getNum(c)] + getOcc(c, ep + 1, myepocc) - 1;
			i--;
		}

//		cout << "count time: " << time(0) - t << endl;
		occin.close();
		if (ep < sp){
			return 0;
		}
		else{
			return ep - sp + 1;
		}
	}
	~LM(){
        delete[] superCount;
	}
};
