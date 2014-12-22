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
		pfail(false, "Не удалось открыть файл по заданному пути (%s)", path);
		if (!f)
			pfail(true, "Не удалось открыть файл с помощью запасного пути (%s). Сворачиваемся...", default_path);

	}
	return f;
};

bool input_from_file(FILE* in, int mode, char** input_buffer_addr, char** input_key_or_path_buffer_addr) {
	//формат входного файла
	//<КЛЮЧ ШИФРОВАНИЯ/РАСШИФРОВКИ | ключевое слово auto + путь для сохранения файла>\r\n
	//[ДАННЫЕ]
	
	
	
	char buf[256];
	
	bool getting_key = false, got_key = false, auto_key = false;
	//лямбда выражение для "умного" копирования ключа
	//копирует строку в input_key_or_path_buffer_addr начиная с offset'ого элемента - ибо это место для ключа.
	//если дошли до новой строки пока копировали ключ, копируем остаток в input_buffer_addr и возвращаем true.
	//то есть true - знак того, что мы закончили читать ключ или путь для генерации.
	auto lambda_obtain_key = [buf, input_buffer_addr, input_key_or_path_buffer_addr](int offset)->bool{
		size_t s;
		s = strlen(buf);
	};
	//алгоритм
	//берем строки пока есть
	while (!feof(in)){
		fgets(buf, 256, in);
		
		//первая строка - еще ничего не прочитали
		if (!got_key&&!getting_key){
			int offset = 0; //используется для пропусков пробелов. Только при авто-генерации
			//проверим первые 4 байта. Если они равны "auto " - значит дальше идет путь к записи ключа
			if (strstr(buf, "auto ") == buf) {
				//доступно только для шифрования
				if (!mode)
					fail("Авто-генерация ключа шифрования доступна только, очевидно, для шифрования.");
				auto_key = true;
				//пропускаем пробельные символы
				for (offset = 4;buf[offset]>=0&&isspace(buf[offset]); ++offset);

			}
			//нет? тогда это просто ключ. Копруем до новой строки или до конца буффера
			
			getting_key = true;
		}
		//получаем ключ
		else if (getting_key){
			//Копируем до новой строки или до конца буффера
		}
		//уже есть полный ключ|путь к генерации ключа
		else {

		}

	} 
	if (!got_key)
		fail("Неожиданный конец входного файла.");
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
			fail("Извините, но отрицательные числа не поддрерживаются.");
		else
			fail("Ошибка чтения - знак в неожиданном месте.");
	}
	else if (str[i] < 0 || !isdigit(str[i]))
		fail("Ошибка чтения - неизвестный или неподдерживаемый символ");
	else {
		if (start_from == -1)
			start_from = i;
		++ndg;
	}

	if (ndg>refs || (ndg == refs&&strcmp(str + start_from, reference) > 0))
		pfail(true, "Переполнение буфера памяти для числа: %s", str);


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
	1.НОД(0, n) = n; НОД(m, 0) = m; НОД(m, m) = m;
	2.НОД(1, n) = 1; НОД(m, 1) = 1;
	3.Если m, n чётные, то НОД(m, n) = 2*НОД(m/2, n/2);
	4.Если m чётное, n нечётное, то НОД(m, n) = НОД(m/2, n);
	5.Если n чётное, m нечётное, то НОД(m, n) = НОД(m, n/2);
	6.Если m, n нечётные и n > m, то НОД(m, n) = НОД((n-m)/2, m);
	7.Если m, n нечётные и n < m, то НОД(m, n) = НОД((m-n)/2, n);
	*/
	unsigned long c = euclid_rec_bin_entry(a, b);
	*buf = (char*)malloc(128);
	sprintf(*buf, "Условие задачи: Найти НОД(%lu,%lu)\r\nНОД(%lu,%lu) = %lu", a, b, a, b, c);

}
void euclid_expanded(unsigned long a, unsigned long b, char** buf) {
	/*
	1.НОД(0, n) = n; НОД(m, 0) = m; НОД(m, m) = m;
	2.НОД(1, n) = 1; НОД(m, 1) = 1;
	3.Иначе - циклим соотношение Безу
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

	sprintf(*buf, "Условие задачи: Найти НОД(%lu,%lu)\r\nНОД(%lu,%lu) = %lu", a, b, a, b, c);

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
	sprintf(*buf, "Условие задачи: Найти d = %ul*%ul(mod %ul)\r\nd = %ul", a, b, c, d);

}
void mod_pow(unsigned long a, unsigned long b, unsigned long c, char** buf){
	unsigned long d = 1, cura = 1, i, moda = a%c;
	*buf = (char*)malloc(128);
	if (b > 65535)
		fail("Слишком большая степень для возведения ", false);
	const int s = CHAR_BIT*sizeof(unsigned long) / sizeof(char);
	for (i = 0; i < s; ++i) {
		//a*b(mod r) = ( a (mod r) * b (mod r) ) (mod r)
		//a^2(mod r) = [ a (mod r) ]^2 (mod r)
		//for a^2 need r/2 and, if r is odd + 1
		cura = (cura*moda) % c; //a^(2*i) (mod c) = (a^(2*i-1) (mod c) * a (mod c) ) (mod c)
		if (b&(1 << i))
			d = (d*cura) % c;
	}

	sprintf(*buf, "Условие задачи: Найти d = %ul^%ul(mod %ul)\r\nd = %ul", a, b, c, d);
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
	const char *header = "Лаб. ДМК№8 Алгоритм RSA c 16-битным модулем";

	FILE *in = NULL, *out = NULL;
	setlocale(LC_ALL, "Russian");

	//from file
	if (!TO_CONSOLE){
		if (argc<min_num_args)
			fail("Не хватает аргументов. Usage: <path.exe> <decrypt|encrypt|generate> <path_to_in> <path_to_out>");
		//check algo type
		if (stricmp(argv[1], "encrypt") == 0)
			mode = 1;
		else if (stricmp(argv[1], "decrypt") == 0)
			mode = 0;
		else if (stricmp(argv[1], "generate") == 0)
			mode = -1;
		else
			fail("Параметр \"encryption_type\"имеет неправильный формат. Допустимые значения: encrypt|decrypt|generate.");
		in = safe_open(argv[2], nullptr, "r"),
		out = safe_open(argv[3], nullptr, "w");
		//if we are to encrypt|decrypt file
		//obtain encryption key|decryption key, or a flag, that indicates to generate the key (available only in encryption mode)
		if (mode >= 0)
			auto_gen = input_from_file(in, mode, &input, &key_or_path);
		
	}
	//from console
	else {
		fail("Консольный ввод не разрешен");

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