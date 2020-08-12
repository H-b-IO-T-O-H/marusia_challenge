import asyncio
import json

MAX_PLAYERS_IN_ROOM = 2
tournament_table = {}
server_rooms = {}

quests_all = {
	'History': {
		'Year of abolition of serfdom': ['1861', 'You are mistaken. Explanation...'],
		'Last name of the first President of the Russian Federation': ['Yeltsin', 'You are mistaken. Explanation...']},
	'Physics': {
		'Does the boiling point of a liquid depend on the altitude above sea level?':
			[
				'Yes',
				'You are mistaken. The boiling point is directly proportional to the pressure of the atmosphere.'],
		'The first derivative of the time coordinate': ['Speed', 'Some explanation...']}}


def get_place(user_list, user_name):
	place = 1
	places = {
		1: 'first',
		2: 'second',
		3: 'third', }
	for user in user_list:
		if user_name == user[0]:
			break
		place += 1
	if place in places:
		return places[place]
	return place


def check_uniq(user_list, user_name):
	for user in user_list:
		if user[0] == user_name:
			return False
	return True


class ClientServerProtocol(asyncio.Protocol):

	def __init__(self):
		self.room_cnt = 0
		self.transport = None

	def connection_made(self, transport):
		self.transport = transport

	def data_received(self, data):
		resp = json.loads(data.decode())
		self.transport.write(str(json.dumps(self.parse_arguments(data=resp)) + '\n\n').encode())

	def parse_arguments(self, data):
		server_answer_template = {
			'STATUS': 'ok',
			'ROOM': '',
			'ROOM_IS_FULL': False,
			'QUESTIONS': {},
			'EXISTING_CATEGORIES': list(quests_all.keys()),
			'PLACE_IN_GAME': ''}
		try:
			request_type = data['REQUEST_TYPE']
			user_name = data['USER_NAME']
			room = data['ROOM']
			room_is_full = data['ROOM_IS_FULL']
			metric_type = data['METRIC_TYPE']
			metric_content = data['METRIC_CONTENT']
		except IndexError:
			server_answer_template['STATUS'] = 'ERROR\n\n'
			return server_answer_template
		if request_type == 'get':
			if metric_type == 'QUEST_CATEGORY':
				if not room or not room_is_full:
					room, room_is_full, questions = self.create_room(metric_content, user_name)
					server_answer_template['QUESTIONS'] = questions
					server_answer_template['ROOM_IS_FULL'] = room_is_full
				server_answer_template['ROOM'] = room
		if request_type == 'put':
			server_answer_template["ROOM"] = room
			if metric_type == 'USER_ANSWER':
				if room not in tournament_table:
					tournament_table[room] = []
				if check_uniq(tournament_table[room], user_name):
					tournament_table[room].append((user_name, metric_content))
					print(tournament_table)
					tournament_table[room] = sorted(tournament_table[room], key=lambda elem: (elem[1][0], -elem[1][1]),
													reverse=True)
				if len(tournament_table[room]) == MAX_PLAYERS_IN_ROOM:
					server_answer_template['PLACE_IN_GAME'] = get_place(tournament_table[room], user_name)
				else:
					server_answer_template['PLACE_IN_GAME'] = -1
			if metric_type == 'SESSION_END':
				if metric_content in server_rooms:
					if room in server_rooms[metric_content]:
						user_room = server_rooms[metric_content][room]
						if user_name in user_room:
							user_room.remove(user_name)
						if not user_room:
							tournament_table[room] = []
			print(f"Tournament table:\n{tournament_table}\nServer rooms:\n{server_rooms}")
		return server_answer_template

	def delete_session(self):
		pass

	def create_room(self, metric_content, user_name):
		room_is_full = False
		if metric_content in quests_all:
			questions = quests_all[metric_content]
			if metric_content in server_rooms:
				rooms_list = server_rooms[metric_content]
				for room in rooms_list:
					if len(rooms_list[room]) < MAX_PLAYERS_IN_ROOM:
						if user_name not in rooms_list[room]:
							rooms_list[room].append(user_name)
							if len(rooms_list[room]) == MAX_PLAYERS_IN_ROOM:
								room_is_full = True
						else:
							return -2, room_is_full, {}
						return room, room_is_full, questions
					elif user_name in rooms_list[room]:
						return room, True, questions
			server_rooms[metric_content] = {self.room_cnt: [user_name]}
			self.room_cnt += 1
			return self.room_cnt - 1, room_is_full, questions
		else:
			return -1, room_is_full, {}


def run_server(host, port):
	loop = asyncio.get_event_loop()
	coro = loop.create_server(
		ClientServerProtocol,
		host, port
	)

	server = loop.run_until_complete(coro)
	try:
		loop.run_forever()
	except KeyboardInterrupt:
		print('Server Stop!')
		pass
	server.close()
	loop.run_until_complete(server.wait_closed())
	loop.close()


run_server('127.0.0.1', 8888)
