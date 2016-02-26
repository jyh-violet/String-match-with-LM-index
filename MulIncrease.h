#include <iostream>
using namespace std;
typedef unsigned char BYTE;
class MulIncrease{
	//后缀数组(倍增法)
private:
	//str 排序串，k关键字间距，
	static bool cmp(unsigned int *str, unsigned int a, unsigned int b, int k){
		//rank相邻的两个串，第一第二关键字都一样，rank一样
		return str[a] == str[b] && str[a + k] == str[b + k];
	}
	static unsigned int getNum(char c){
		switch (c)
		{
		case 'A':return 0;
		case 'C':return 1;
		case 'G':return 2;
		case 'T':return 3;
		default:return 0;
		}
	}
public:
	//str:文本串，n:文本串长度，m:文本串中最大的字符+1
	static void sort(char* str, unsigned int n, unsigned int m, unsigned int* sa){
		unsigned int* x = new unsigned int[n + 1];
		unsigned int* y = new unsigned int[n + 1];
		unsigned int* c = new unsigned int[n + 1];
	//	unsigned int *sa = new unsigned int[n + 1];
		n++;
		unsigned int i = 0;
		//首次基数排序，对每位数字进行排序
		for (i = 0; i < n ; i++) c[i] = 0;
		for (i = 0; i < n; i++) c[x[i] = getNum(str[i])] ++;
		for (i = 1; i < m; i++) c[i] += c[i - 1];
		for (i = n - 1; i > 0; i--) sa[--c[x[i]]] = i;
		sa[--c[x[0]]] = 0;
	/*	for (unsigned int i = 0; i < n; i++){
			cout << sa[i] << " ";
		}
		cout << endl; */
		for (unsigned int j = 1; j <= n; j *= 2){
			unsigned int p = 0;
			//[n-j,n)没有对应的第二关键字，故第二关键字排名最小
			for (i = n - j; i < n; i++) y[p++] = i;
			//i为关键字排名，第二关键字起始位置在[1,j)，不会有第一关键字
			for (i = 0; i < n; i++){
				if (sa[i] >= j){
					y[p++] = sa[i] - j;
				}
			}
			//更新sa数组，y[i]排名为i的第二关键字所对应第一关键字的起始位置
			for (i = 0; i<m; i++) c[i] = 0;
			for (i = 0; i<n; i++) c[x[y[i]]] ++;
			for (i = 1; i<m; i++) c[i] += c[i - 1];
			for (i = n - 1; i > 0; i--) sa[--c[x[y[i]]]] = y[i];
			sa[--c[x[y[0]]]] = y[0];
			//更新x数组，x[i]　起始位置为i的关键字1的排名
			swap(x, y);
			p = 1, x[sa[0]] = 0;
			for (i = 1; i < n; i++){
				x[sa[i]] = cmp(y, sa[i - 1], sa[i], j) ? p - 1 : p++;
			}
			if (p >= n)
				break;
			m = p;
			
		}
		cout << "sort end !" << endl;
/*		for (unsigned int i = 0; i < n; i++){
			cout << sa[i] << " ";
		}
		cout << endl;
		//计算rank数组
		for (i = 0; i<n; i++)
		     rank[sa[i]] = i + startIndex; */
		delete[] x;
		delete[] y;
		delete[] c;
	}
};
