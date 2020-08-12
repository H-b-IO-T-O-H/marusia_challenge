/*
  Помоги Марусе вывести Дашу Путешественницу из подземелья. Дано квадратное поле [N x N] заданной величины (N находится в диапазоне [10; 100]).
 Поле — подземелье с коридором, который ведет к выходу. Даша находится в начале коридора, а голосовой помощник помогает ей выбраться из подземелья.
 Выход находится в конце коридора на границе карты.
 На первой строке программа принимает на вход размер подземелья — N, затем представление самого подземелья через пробел:
 _ — стены подземелья.
 х — местонахождение Даши.
 . — продолжение коридора.
 Указания — следующие слова:
 Наверх!
 Направо!
*/
#include "task3.h"

using std::string;
using std::vector;
using std::cin;
using std::cout;

void catch_error(const string &error_msg, const short &exit_code) {
	cout << error_msg;
	exit(exit_code);
}

static inline bool allowed_characters(const char &a) {
	return a == '_' || a == 'X' || a == '.' || a == ' ';
}

string recognize_str(int &start_pos) {
	string str, str_no_spaces;
	
	getline(cin, str);
	for (char & i : str) {
		if (!allowed_characters(i))
			catch_error("An unexpected symbol was encountered in map! Allowed format: '_X.'", INVALID_MAP);
		if (i != ' ')
			str_no_spaces += i;
		if (i == 'X')
			start_pos = int(str_no_spaces.size()) - 1;
	}
	return str_no_spaces;
}

int get_map_size() {
	int size = 0;
	
	(cin >> size).get();
	if (size < MAP_MIN_SIZE || size > MAP_MAX_SIZE)
		catch_error("MAP size out of range. Allowed range: [10;100]\n", INVALID_MAP);
	return size;
}

void fill_map(vector<string> &map, const int &size, int &start_pos_i, int &start_pos_j) {
	bool was_updated = false;
	
	for (auto i = 0; i < size; ++i) {
		map[i] = recognize_str(start_pos_j);
		if (start_pos_j != -1 && !was_updated) { // определение в цикле заполнения карты стартовой позиции "Даши"
			start_pos_i = i;
			was_updated = true;
		}
	}
}

int count_directions(const vector<string> &map, const int &size, const int &i, const int &j) {
	return (i > 0 && map[i - 1][j] == '.') + (i < size - 1 && map[i + 1][j] == '.') +
		   (j > 0 && map[i][j - 1] == '.') + (j < size - 1 && map[i][j + 1] == '.');
		
}

bool clear_dead_end(vector<int> &commands) { // удаление маршрутов, ведущих в тупик
	while (!commands.empty()) {
		commands.pop_back();
		if (commands.back() == PATH_SEPARATOR) {
			commands.pop_back();
			break;
		}
	}
	return false;
}

void print_map(const vector<string> &map, const int &size) {
	for (int i = 0; i < size; ++i) {
		for (int j = 0; j < size; ++j)
			cout << map[i][j] << ' ';
		cout << '\n';
	}
}

bool find_next_point(vector<string> &map, vector<int> &commands, const int &size, int i, int j) {
	/*
	 Первый успешный маршрут возвращается в виде команд. Маршрут может быть не самым оптимальным, зато отстуствует полный перебор карты. Это, возможно,
	 будет предпочтительнее на больших картах с петлевыми и сложными разветленными путями. Однако можно подобрать карту, которая заставит алгоритм
	 просмотреть все пути. Решение с использованием рекурсии применимо в рамках данной задачи, так как размеры карты ограничены, и гарантировано не будет
	 переполнения стека. Тем не менее, для больших карт алгоритм следует реализовывать на графах, где, кстати, можно будет определять кратчайших маршрут.
	 */
	int temp;
	static bool path_was_found = false;
	
	map[i][j] = 'v'; //visited
	if (i == 0 || i == size - 1 || j == 0 || j == size - 1)
		return (path_was_found = true);
	if ((temp = count_directions(map, size, i, j)) == 0)
		return clear_dead_end(commands);
	if (temp > 1)
		for (int k = 1; k < temp; ++k)
			commands.push_back(PATH_SEPARATOR);
	if (!path_was_found && i > 0 && map[i - 1][j] == '.') {
		commands.push_back(UP);
		find_next_point(map, commands, size, i - 1, j);
	}
	if (!path_was_found && i < size - 1 && map[i + 1][j] == '.') {
		commands.push_back(DOWN);
		find_next_point(map, commands, size, i + 1, j);
	}
	if (!path_was_found && j > 0 && map[i][j - 1] == '.') {
		commands.push_back(LEFT);
		find_next_point(map, commands, size, i, j - 1);
	}
	if (!path_was_found && j < size - 1 && map[i][j + 1] == '.') {
		commands.push_back(RIGHT);
		find_next_point(map, commands, size, i, j + 1);
	}
	return path_was_found;
}

void print_commands(const vector<int> &commands) {
	static string commands_str[] = {"ВВЕРХ!", "ВНИЗ!", "НАЛЕВО!", "НАПРАВО!"}; // Выходные данные в задании не соотвествуют указанным в условии !!!
	for (int command : commands)
		if (command != PATH_SEPARATOR)
			cout << commands_str[command] << '\n';
}

vector<int> recognize_map() {
	const int size = get_map_size();
	vector<int> commands;
	vector<string> map(size);
	int start_pos_i = -1, start_pos_j = -1;
	
	fill_map(map, size, start_pos_i, start_pos_j);
	if (!find_next_point(map, commands, size, start_pos_i, start_pos_j))
		commands.clear();
	return commands;
}

int main() {
	print_commands(recognize_map());
	return 0;
}