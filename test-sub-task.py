import socket
import time
import sys

if __name__ == '__main__':
    kernel_socket_port = int(sys.argv[1])
    self_socket_port = int(sys.argv[2])
    print("Received kernel port: %d." % kernel_socket_port)
    print("Database port: %d." % self_socket_port)
    print("Waitting 100 ms.")
    time.sleep(0.1)
    kernel_sock = socket.socket()
    kernel_sock.connect(('127.0.0.1', kernel_socket_port))
    kernel_sock.send(bytes([0, 1, 2, 1]) + 'PID:1000'.encode(encoding='ascii'))
    msg = kernel_sock.recv(256)
    print(msg.decode(encoding='ascii'))
    kernel_sock.close()

    time.sleep(2)
    kernel_sock = socket.socket()
    kernel_sock.connect(('127.0.0.1', kernel_socket_port))
    kernel_sock.send(bytes([0, 1, 2, 1]) + 'QUIT'.encode(encoding='ascii'))
    kernel_sock.close()