#pragma warning(disable:4996)
#define _CRT_USE_NO_WARNINGS
#include <cstdio>
#include <cstdlib>
#include <conio.h>
#include <iostream>
#include <fstream>
#include <climits>
#include <iomanip>
#include <varargs.h>
#include <Windows.h>

static const bool TO_CONSOLE = true;

void fail(const char* str, bool close = true){
	puts(str);
	if (close){
		getch();
		exit(1);
	}
};

//an updated version of fail(), which can accept a formatted string with params
void pfail(bool close, const char* format_string, ...){
	va_list vl;
	va_start(vl, format_string);
	vprintf(format_string, vl);
	puts("");
	va_end(vl);
	if (close) {
		getch();
		exit(1);
	}
}

FILE* safe_open(const char* path, const char* default_path, const char* md){
	FILE* f = fopen(path, md);
	if (!f) {
		f = fopen(default_path, md);
		pfail(false, "�� ������� ������� ���� �� ��������� ���� (%s)", path);
		if (!f)
			pfail(true, "�� ������� ������� ���� � ������� ��������� ���� (%s). �������������...", default_path);

	}
	return f;
};

bool input_from_file(FILE* in, int mode, char** input_buffer_addr, char** input_key_or_path_buffer_addr) {
	//������ �������� �����
	//<���� ����������/����������� | �������� ����� auto + ���� ��� ���������� �����>\r\n
	//[������]
	
	
	
	char buf[256];
	
	bool getting_key = false, got_key = false, auto_key = false;
	//������ ��������� ��� "������" ����������� �����
	//�������� ������ � input_key_or_path_buffer_addr ������� � offset'��� �������� - ��� ��� ����� ��� �����.
	//���� ����� �� ����� ������ ���� ���������� ����, �������� ������� � input_buffer_addr � ���������� true.
	//�� ���� true - ���� ����, ��� �� ��������� ������ ���� ��� ���� ��� ���������.
	auto lambda_obtain_key = [buf, input_buffer_addr, input_key_or_path_buffer_addr](int offset)->bool{
		size_t s;
		s = strlen(buf);
	};
	//��������
	//����� ������ ���� ����
	while (!feof(in)){
		fgets(buf, 256, in);
		
		//������ ������ - ��� ������ �� ���������
		if (!got_key&&!getting_key){
			int offset = 0; //������������ ��� ��������� ��������. ������ ��� ����-���������
			//�������� ������ 4 �����. ���� ��� ����� "auto " - ������ ������ ���� ���� � ������ �����
			if (strstr(buf, "auto ") == buf) {
				//�������� ������ ��� ����������
				if (!mode)
					fail("����-��������� ����� ���������� �������� ������, ��������, ��� ����������.");
				auto_key = true;
				//���������� ���������� �������
				for (offset = 4;buf[offset]>=0&&isspace(buf[offset]); ++offset);

			}
			//���? ����� ��� ������ ����. ������� �� ����� ������ ��� �� ����� �������
			
			getting_key = true;
		}
		//�������� ����
		else if (getting_key){
			//�������� �� ����� ������ ��� �� ����� �������
		}
		//��� ���� ������ ����|���� � ��������� �����
		else {

		}

	} 
	if (!got_key)
		fail("����������� ����� �������� �����.");
	return auto_key;
}
unsigned long safe_input_int(const char* str){
	size_t i, refs, s = strlen(str), ndg = 0, start_from = -1;
	unsigned long d = 0;
	const char *reference = "4294967295";
	refs = strlen(reference);
	for (i = 0; i < s; ++i)
	if (str[i] == '-') {
		if (!ndg)
			fail("��������, �� ������������� ����� �� ���������������.");
		else
			fail("������ ������ - ���� � ����������� �����.");
	}
	else if (str[i] < 0 || !isdigit(str[i]))
		fail("������ ������ - ����������� ��� ���������������� ������");
	else {
		if (start_from == -1)
			start_from = i;
		++ndg;
	}

	if (ndg>refs || (ndg == refs&&strcmp(str + start_from, reference) > 0))
		pfail(true, "������������ ������ ������ ��� �����: %s", str);


	d = atol(str + start_from);
	return d;
}

