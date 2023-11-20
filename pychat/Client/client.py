# CLIENT
# definitions for functions regarding protocol
# functions to be called by the ui

from socket import *
import json
import hashlib

myhash = hashlib.sha256()
serverName = "localhost"
serverPort = 12000
clientSocket = socket(AF_INET, SOCK_DGRAM)
# userinput = input("Input lowercase sentence (or 'q' to quit): ");

# -----------------------------------------
def reqCREATE_ACC(uname, pword):
    create_acc = {"username": uname, "password": pword}
    return "CREATE_ACC", json.dumps(create_acc)


def resCREATE_ACC(body):
    if body == "ACCOUNT CREATED":
        return True
    elif body == "USERNAME ALREADY EXISTS":
        return False


# ------------------------------------------


def reqLOGIN(uname, pword):
    login = {"username": uname, "password": pword}
    return "LOGIN", json.dumps(login)


def resLOGIN(body):
    if body == "INVALID PASSWORD OR USERNAME":
        return "False"
    else:
        # msglist = json.loads(body)
        return body


# -------------------------------------------
def reqUPDATE_MSGS(uname, lastudate):
    update_msgs = {"username": uname, "lastUpdate": lastudate}
    return "UPDATE_MSGS", json.dumps(update_msgs)


def resUPDATE_MSGS(body):
    return body


# -----------------------------------------
def reqCREATE_GROUP(name, participants):
    create_group = {"name": name, "participants": participants}
    return "CREATE_GROUP", json.dumps(create_group)


def resCREATE_GROUP(body):
    if body == "PARTICIPANTS NOT VALID":
        return "False"
    else:
        return body


# -----------------------------------------
def reqSEND_MSG(groupID, sender, content):
    send_msg = {"groupId": groupID, "sender": sender, "content": content}
    return "SEND_MSG", json.dumps(send_msg)


def resSEND_MSG(body):
    if body == "MESSAGE SENT":
        return "True"
    else:
        return "False"


# ---------------------------------------

# DECODING RESPONSE
def DECODE_RESPONSE(action, response):
    jsonbody = json.loads(response.decode())
    # assuming invalid messages also have to be decoded
    if action == "CREATE_ACC":
        return resCREATE_ACC(jsonbody)
    elif action == "LOGIN":
        return resLOGIN(jsonbody)
    elif action == "UPDATE_MSGS":
        return resUPDATE_MSGS(jsonbody)
    elif action == "CREATE_GROUP":
        return resCREATE_GROUP(jsonbody)
    elif action == "SEND_MSG":
        return resSEND_MSG(jsonbody)
    else:
        return "not a real action"


# probably have loop in UI
def send(action, body):
    # SENDING MESSAGE------------------
    hashvalue = hashlib.sha256((action + body).encode()).hexdigest()
    message = "{//" + action + ";" + hashvalue + "//}" + body
    clientSocket.sendto(message.encode(), (serverName, serverPort))  # send message
    # --------------------------------


def receive(command):
    modifiedMessage, serverAddress = clientSocket.recvfrom(10 * 1024)  # receive message
    return DECODE_RESPONSE(command, modifiedMessage)
