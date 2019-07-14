import socket
import json
import requests


BUFFER_SIZE = 1024


def main():
	s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	s.bind(('localhost',6017)) 
	s.listen(1)
	conn, addr = s.accept()
	while True:
		data = conn.recv(BUFFER_SIZE)
		print("Recieved: " + data.decode("ascii"))
		conn.send(data)  # echo
	conn.close()

main()

