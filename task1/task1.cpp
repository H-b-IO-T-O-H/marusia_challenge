/*
 Когда мы были детьми, то часто играли в «Крестики-нолики». Голосовые помощники тоже хотят научиться этой забаве. Помоги Марусе, которая никогда раньше не играла в «Крестики-нолики», выявить победителя. В поле для игры [N x N] определи победителя или ничью.
Входные данные: на первой строке задается размер игрового поля N. На последующих N строках задаются строки, каждая из которых содержит N значений, разделенных пробелом.
Множество значений:
x — клетка занята крестиком
0 — клетка занята ноликом
. — клетка не занята
Выходные данные:
x — победили крестики (икс)
0 — победили нолики (ноль)
x0 — ничья или результат ещё нельзя определить (икс ноль).
Ограничения: N — целое число на отрезке [0; 30]
 */

#include "task1.h"

using std::string;
using std::cin;
using std::cout;

static inline bool allowed_characters(const char &a) {
	return a == 'O' || a == 'x' || a == '.' || a == ' ';
}

string recognize_str() {
	string str, str_no_spaces;
	
	getline(cin, str);
	for (char & i : str) {
		if (!allowed_characters(i))
			catch_error("An unexpected symbol was encountered in map! Allowed format: 'Ox.'", INVALID_MAP);
		if (i != ' ')
			str_no_spaces += i;
	}
	return str_no_spaces;
}

void catch_error(const string &error_msg, const short &exit_code) {
	cout << error_msg;
	exit(exit_code);
}

short get_map_size() {
	short size = 0;
	string format;

	(cin >> format).get();
	if ((size = format.length()) <= 1 || size > 3 || format[0] != 'a')
		catch_error("Invalid map size got! Allowed size: [0:30], format: 'aSIZE'", INVALID_MAP);
	size = atoi(format.c_str() + 1);
	if (size < 0 || size > 30)
		catch_error("Invalid map size got! Allowed size: [0:30]", INVALID_MAP);
	return size;
}

void fill_map(string *map, short size) {
	for (auto i = 0; i < size; ++i)
		map[i] = recognize_str();
}

bool check_horizontal(const string *map, const short &size, const char &victory_contender) {
	char find_character = 'x';
	
	if (victory_contender == 'x')
		find_character = 'O';
	for (auto i = 0; i < size; ++i) {
		if (map[i].find(find_character) == -1 && map[i].find('.') == -1)
			return true;
	}
	return false;
}

short vertical_find(const string *map, const short &index, const short &size, const char &character_to_find) {
	for (auto i = 0; i < size; ++i)
		if (map[i][index] == character_to_find)
			return i;
	return -1;
}

short diagonal_find(const string *map, short index, const short &size, const char &character_to_find) {
	char oper = '+';  // for main diagonal
	
	if (index == size) {
		oper = '-';  // for side diagonal
		index--;
	}
	for (auto i = 0; i < size; ++i) {
		string buf = map[i];
		if (buf[index] == character_to_find)
			return i;
		oper == '+' ? index++ : index--;
	}
	return -1;
}

bool check_vertical(const string *map, const short &size, const char &victory_contender) {
	char find_character = 'x';
	
	if (victory_contender == 'x')
		find_character = 'O';
	for (auto i = 0; i < size; ++i) {
		if (vertical_find(map, i, size, find_character) == -1 && vertical_find(map, i, size, '.') == -1)
			return true;
	}
	return false;
}

bool check_main_diagonal(const string *map, const short &size, const char &victory_contender) {
	if (victory_contender == 'O')
		return diagonal_find(map, 0, size, 'x') == -1 && diagonal_find(map, 0, size, '.') == -1;
	return diagonal_find(map, 0, size, 'O') == -1 && diagonal_find(map, 0, size, '.') == -1;
}

bool check_side_diagonal(const string *map, const short &size, const char &victory_contender) {
	if (victory_contender == 'O')
		return diagonal_find(map, size, size, 'x') == -1 && diagonal_find(map, size, size, '.') == -1;
	return diagonal_find(map, size, size, 'O') == -1 && diagonal_find(map, size, size, '.') == -1;
}

bool win_check(const string *map, const short &size, const char &victory_contender) {
	return check_main_diagonal(map, size, victory_contender) || check_side_diagonal(map, size, victory_contender) ||
		   check_horizontal(map, size, victory_contender) || check_vertical(map, size, victory_contender);
}

int get_game_verdict() {
	const short size = get_map_size();
	bool victory_verdict;
	int party = XO_DRAW;
	string map[size];
	
	fill_map(map, size);
	victory_verdict = win_check(map, size, 'x');
	if (victory_verdict)
		party = X_WIN;
	victory_verdict = win_check(map, size, 'O');
	if (victory_verdict) {
		if (party == XO_DRAW)
			return O_WIN;
		else
			return XO_DRAW;
	}
	return party;
}

void print_map(const string *map, const short &size) {
	for (int i = 0; i < size; ++i)
		cout << map[i] << '\n';
}

void print_game_verdict(const int &verdict) {
	if (verdict == X_WIN)
		cout << 'x';
	else if (verdict == O_WIN)
		cout << 'O';
	else
		cout << "xO";
}

int main() {
	print_game_verdict(get_game_verdict());
	return 0;
}