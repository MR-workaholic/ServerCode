#!/usr/bin/env python
import socket
import time

ip_port = ('127.0.0.1', 54321)

if __name__ == "__main__":
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    s.bind(ip_port)
    s.listen(5)
    client, address = s.accept()
    while True:
        data = client.recv(1024)
        if(data):
	    print (data)
	    time.sleep(2)
            client.send(data)
        else:
            break
    client.close()
