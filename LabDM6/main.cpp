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

void input_from_file(FILE* in, unsigned long *a, unsigned long *b, unsigned long *c) {
	//первая строка -
	char ch = 0;
	if (!fscanf(in, "%d", a))
		fail("Не удалось прочитать первый параметр)");
	do
		ch = fgetc(in);
	while (c<0 || !isdigit(ch));
	fseek(in, -1, SEEK_CUR);

	if (!fscanf(in, "%d", b));
	fail("Не удалось прочитать второй параметр");
	do
		ch = fgetc(in);
	while (c<0 || !isdigit(ch));
	fseek(in, -1, SEEK_CUR);

	if (!fscanf(in, "%d", c));
	fail("Не удалось прочитать третий параметр");

}
unsigned long safe_input_int( const char* str ){
	size_t i,refs, s = strlen(str), ndg = 0, start_from = -1;
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
			
		if (ndg>refs || (ndg==refs&&strcmp(str + start_from, reference) > 0))
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
	unsigned long d = 1, cura =1 , i, moda = a%c;
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

int main(unsigned long argc, char** argv){
	unsigned long numALG = -1, a, b, c;
	const unsigned long min_num_args = 2;
	char* buf = NULL;
	const char* headers[4] = {
		"Коряковцев Алишер. ИУВ-34.\r\n ЛАБ ДМК№6 - Бинарный алгоритм Евклида",
		"Коряковцев Алишер. ИУВ-34.\r\n ЛАБ ДМК№6 - Расширенный алгоритм Евклида",
		"Коряковцев Алишер. ИУВ-34.\r\n ЛАБ ДМК№6 - Умножение по модулю",
		"Коряковцев Алишер. ИУ8-34.\r\n ЛАБ ДМК№6 - Возведение в степень по модулю"
	};
	const char* descriptions[4] = {
		"НОД(a,b), где a, b - беззнаковые 32-битные числа",
		"НОД(a,b), где a, b - беззнаковые 32-битные числа",
		"a*b (mod c), где a, b, c - беззнаковые 32-битные числа",
		"a^b (mod c), где a, b, c - беззнаковые 32-битные числа и c<65535"
	};

	const char *default_in = "in.txt", *default_out = "out.txt";
	FILE *in = NULL, *out = NULL;
	setlocale(LC_ALL, "Russian");
	if (!TO_CONSOLE){
		if (argc<min_num_args)
			fail("Не хватает аргументов. Usage: <path.exe> <type_of_algorithm=0-2> [path_to_in=in.txt]");
		//so, there are at least 3 args
		if (!sscanf(argv[1], "%d", &numALG))
			fail("Не удалось получить индекс алгоритма (0-2)");
		if (numALG<0 || numALG>3)
			fail("Неверный индекс алгоритма. ( idx<0 or idx>3 )");
		//if(argc<min_num_args+1&&numALG==2)
		//fail("Для выбранного алгоритма не хватает данных.");
		//so, there are enough params.
		in = safe_open(argc>2 ? argv[2] : default_in, default_in, "r"),
		out = safe_open(argc>3 ? argv[3] : default_out, default_out, "w");
		//в ине - число? или аж два? - Необходимое кол-во для работы
		input_from_file(in, &a, &b, numALG >= 2 ? NULL : &c);
	}
	else {
		char inbuf[100];
		printf("ЛАБ№6. Вот список всех поддерживаемых алгоритмов:\r\n");
		const int offset = strlen("Коряковцев Алишер. ИУВ-34.\r\n ЛАБ ДМК№6 - ");
		//покажем названия алгоритмов
		for (int i = 0; i < 4; ++i)
			printf("%d - %s\r\n", i, headers[i] + offset);
		//пока пишешь фигню - пиши еще!
		do {
			std::cout << "Введите индекс алгоритма: " << std::endl;
			std::cin >> numALG;
		} while (numALG<0 || numALG>3);
		//покажем описания вводимых параметров
		std::cout << descriptions[numALG] << std::endl;
		//вводим число
		std::cout << "Введите число a: " << std::endl;
		std::cin >> std::setw(100) >> inbuf;
		a = safe_input_int(inbuf);
		std::cout << "Введите число b: " << std::endl;
		std::cin >> std::setw(100) >> inbuf;
		b = safe_input_int(inbuf);
		if (numALG >= 2){
			std::cout << "Введите число c: " << std::endl;
			std::cin >> std::setw(100) >> inbuf;
			c = safe_input_int(inbuf);
		}


	}

	switch (numALG){
	case 0:
		euclid_binary(a, b, &buf);
		break;
	case 1:
		euclid_expanded(a, b, &buf);
		break;
	case 2:
		mod_multiply(a, b, c, &buf);
		break;
	case 3:
		mod_pow(a, b, c, &buf);
		break;
	default:
		fail("Непредвиденная ошибка. С памятью играешься, паразит? :)");
		break;
	}

	fprintf(TO_CONSOLE ? stdout : out, "%s\r\n\r\n%s", headers[numALG], buf);
	//free all memory
	free(buf);
	if (in)
		fclose(in);
	if (out)
		fclose(out);
	getch();
	return 0;
}