unsigned long euclid_rec_bin_entry(unsigned long a, unsigned long b) {
	unsigned long c;
	//fast and furious
	if (a == 0 || b == 0)
		return max(a, b);
	else if (a == 1 || b == 1)
		return 1;
	else if (a == b)
		return b;
	else if (((a | b) & 1) == 0)
		c = 2 * euclid_rec_bin_entry(a >> 1, b >> 1);
	else if (!(a & 1) && b & 1)
		c = euclid_rec_bin_entry(a >> 1, b);
	else if (a & 1 && !(b & 1))
		c = euclid_rec_bin_entry(a, b >> 1);
	else if ((a&b) & 1)  {
		if (b>a)
			c = euclid_rec_bin_entry((b - a) / 2, a);
		else
			c = euclid_rec_bin_entry((a - b) / 2, b);
	}

	return c;
}
void euclid_binary(unsigned long a, unsigned long b, char** buf) {
	/*
	1.���(0, n) = n; ���(m, 0) = m; ���(m, m) = m;
	2.���(1, n) = 1; ���(m, 1) = 1;
	3.���� m, n ������, �� ���(m, n) = 2*���(m/2, n/2);
	4.���� m ������, n ��������, �� ���(m, n) = ���(m/2, n);
	5.���� n ������, m ��������, �� ���(m, n) = ���(m, n/2);
	6.���� m, n �������� � n > m, �� ���(m, n) = ���((n-m)/2, m);
	7.���� m, n �������� � n < m, �� ���(m, n) = ���((m-n)/2, n);
	*/
	unsigned long c = euclid_rec_bin_entry(a, b);
	*buf = (char*)malloc(128);
	sprintf(*buf, "������� ������: ����� ���(%lu,%lu)\r\n���(%lu,%lu) = %lu", a, b, a, b, c);

}
void euclid_expanded(unsigned long a, unsigned long b, char** buf) {
	/*
	1.���(0, n) = n; ���(m, 0) = m; ���(m, m) = m;
	2.���(1, n) = 1; ���(m, 1) = 1;
	3.����� - ������ ����������� ����
	*/
	unsigned long c;
	*buf = (char*)malloc(128);

	unsigned long first = a, second = b;
	unsigned long ri, qi;
	if (a == 0 || b == 0)
		c = max(a, b);
	else if (a == 1 || b == 1)
		c = 1;
	else if (a == b)
		c = b;
	else {
		bool finished = false;
		do {
			qi = max(first, second) / min(first, second);
			if (first < second)
				ri = second*qi - first;
			else
				ri = first - second*qi;

			if (second % ri == 0)
				finished = true;
			else {
				first = second;
				second = ri;
			}
		} while (!finished);
		c = ri;
	}

	sprintf(*buf, "������� ������: ����� ���(%lu,%lu)\r\n���(%lu,%lu) = %lu", a, b, a, b, c);

}
unsigned long mulmod_entry(unsigned long a, unsigned long b, unsigned long m)
{
	if (!a || !b) return 0;
	if (a & 1)
		return (b + mulmod_entry(a - 1, b, m)) % m;
	return (mulmod_entry(a / 2, b, m) * 2) % m;
}
void mod_multiply(unsigned long a, unsigned long b, unsigned long c, char** buf) {
	unsigned long d = 0;
	*buf = (char*)malloc(128);
	d = mulmod_entry(a, b, c);
	sprintf(*buf, "������� ������: ����� d = %ul*%ul(mod %ul)\r\nd = %ul", a, b, c, d);

}
void mod_pow(unsigned long a, unsigned long b, unsigned long c, char** buf){
	unsigned long d = 1, cura = 1, i, moda = a%c;
	*buf = (char*)malloc(128);
	if (b > 65535)
		fail("������� ������� ������� ��� ���������� ", false);
	const int s = CHAR_BIT*sizeof(unsigned long) / sizeof(char);
	for (i = 0; i < s; ++i) {
		//a*b(mod r) = ( a (mod r) * b (mod r) ) (mod r)
		//a^2(mod r) = [ a (mod r) ]^2 (mod r)
		//for a^2 need r/2 and, if r is odd + 1
		cura = (cura*moda) % c; //a^(2*i) (mod c) = (a^(2*i-1) (mod c) * a (mod c) ) (mod c)
		if (b&(1 << i))
			d = (d*cura) % c;
	}

	sprintf(*buf, "������� ������: ����� d = %ul^%ul(mod %ul)\r\nd = %ul", a, b, c, d);
}


void rsa_genkeypair(){

}
void rsa_encrypt(const int*){

}

void rsa_decrypt(){

}
int* chars_to_int_arr(const char* const arr){

}
char* int_to_chars_arr(const int* const arr){

}


int main(unsigned long argc, char** argv){
	int mode; // true - encrypting, false - decrypting
	char *input, *key_or_path;
	bool auto_gen = false;

	unsigned long p,q,n;
	const unsigned long min_num_args = 4;
	char* buf = NULL;
	const char *header = "���. ��ʹ8 �������� RSA c 16-������ �������";

	FILE *in = NULL, *out = NULL;
	setlocale(LC_ALL, "Russian");

	//from file
	if (!TO_CONSOLE){
		if (argc<min_num_args)
			fail("�� ������� ����������. Usage: <path.exe> <decrypt|encrypt|generate> <path_to_in> <path_to_out>");
		//check algo type
		if (stricmp(argv[1], "encrypt") == 0)
			mode = 1;
		else if (stricmp(argv[1], "decrypt") == 0)
			mode = 0;
		else if (stricmp(argv[1], "generate") == 0)
			mode = -1;
		else
			fail("�������� \"encryption_type\"����� ������������ ������. ���������� ��������: encrypt|decrypt|generate.");
		in = safe_open(argv[2], nullptr, "r"),
		out = safe_open(argv[3], nullptr, "w");
		//if we are to encrypt|decrypt file
		//obtain encryption key|decryption key, or a flag, that indicates to generate the key (available only in encryption mode)
		if (mode >= 0)
			auto_gen = input_from_file(in, mode, &input, &key_or_path);
		
	}
	//from console
	else {
		fail("���������� ���� �� ��������");

	}

	if (mode>=0){

	}
	else {

	}

	fprintf(TO_CONSOLE ? stdout : out, "%s", buf);
	//free all memory
	free(buf);
	if (in)
		fclose(in);
	if (out)
		fclose(out);
	getch();
	return 0;
}