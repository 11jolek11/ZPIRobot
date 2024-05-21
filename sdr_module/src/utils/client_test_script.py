import os
import socket
import json
import argparse
from pathlib import Path

# python client_test_script.py '{"command": "get_rssi"}'
default_socket_path = "/tmp/mysocket"

parser = argparse.ArgumentParser()

parser.add_argument("-r", "--repeat", type=int, action="store", default=1)
parser.add_argument("-s", "--socket", type=str, action="store", dest="socket_path", default=default_socket_path)
parser.add_argument("msg", type=str, action="store")

args = parser.parse_args()

try:
    args.msg = json.loads(args.msg)
except json.decoder.JSONDecodeError:
    print("JSON error")
    exit(1)

args.socket_path = Path(args.socket_path)

if not args.socket_path.is_socket():
    print("Bad socket file.")
    print("Is server running?")
    print("Exiting ...")
    exit(1)

client = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)

client.connect(str(args.socket_path))

for counter in range(args.repeat):
    print("SEND -> {}".format(args.msg))
    client.sendall(json.dumps(args.msg).encode())

response = client.recv(1024)
print("Response " + response.decode())
client.close()

