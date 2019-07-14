import socket


BUFFER_SIZE = 1024


def main():
	quote = send_message()
	print(quote)


def send_message():
	
	s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	s.connect(('localhost', 6017))
	
	quit = False
	while not quit:
		tosend = input("Message to Send: ")
		if tosend == "q":
			break;
		s.send(tosend.encode("ascii"))
		data = s.recv(BUFFER_SIZE).decode("ascii")
		print("Recieved: " + data)
	s.close()
	return data

main()

