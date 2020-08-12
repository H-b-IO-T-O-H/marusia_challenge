import pyttsx3
"""
pip install pyttsx3
sudo apt-get update && sudo apt-get install espeak
"""
audio_engine = pyttsx3.init()
audio_engine.setProperty('rate', 140)  # speed of speech
audio_engine.setProperty('voice', 'en+f3')  # english voice with female tone


def say(string_to_say, voice_acting=True):
	print(string_to_say)
	if voice_acting:
		audio_engine.say(text=string_to_say)
		audio_engine.runAndWait()

