# pyChat

## Description

This is a simple network application that can connect two or more users running seperate client applications through a single server.

The application uses UDP on the transport layer and we have created our own application layer protocol, as defined in the report

## Running the application

### Server

In the root directory of the server, run `python3 index.py` to start the server. It will output "The server is ready to receive".

### Client

In the root directory of the client, run `python3 chatUI.py` to start the client. It will create a GUI which you can interact with.

## Sizing screens

If any of the screens seems cut off, please edit the variables `HEIGHT` and `WIDTH` in `chatUI.py` (recommended values are 800 and 440 respectively). The intended look of the client application can be seen in the report. This sizing issue is due to the python library Tkinter having platform specific UI. (The application was tested on macOS)

The chat screen has been made resizable as sizing was known to be an issue on this screen after testing the app on various platforms
