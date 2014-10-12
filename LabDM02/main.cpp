#pragma warning(disable:4996)
#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<conio.h>
#include<io.h>
#include<string.h>
//#include<alloc.h>

#include "iostream"
#include "ctype.h"
using namespace std;

void fail(const char* str, bool close = true){
	puts(str);
	if (close){
		getch();
		exit(1);
	}
};


void main(int argc, char* argv[])
{
	char msA[12], ms[100], tmp[5];
	char * mout, *m_in;
	int * mnX;
	int i, j, r, kA, nA, kB, nB, MA, nX0;
	FILE *in, *out;

	if (argc < 3) {
		fail("Arguments to the program are: {input file path} {output file path}. Trying to apply defaults...", false);
		in = fopen("in.txt", "rb");
		out = fopen("out.txt", "w");

	}
	else {
		in = fopen(argv[1], "rb");
		out = fopen(argv[2], "w");
	}

	if (!in)
		fail("Failed to open input file. Closing...");
	if (!out)
		fail("Failed to open output file for writing. Closing...");

	fgets(ms, 100, in);            /* � ������ ������ ��� ���������� ��� ���������*/
	fgets(ms, 100, in);
	// kA; //���� ��������� � A
	kA = atoi(ms);    /*�� ������ �������� ��������� A �������� ��� �������������� �������������*/
	fgets(ms, 100, in); /*�� ����� ��������� 3-� ������, ���������� ������ ��������� ��������� A*/

	r = 0;   /* r � ������� � ms*/
	nA = 0;
	msA[0] = atoi(ms + r);
	nA = 1 << msA[0];     /* nA � �������������� ������������� ��������� A*/
	for (int i = 1; i <kA; i++)
	{
		while (ms[r] != ',')    r++;
		r++;
		msA[i] = atoi(ms + r);
		nA |= 1 << msA[i];
	}
	/* ���������� ��������� ��������� B � ��������� ��� ����������������������������*/
	fgets(ms, 100, in);                                  /*��������� 4-� ������ �������� �����*/
	nB = atoi(ms);
	fgets(ms, 100, in);     /*�� ����� ��������� 5-� ������, ���������� ������ ��������� ��������� B*/
	r = 0;
	//nB = 1 << atoi(ms + r);         /* ���������� ������ ��� ��������� B � ����� ������ �� �����*/
	for (int i = 1; i <nB; i++)
	{
		while (ms[r] != ',')   
			r++;
		r++; nB |= 1 << atoi(ms + r);
	}
	if (nA & nB != nA) {
		puts("Net reshenyi!");   exit(0); //�������� ������������ �����
	}
	//	int * mnX; /* � mnX[] ����� ����������� �������������� ������������� ������� ������ ���������-�������������� ���������*/
	MA = 1 << kA; /*MA � ����� ����������� ��������� A*/
	nX0 = nB & (~nA & 4095);          /* nX0 � �������������� ������������� ��������� B \ A (���������� ������������ ������� ��������� */

	mnX = (int*)malloc(sizeof(int) * MA);
	for (i = 0; i < MA; i++)
	{

		mnX[i] = nX0;
		for (j = 0; j < kA; j++)
			if (i & (1 << j))
				mnX[i] |= (1 << msA[j]);
	}

	mout = (char*)malloc(80 * MA);
	*mout = 0;
	for (i = 0; i <MA; i++)
	{
		sprintf(tmp, "{"); /* ������ ������ i-�� ������� � ���������� ����������� �������� ������ */
		strcat(mout, tmp); /* ������������ (�������) �����; � ������ mout ����������� ������ tmp*/
		/* ������������ ��������� i-�� ������� */
		size_t u = 0;
		for (j = 0; j < 12; j++)
			if (mnX[i] & (1 << j)) {
				sprintf(tmp, "%d,%s", j, (u<11)?" ":"");
				strcat(mout, tmp);
				++u;
			}
		strcat(mout, "}");
		/*������� strlen(ms) ���������� ����� ������ ms*/
		sprintf(tmp, "\n"); /* ���������� ������� ����� ������*/
		strcat(mout, tmp);
	}
	//fclose(in);
	fseek(in, 0, SEEK_END);
	int size = ftell(in);
	fseek(in, 0, SEEK_SET);
	m_in = (char*)malloc(size);
	m_in[0] = 0;
	fread(m_in, size, 1, in);
	fprintf(out, m_in);
	fprintf(out, "��������� ���������   �������� ��������� ��������� : \r\n");

	fprintf(out, "%s", mout);
	fclose(out);
	fclose(in);
	puts("End");
	getch();                                 /* ��� �������� �� ������ ������ "End"  */


}

