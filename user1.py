from client import User

user1 = User('Vanya', '127.0.0.1', 8888)
user1.connect(15)
user1.start_game('История')