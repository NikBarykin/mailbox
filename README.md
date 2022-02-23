# Mailbox
### Description
This project implements socket-based applications for sending and receiving emails (server, client and tests).
### Project structure
Project consists of three directories: `server`, `client` and `common`. The last one contains functionality that is used by both server and client. Each of these directories divides into subdirectories `source` and `test`, `test` isn't used in application and contains unit-tests for corresponding `source`. 
### Compilation
The easiest way to build the project is to use Cmake, but if you want to compile applications manually you can use following commands (assume you're using gcc compiler):  
server - `g++ --std=c++2a -I . server\source\* common\source\* -o server.exe -lws2_32 -lwsock32`           
client - `g++ --std=c++2a -I . client\source\* common\source\* -o client.exe -lws2_32 -lwsock32`  

###### Note
Project requires windows socket library (ws2_32, wsock32).
### Usage example
server
```
> server.exe 8080
```

client A
```
> client.exe localhost 8080
> Query name:
> Authorize
> Login:
A
> Password:
A
> 
> Authorized successfully
> 
> Query name:
GetMail
> 
> 1.
> Date: 23.02.2022
> From: B
> Hey, A!
> 
> Query name:
SendLetter
> Letter addressee:
B
> Letter body: (empty line marks end of letter)
Hey, B!

> 
> Letter sent successfully
> 
> Query name:
> SendLetter
> Letter addressee:
B
> Letter body: (empty line marks end of letter)
How are you?

> 
> Letter sent successfully
> 
> Query name:
Terminate
>  
> Session terminated
>
```

client B
```
> client.exe localhost 8080
> Query name:
> Authorize
> Login:
B
> Password:
B
> 
> Authorized successfully
> 
> Query name:
SendLetter
> Letter addressee:
A
> Letter body: (empty line marks end of letter)
Hey, A!

> 
> Letter sent successfully
> 
> Query name:
GetMail
> 
> 1.
> Date: 23.02.2022
> From: A
> Hey, B!
> 2.
> Date: 23.02.2022
> From: A
> How are you?
> 
> Query name:
GetMailFiltered
> Letter filter:
("you" in body && from == "A") || date < 20.01.2022
> 
> 1.
> Date: 23.02.2022
> From: A
> How are you?
> 
> Query name:
Terminate
> 
> Session terminated
> 
```
