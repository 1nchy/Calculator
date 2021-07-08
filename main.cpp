#include <iostream>
#include <algorithm>
#include <cstdio>
#include <cstdlib>

#include "calculator.h"
#include "display.h"

using std::cin;
using std::cout;
using std::endl;

using std::find;
using std::vector;

int main(int argc, char *argv[]) {
	cout << welcomeMsg << endl;
	cout << versionMsg << endl;
	Expression ep;
	string function;
	vector<string> sVector;
	vector<string>::iterator sVectorIter;
	string::iterator sIter;
	while (1)
	{
		string s;
		char c;
		bool asgn = false, asfx = false;
		unsigned outPos = 0;
		sVectorIter = sVector.end();
		sIter = s.begin();
		do
		{
			printf("%s", inputNotice.c_str());
			// if (std::getline(cin, s).eof()) {
			// 	cout << endl;
			// 	return 0;
			// }
			
			system ("stty raw");
			while((c=getchar())){
				// Enter键
				if(c=='\r'){
					printf("\b\b");
					printf("  ");
					printf("\b\b");
					for (auto iter=sIter; iter != s.end(); iter++)
					{
						cout << *iter;
					}
					break;
				}
				// Ctrl+C or Ctrl+D
				if (c == '\04'||c == '\03'){
					system("/bin/stty cooked");
					return 0;
				}
				// 方向键
				if (c == '\033')
				{
					getchar();
					char arrow = getchar();
					// 处理上方向键
					if(arrow=='A'){
						printf("\b\b\b\b");
						printf("    ");
						printf("\b\b\b\b");
						if(!s.empty()){
							for (auto iter=sIter; iter != s.begin(); iter--){
								printf("\b");
							}
							for (auto iter=s.begin(); iter != s.end(); iter++){
								printf(" ");
							}
							for (auto iter=s.begin(); iter != s.end(); iter++){
								printf("\b");
							}
						}
						if(sVector.empty()){
							cout << "No history!" << endl;
						}else if(sVectorIter==sVector.begin()){
							cout << *sVectorIter ;
							s = *sVectorIter;
							sIter = s.end();
						}else{
							--sVectorIter;
							cout << *sVectorIter ;
							s = *sVectorIter;
							sIter = s.end();
						}
						continue;
						// 处理下方向键
					}else if(arrow == 'B'){
						printf("\b\b\b\b");
						printf("    ");
						printf("\b\b\b\b");
						if(!s.empty()){
							for (auto iter=sIter; iter != s.begin(); iter--){
								printf("\b");
							}
							for (auto iter=s.begin(); iter != s.end(); iter++){
								printf(" ");
							}
							for (auto iter=s.begin(); iter != s.end(); iter++){
								printf("\b");
							}
						}
						if(sVector.empty()){
							cout << "No history!" << endl;
						}
						else if(sVectorIter==sVector.end()){
							cout << "No more history!";
						}
						else if (sVectorIter == sVector.end() - 1)
						{
							cout << *sVectorIter ;
							s = *sVectorIter;
							sIter = s.end();
						}
						else
						{
							++sVectorIter;
							cout << *sVectorIter ;
							s = *sVectorIter;
							sIter = s.end();
						}
						continue;
					}
					// 处理左方向键
					else if (arrow == 'D')
					{
						printf("\b\b\b\b");
						printf("    ");
						printf("\b\b\b\b");
						for (auto iter=sIter; iter != s.end(); iter++)
						{
							cout << *iter;
						}
						for (auto iter=sIter; iter != s.end(); iter++)
						{
							printf("\b");
						}
						if(sIter!=s.begin()){
							sIter--;
							printf("\b");
						}
					}
					// 处理右方向键
					else if (arrow == 'C'){
						printf("\b\b\b\b");
						printf("    ");
						printf("\b\b\b\b");
						if(sIter!=s.end()){
							for (auto iter=sIter; iter != s.end(); iter++)
							{
								cout << *iter;
							}
							sIter++;
							for (auto iter=sIter; iter != s.end(); iter++)
							{
								printf("\b");
							}
						}
					}
					continue;
				}
				// 退格键
				if((int)c == 127){
					if(sIter!=s.begin()){
						sIter=s.erase(--sIter);
						printf("\b\b\b");
						printf("   ");
						printf("\b\b\b");
						for (auto iter = sIter; iter != s.end(); iter++)
						{
							cout << *iter;
						}
						printf(" ");
						for (auto iter=sIter; iter != s.end(); iter++){
							printf("\b");
						}
						printf("\b");
						continue;
					}
				}
				// printf("%i", int(c));
				// 一般输入情况
				if(sIter==s.end()){
					s += c;
					sIter = s.end();
				}
				else
				{
					sIter=s.insert(sIter, c);
					sIter++;
					for (auto iter = sIter; iter != s.end(); iter++)
						cout << *iter;
					for (auto iter = sIter; iter != s.end(); iter++)
						printf("\b");
				}
		}
			system("/bin/stty cooked");
			cout << endl;
		} while (s.empty() || s.compare(string(s.size(), ' ')) == 0);
		sVector.push_back(s);
		// s 一定非空且非全空格
		if (find(quitSet.cbegin(), quitSet.cend(), s) != quitSet.cend()) {
			system("/bin/stty cooked");
			break;
		}
		else if (find(helpSet.cbegin(), helpSet.cend(), s) != helpSet.cend()) {
			DisplayOperators();
			continue;
		}
		else if (find(echoSet.cbegin(), echoSet.cend(), s) != echoSet.cend()) {
			cout << " x = " << ep.x << endl << "fx = " << ep.fx << endl;
			continue;
		}
		else if (s.at(0) == asgnCmd) {
			s = s.erase(0, 1);
			if (s.empty()) {
				ep.x = 0;
				continue;
			}
			outPos = 1;
			asgn = true;
		}
		else if (s.at(0) == funcCmd) {
			function = s.erase(0, 1);
			if (s.empty()) {
				ep.fx = "";
				continue;
			}
			outPos = 1;
			asfx = true;
			// ep.func = function;
		}

		string base_str;
		auto bpos = s.rfind(baseTrans);
		if (bpos != string::npos) {
			base_str = s.substr(bpos+2);
			// cout << base_str << endl;
			s.erase(bpos);
		}
		ep.init(s, asgn, asfx);
		if (ep.compute() == CORRECT) {
			if (!ep.accurate) {
				DisplayResult(ep.getResult(), base_str);
				// cout << ep.getResult() << endl;
			}
			else {
				printf("%.10lf\n", ep.result);
			}
		}
		else {
			DisplayErrorMsg(ep, outPos);
		}
	}
	return 0;
}
