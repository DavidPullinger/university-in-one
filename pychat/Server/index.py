from socket import *
from protocol import *
from hashlib import sha256
import json


def no_such_func(body):
    return "ERROR: function not found"


def use_protocol(message):
    return {
        "CREATE_ACC": create_account,
        "LOGIN": login,
        "UPDATE_MSGS": update_messages,
        "CREATE_GROUP": create_group,
        "SEND_MSG": send_message,
    }.get(message, no_such_func)


def main():
    serverPort = 12000
    serverSocket = socket(AF_INET, SOCK_DGRAM)
    serverSocket.bind(("", serverPort))
    print("The server is ready to receive")
    while True:
        # get message from any user
        message, clientAddress = serverSocket.recvfrom(2048)
        # use protocol to decide what to do
        message = message.decode()
        # get header and body from user request
        header = message[message.find("{//") + 3 : message.rfind("//}")]
        ## find command and hash using header
        command, hash = header.split(";")
        ## find body
        body = message[message.rfind("//}") + 3 :]
        ## ensure message was not lost
        if hash == sha256((command + body).encode()).hexdigest():
            res = use_protocol(command)(json.loads(body))
        else:
            res = "MESSAGE INVALID"
        serverSocket.sendto(json.dumps(res).encode(), clientAddress)


if __name__ == "__main__":
    main()
