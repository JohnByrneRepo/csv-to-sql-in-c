// Win32Project2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


#include <iostream>
using namespace std;

#include<stdio.h>

#include<string.h>

void create_marks_csv(int a[][3], int n, int m) {

	printf("\n Creating %s.csv file", "test");

	FILE *fp;

	int i, j;

//	filename = strcat("test", ".csv");

	fp = fopen("test.csv", "w+");

	fprintf(fp, "Student Id, Physics, Chemistry, Maths");

	for (int loop = 0; loop < 200000; loop++) {
		for (i = 0; i < m; i++) {
			int id = (loop * 3) + i + 1;
			fprintf(fp, "\n%d", id); // i + 1);

			for (j = 0; j < n; j++)

				fprintf(fp, ",%d ", a[i][j]);

		}
	}
	fclose(fp);

	printf("\n %sfile created", "test.csv");

}

int main() {

	int a[3][3] = { { 50,50,50 },{ 60,60,60 },{ 70,70,70 } };

	//char str[100];

	//printf("\n Enter the filename :");

	//gets(str);

	create_marks_csv(a, 3, 3);
	std::cin.get();
	return 0;

}
