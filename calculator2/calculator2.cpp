// calculator2.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include "ctime"
#include <iostream>
#include <windows.h>
#include <string>
using namespace std;

class number {
public:
	int num;
	int newnum = 0;
	int operat;
	bool change = 0;
	bool correct = 1;
	bool ending = 0;
	void create();      //this function is to initial my number, including giving it a num and an operation, they are a couple.
	void end();
	void operation();   //this function turns change to 1, which means this node has been operated.
	void initial();     //for the first node(the one that will contain the sum)
};

void calculator(int i, int j);		//this function creates an exercise, composed of several number nodes.

number exercises[10][10] = {0};    //the first "10" means it can save ten exercises, and the second one means it can content no more than 10.

int main()
{
	srand(time(0));
	bool goon = 1;
	while (goon != 0) {
		printf("now you have 10 questions to answer;");
		DWORD start, end;
		start = GetTickCount();
		for (int i = 0; i < 10; i++) {
			printf("The number %d question:\n", i + 1);
			int opt = rand() % 3 + 3;
			calculator(opt, i);
		}
		end = GetTickCount();
		cout << "You use " << (int)(end - start) / 1000 << " seconds!";
		int wrong = 0;
		for (int m = 0; m < 10; m++) {
			if (exercises[m][0].correct == 0) {
				wrong++;
			}
		}
		cout << "The number of your correct answer: " << 10 - wrong << endl;
		cout << "The following is the right answers of the questions you got wrong:" << endl;
		for (int m = 0; m < 10; m++) {
			if (exercises[m][0].correct == 0) {
				for (int k = 1;; k++) {
					if (exercises[m][k].ending != 1) {
						cout << exercises[m][k].num;
						if (exercises[m][k].operat == 0)cout << "+";
						else if (exercises[m][k].operat == 1)cout << "-";
						else if (exercises[m][k].operat == 2)cout << "*";
						else if (exercises[m][k].operat == 3)cout << "/";
					}
					else {
						cout << exercises[m][k].num << "=" << exercises[m][0].num << endl;
						break;
					}
				}
			}
		}
		cout << "Do you want to continue? 1:yes 0.no  : ";
		cin >> goon;
	}
}

void number::operation() {
	change = 1;
}

void number::create() {
	num = rand() % 101;
	operat = rand() % 4;
	newnum = num;
}

void number::initial() {
	num = 0;
	newnum = 0;
	operat = 0;
	change = 0;
}

void number::end() {
	num = rand() % 101;
	newnum = num;
	operat = 0;
	change = 0;
	ending = 1;
}

void calculator(int n,int i) {
	FILE *fp = fopen("result.txt", "at+");
	for (int k = 1; k <= n; k++) {
		exercises[i][k].create();
		fprintf(fp, "%d", exercises[i][k].num);
		printf("%d", exercises[i][k].num);
		if (exercises[i][k].operat == 0) {
			fprintf(fp, "+");
			printf("+");
		}
		else if (exercises[i][k].operat == 1) {
			fprintf(fp, "-");
			printf("-");
		}
		else if (exercises[i][k].operat == 2) {
			fprintf(fp, "*");
			printf("*");
		}
		else if (exercises[i][k].operat == 3) {
			fprintf(fp, "/");
			printf("/");
		}
	}
	exercises[i][0].initial();
	exercises[i][n + 1].end();
	fprintf(fp, "%d", exercises[i][n + 1].num);
	printf("%d=", exercises[i][n + 1].num);
	int index = 0;
	for (int m = 1; m <= n; m++) {
		if (exercises[i][m - 1].change == 0) {
			if (exercises[i][m].operat == 2) {
				exercises[i][m].newnum = exercises[i][m].num*exercises[i][m + 1].num;
				exercises[i][m].change = 1;
				index++;
			}
			else if (exercises[i][m].operat == 3) {
				exercises[i][m].newnum = exercises[i][m].num/exercises[i][m + 1].num;
				exercises[i][m].change = 1;
				index++;
			}
		}
		else if (exercises[i][m - 1].change == 1) {
			if (exercises[i][m].operat == 0 || exercises[i][m + 1].operat == 1) {
				continue;
			}
			int t = 0;
			for (t = 0; t < index; t++) {
				if (exercises[i][m - t - 1].change == 0)
					break;
			}
			if (exercises[i][m].operat == 2) {
				exercises[i][m - t].newnum = exercises[i][m - t].newnum*exercises[i][m + 1].num;
			}
			else if (exercises[i][m].operat == 3) {
				exercises[i][m - t].newnum = exercises[i][m - t].newnum/exercises[i][m + 1].num;
			}
		}
	}
	for (int m = 1; m <= n + 1; m++) {
		if (exercises[i][m - 1].change == 1)continue;
		else if (exercises[i][m - 1].operat == 0) {
			exercises[i][0].num = exercises[i][0].num + exercises[i][m].newnum;
		}
		else if (exercises[i][m - 1].operat == 1) {
			exercises[i][0].num = exercises[i][0].num - exercises[i][m].newnum;
		}
	}
	fprintf(fp, "%d\n", exercises[i][0].num);
	fclose(fp);
	string answer;
	getline(cin, answer);
	if (answer.size() == 0)exercises[i][0].correct = 0;
	else if (atoi(answer.c_str()) != exercises[i][0].num)exercises[i][0].correct = 0;
}