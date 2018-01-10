# rndchat
A simple Random Chat TCP based program in C

## How it works?
A relay server is used to send messages from one to others clients. The server creates a TCP welcome socket, with the port number specified by the user, and another socket for each client that connects to the chat.
The maximum number of clients of the chat has to be specified in the code.
Each client has a turn to write, and another to read the other's clients messages. Each client can quit the chat sending the message `QUIT` when is his turn.

## Personalizations
* **Max message length**: in both `client.c` and `server.c` edit the line `#define MAXLINE 512` and set your preferred value. (Make sure that they're the same value!)
* **Max users per chat**: in `server.c` edit the line `#define MAXUSERS 3` and set your preferred value.
* **Color of the messages**: in `client.c` there's a set of colors predefined: <br />
`#define ANSI_COLOR_RED"\x1b[91m"`<br />
`#define ANSI_COLOR_GREEN   "\x1b[92m"`<br />
`#define ANSI_COLOR_YELLOW  "\x1b[93m"`<br />
`#define ANSI_COLOR_BLUE    "\x1b[94m"`<br />
`#define ANSI_COLOR_MAGENTA "\x1b[95m"`<br />
`#define ANSI_COLOR_CYAN    "\x1b[96m"`<br />
`#define ANSI_COLOR_RESET   "\x1b[0m"`<br />
Edit the following to suit your prefs: <br/>
`#define MY_MSG_COLOR 		ANSI_COLOR_BLUE`<br/>
`#define OTHERS_MSG_COLOR	ANSI_COLOR_GREEN`
