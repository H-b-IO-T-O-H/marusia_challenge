/*
 Маруся хочет научиться играть в морской бой. Помоги ей разобраться с механикой игры и верно реагировать на ходы противника.
 Дано поле для игры в морской бой [9×9]. Самая первая строка — числовое обозначение столбцов. Первый символ на каждой строке — её буквенное обозначение [A-I],
 а дальше через пробел идет её содержимое:
 _ — пустая клетка.
 x — палуба корабля.
 После этого на вход подается 10 обозначений клеток, каждое на отдельной строке. В ответ ожидается результат в виде: «Попал!» или «Промах!».
*/

#include "task2.h"
using std::string;
using std::array;
using std::cin;
using std::cout;

static inline bool allowed_characters(const char &a) {
	return a == ' ' || a == 'x' || a == '_' || a >= 'A' && a <= 'I' || a >= '1' && a <= '9';
}

void recognize_str(array<char, MAP_SIZE> &str_in_map_array) {
	string str;
	int it = 0;
	
	getline(cin, str);
	for (char &c: str) {
		if (!allowed_characters(c))
			catch_error("An unexpected symbol was encountered in map!", INVALID_MAP);
		if (c == 'x' || c == '_')
			str_in_map_array[it++] = c;
	}
}

void catch_error(const string &error_msg, const short &exit_code) {
	cout << error_msg;
	exit(exit_code);
}

void fill_map(array<array<char, MAP_SIZE>, MAP_SIZE> &map) {
	string temp;
	
	getline(cin, temp);
	for (int i = 0; i < MAP_SIZE; ++i)
		recognize_str(map[i]);
}

void get_shots_coordinates(array<string, SHOT_CNT> &shots_coordinates) {
	for (int i = 0; i < SHOT_CNT; ++i)
		cin >> shots_coordinates[i];
}

void get_and_check_indexes_from_str(const string &src, int &i, int &j) {
	static string index_vertical = "ABCDEFGHI";
	
	if (src.size() != 2 || !(src[0] >= 'A' && src[0] <= 'I' && src[1] >= '1' && src[1] <= '9'))
		catch_error("Error in shots coordinates. Allowed format 'A1'", INVALID_COORD);
	i = index_vertical.find(src[0]);
	j = src[1] - '0' - 1;
}

void check_shots_on_map(array<array<char, MAP_SIZE>, MAP_SIZE> &map, array<string, SHOT_CNT> &shots_coordinates) {
	int i, j;
	
	for (int k = 0; k < SHOT_CNT; ++k) {
		get_and_check_indexes_from_str(shots_coordinates[k], i, j);
		if (map[i][j] == 'x')
			cout << "Попал!";
		else
			cout << "Промах!";
		cout << "\n";
	}
}

void get_shoot_status() {
	array<array<char, MAP_SIZE>, MAP_SIZE> map {};
	array<string, SHOT_CNT> shots_coordinates;
	
	fill_map(map);
	get_shots_coordinates(shots_coordinates);
	check_shots_on_map(map, shots_coordinates);
}

int main() {
	get_shoot_status();
	return 0;
}