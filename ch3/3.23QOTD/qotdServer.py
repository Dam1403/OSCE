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
		conn.send(get_qotd().encode("ascii"))  # echo
		conn.close()

def get_qotd():
	with requests.get("http://quotes.rest/qod.json") as r:
		return json.loads(r.text)["contents"]["quotes"][0]["quote"];

main()


