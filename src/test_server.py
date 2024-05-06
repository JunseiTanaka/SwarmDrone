import requests

ESP32_IP = "172.20.10.9"

def send_message_and_print_response(message):
    url = f"http://{ESP32_IP}/"
    response = requests.post(url, data=message)
    print("Response from ESP32:")
    print(response.text)

if __name__ == "__main__":
    message = input("Send message to ESP32: ")
    send_message_and_print_response(message)

