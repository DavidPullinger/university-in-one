import datetime
from hashlib import sha256
import sqlite3 as sl

# create database connection
# and enforce foreign key constraints
databaseCon = sl.connect("chatapp.db")
databaseCon.execute("PRAGMA foreign_keys = 1")

# methods used in protocol
def create_account(body):
    # deconstruct body of request
    username, passwordhash = (
        body.get("username"),
        sha256(body.get("password").encode()).hexdigest(),
    )
    # try insert user into database
    try:
        databaseCon.execute(f'INSERT INTO USER VALUES("{username}","{passwordhash}")')
        databaseCon.commit()
        return "ACCOUNT CREATED"
    except:
        databaseCon.rollback()
        return "USERNAME ALREADY EXISTS"


def login(body):
    # deconstruct body of request
    username, passwordhash = (
        body.get("username"),
        sha256(body.get("password").encode()).hexdigest(),
    )
    # check is username and password is valid using hash
    cursor = databaseCon.execute(f'SELECT * FROM USER WHERE username = "{username}"')
    record = cursor.fetchone()
    # if valid, get all users messages from db
    if record != None and record[1] == passwordhash:
        return update_messages({"username": username, "lastUpdate": "0"})
    else:
        return "INVALID PASSWORD OR USERNAME"


def update_messages(body):
    # deconstruct body of request
    username, lastUpdate = (body.get("username"), body.get("lastUpdate"))
    # select all NEW messages relevant to this user
    # a message is NEW if it has been received since the user last checked
    cursor = databaseCon.execute(
        f"""SELECT 'GROUP'.groupId,name,content,sender,sentTime from 'USERS IN GROUPS' 
        LEFT JOIN 'GROUP' ON 'USERS IN GROUPS'.groupId = 'GROUP'.groupId 
        LEFT JOIN MESSAGE ON 'USERS IN GROUPS'.groupId = MESSAGE.groupId 
        WHERE username="{username}" AND (sentTime >= "{lastUpdate}" OR sentTime IS NULL)
        ORDER BY name, sentTime ASC"""
    )
    # construct a "message" dictionary with various fields
    res = []
    for rec in cursor:
        message = {}
        message["groupId"] = rec[0]
        message["groupName"] = rec[1]
        message["content"] = rec[2]
        message["sender"] = rec[3]
        message["sentTime"] = rec[4]
        res.append(message)
    return res


def create_group(body):
    # deconstruct body of request
    name, participants = (body.get("name"), body.get("participants"))
    # insert group into GROUP table
    databaseCon.execute(f'INSERT INTO "GROUP" (name) VALUES ("{name}")')
    cursor = databaseCon.execute("SELECT last_insert_rowid()")
    # get id of added group
    groupId = cursor.fetchone()[0]
    # use group id to insert participants
    try:
        for p in participants:
            databaseCon.execute(
                f'INSERT INTO "USERS IN GROUPS" VALUES ({groupId},"{p}")'
            )
        databaseCon.commit()
        return groupId
    except:
        databaseCon.rollback()
        return "PARTICIPANTS NOT VALID"


def send_message(body):
    # deconstruct body of request
    groupId, sender, content = (
        body.get("groupId"),
        body.get("sender"),
        body.get("content"),
    )
    # insert message into MESSAGE TABLE
    try:
        databaseCon.execute(
            f"""INSERT INTO 'MESSAGE' (content, groupId, sender, sentTime)
                         VALUES ("{content}","{groupId}","{sender}","{datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S")}")"""
        )
        databaseCon.commit()
        return "MESSAGE SENT"
    except:
        databaseCon.rollback()
        return "MESSAGE NOT SENT"
