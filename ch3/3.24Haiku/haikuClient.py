import socket


BUFFER_SIZE = 1024


def main():
	port = 6017
	quote = client_get_haiku(port)
	print(quote)



def client_get_haiku(port):
	
	s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	s.connect(('localhost', port))
	data = s.recv(BUFFER_SIZE)
	s.close()
	return data

main()

