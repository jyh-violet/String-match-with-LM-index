#include <iostream>
using namespace std;
typedef unsigned char BYTE;
class MulIncrease{
	//��׺����(������)
private:
	//str ���򴮣�k�ؼ��ּ�࣬
	static bool cmp(unsigned int *str, unsigned int a, unsigned int b, int k){
		//rank���ڵ�����������һ�ڶ��ؼ��ֶ�һ����rankһ��
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
	//str:�ı�����n:�ı������ȣ�m:�ı����������ַ�+1
	static void sort(char* str, unsigned int n, unsigned int m, unsigned int* sa){
		unsigned int* x = new unsigned int[n + 1];
		unsigned int* y = new unsigned int[n + 1];
		unsigned int* c = new unsigned int[n + 1];
	//	unsigned int *sa = new unsigned int[n + 1];
		n++;
		unsigned int i = 0;
		//�״λ������򣬶�ÿλ���ֽ�������
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
			//[n-j,n)û�ж�Ӧ�ĵڶ��ؼ��֣��ʵڶ��ؼ���������С
			for (i = n - j; i < n; i++) y[p++] = i;
			//iΪ�ؼ����������ڶ��ؼ�����ʼλ����[1,j)�������е�һ�ؼ���
			for (i = 0; i < n; i++){
				if (sa[i] >= j){
					y[p++] = sa[i] - j;
				}
			}
			//����sa���飬y[i]����Ϊi�ĵڶ��ؼ�������Ӧ��һ�ؼ��ֵ���ʼλ��
			for (i = 0; i<m; i++) c[i] = 0;
			for (i = 0; i<n; i++) c[x[y[i]]] ++;
			for (i = 1; i<m; i++) c[i] += c[i - 1];
			for (i = n - 1; i > 0; i--) sa[--c[x[y[i]]]] = y[i];
			sa[--c[x[y[0]]]] = y[0];
			//����x���飬x[i]����ʼλ��Ϊi�Ĺؼ���1������
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
		//����rank����
		for (i = 0; i<n; i++)
		     rank[sa[i]] = i + startIndex; */
		delete[] x;
		delete[] y;
		delete[] c;
	}
};
