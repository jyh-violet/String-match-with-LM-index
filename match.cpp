#include<fstream>
#include "BWT.h"

using namespace std;
//现在的主要问题在于文件格式上, 这会影响文件的长度处理和一开始要读几个字符.
int main(){
    string name, matchFileName;
    int flag = 1;
	//string name = "test";
	//string name = "SRR311115_rows_ATCG_handle";
	//string name =  "SRR163132_rows_ATCG_handle";
//    string name =  "SRR00001_rows_ATCG_handle";
//	string matchFileName = "search_SRR00001.txt";
	//string matchFileName ="search_SRR163132.txt";
    //string matchFileName = "search_SRR311115.txt";

    cin >> name;
    cin >> matchFileName;
    cin >> flag;
    cout << "start" << endl;
	BWT bwt(name, flag == 1);
	unsigned int t = time(0);
    ifstream in(matchFileName.c_str(), ios::binary);
    cout << "start search"<< endl;
    char s[250];
	unsigned int i = 0;
	while(!in.eof()){
        in.getline(s, 250);
        if(strlen(s) == 0){
		break;
        }
        cout << "number: " << i++ << endl;
     //   unsigned int num = bwt.strLocate(s, strlen(s) - 1);
        unsigned int num = bwt.strLocate(s, strlen(s) - 1);
        cout << ", Num: " << num << endl;
        cout << "use time: " << time(0) - t << endl;
	}
	in.close();
	cout << "total time: " << time(0) - t << endl;
	return 0;
}
