import socket
import json
import requests


BUFFER_SIZE = 1024


def main():
	s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	s.bind(('localhost',6017)) 
	s.listen(1)
	while True:
		conn, addr = s.accept()
		print("Dumping Quote")
		conn.send(get_haiku().encode("ascii"))  # echo
		conn.close()

def get_haiku():
	with requests.get("https://randomhaiku.com") as r:
		chunks = r.text.split("<line>")
		chunks = chunks[1:4]
		original_art = ""
		for chunk in chunks:
			original_art += chunk.split("</line>")[0] + "\n"
		return original_art[:-1]
main()

