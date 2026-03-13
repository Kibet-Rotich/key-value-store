import socket


HOST = "127.0.0.1"
PORT = 8080
TEST_MESSAGES = [
	"hello from python\n",
	"SET name Alice\n",
	"GET name\n",
	"DEL name\n",
]


def run_case(message: str) -> None:
	with socket.create_connection((HOST, PORT), timeout=3) as client:
		welcome = client.recv(1024).decode("utf-8", errors="replace")
		client.sendall(message.encode("utf-8"))
		response = client.recv(1024).decode("utf-8", errors="replace")

	print("-" * 60)
	print(f"Sent:     {message.rstrip()}")
	print(f"Welcome:  {welcome.rstrip()}")
	print(f"Response: {response.rstrip()}")


def main() -> None:
	print(f"Testing server at {HOST}:{PORT}")
	for message in TEST_MESSAGES:
		run_case(message)


if __name__ == "__main__":
	main()
