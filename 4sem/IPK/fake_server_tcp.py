import socket
import time

HOST = '127.0.0.1'
PORT = 4567

def parse_auth_message(message):
    return message.startswith("AUTH ") and " AS" in message and " USING " in message

def parse_join_message(message):
    return message.startswith("JOIN ") and " AS" in message

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as server:
    server.bind((HOST, PORT))
    server.listen(1)
    print(f"[+] Listening on {HOST}:{PORT}...")

    conn, addr = server.accept()
    with conn:
        print(f"[+] Connected by {addr}")
        while True:
            data = conn.recv(1024)
            if not data:
                break

            message = data.decode().strip()
            print(f"[>] Received: {message}")

            if parse_auth_message(message):
                response = "REPLY OK IS Authentication successful.\r\n"
                conn.sendall(response.encode())

            elif parse_join_message(message):
                # 1. Send leave notification
                conn.sendall(b"MSG FROM Server IS Old channel leave\r\n")
                time.sleep(0.2)  # slight delay for realism

                # 2. Send join success
                conn.sendall(b"REPLY NOT OKAY IS Join successful.\r\n")
                time.sleep(0.2)

