import socket
import time
import json
from audio import say


class ClientError(Exception):
	pass


class ClientSocketError(ClientError):
	pass


class ClientProtocolError(ClientError):
	pass


class Client:

	def __init__(self, host, port, timeout=None):
		try:
			self._sock = socket.create_connection((host, port), timeout=timeout)
		except socket.error as exc:
			raise ClientSocketError('Error in create connection!', exc)
		self.user_template = {
			'REQUEST_TYPE': '',
			'USER_NAME': '',
			'ROOM': '',
			'ROOM_IS_FULL': False,
			'METRIC_TYPE': '',
			'METRIC_CONTENT': '',
		}

	def __del__(self):
		try:
			self._sock.close()
		except socket.error as exc:
			raise ClientSocketError('Error in socket close!', exc)
		except AttributeError:
			print("Сan't contact with server")

	def get_server_answer(self):
		data = b''
		while not data.endswith(b'\n\n'):
			try:
				data += self._sock.recv(1024)
			except socket.error as exc:
				raise ClientSocketError('Error in recv data!', exc)
		return json.loads(data.decode('utf-8').strip('\n'))

	def put(self, room, user_name, metric_type, metric_content):
		self.user_template['REQUEST_TYPE'] = 'put'
		self.user_template['USER_NAME'] = user_name
		self.user_template['METRIC_TYPE'] = metric_type
		self.user_template['METRIC_CONTENT'] = metric_content
		self.user_template['ROOM'] = room
		request = json.dumps(self.user_template)
		try:
			self._sock.sendall(request.encode('utf-8'))
			return self.get_server_answer()
		except socket.error as exc:
			raise ClientSocketError('Error in send!', exc)

	def get(self, room, room_is_full, user_name, metric_type, metric_content):
		self.user_template['REQUEST_TYPE'] = 'get'
		self.user_template['USER_NAME'] = user_name
		self.user_template['METRIC_TYPE'] = metric_type
		self.user_template['METRIC_CONTENT'] = metric_content
		self.user_template['ROOM'] = room
		self.user_template['ROOM_IS_FULL'] = room_is_full
		request = json.dumps(self.user_template)
		try:
			self._sock.sendall(request.encode('utf-8'))
			return self.get_server_answer()
		except socket.error as exc:
			raise ClientSocketError('Error in send!', exc)


class User:
	achievements = {}
	quests_already_answered = []
	quests_with_incorrect_answers = []

	def __init__(self, name, host, port):
		self.client = None
		self.timeout = 0
		self.host = host
		self.port = port
		self.name = name

	@staticmethod
	def catch_error(msg):
		print(msg)
		exit(1)

	def connect(self, timeout=10, voice_acting=True):
		self.timeout = timeout
		try:
			self.client = Client(host=self.host, port=self.port, timeout=timeout)
		except ClientError:
			self.catch_error('Something went wrong. Try again')
		say('Connection was made successfully', voice_acting)

	def start_game(self, quest_category, voice_acting=True):
		room_status = False
		user_place = -1
		room = ''
		while not room_status:
			server_data = self.client.get(room, room_status, self.name, 'QUEST_CATEGORY', quest_category)
			if server_data['STATUS'] != 'ok':
				self.catch_error('Something went wrong. Try again')
			room_status = server_data['ROOM_IS_FULL']
			room = server_data['ROOM']
			say('Waiting another players...', voice_acting)
			time.sleep(1)
		quest_list = server_data['QUESTIONS']
		if not quest_list:
			say(f"I can't find questions for selected category :('\
					You can choose from these:{server_data['EXISTING_CATEGORIES']}", voice_acting)
			self.start_game(input())
			exit(0)
		answers_info = self.get_and_check_user_answers(quest_list, voice_acting)
		while user_place == -1:
			server_data = self.client.put(server_data['ROOM'], self.name, 'USER_ANSWER', answers_info)
			if server_data['STATUS'] != 'ok':
				self.catch_error('Something went wrong. Try again')
			user_place = server_data['PLACE_IN_GAME']
			say('Waiting another players...', voice_acting)
			time.sleep(3)
		status = 'ko'
		end_count = 3  # 3 attempts to close the connection with server
		while status != 'ok' and end_count:
			server_data = self.client.put(server_data['ROOM'], self.name, 'SESSION_END', quest_category)
			status = server_data["STATUS"]
			end_count -= 1
			time.sleep(1)
		say(f'You took {user_place} place!', voice_acting)

	@staticmethod
	def get_and_check_user_answers(quest_list, voice_acting=True):
		ans_total_time = 0
		ans_correct = 0
		for question in quest_list:
			say(question, voice_acting)
			ans_start = time.time()
			if input() != quest_list[question][0]:
				say(f'It seems you\'re mistaken :( But you will learn something new and that\'s cool! '
					f'Correct answer: {quest_list[question][1]}', voice_acting)
			else:
				ans_correct += 1
				say('Absolutely true! Well done!', voice_acting)
			ans_total_time += round(time.time() - ans_start, 2)
		return ans_correct, ans_total_time

