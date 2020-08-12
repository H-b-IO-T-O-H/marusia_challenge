from client import User

user1 = User('Petya', '127.0.0.1', 8888)
user1.connect(15, voice_acting=False)
user1.start_game('Physics', voice_acting=False)
