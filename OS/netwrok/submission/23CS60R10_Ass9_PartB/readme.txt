1. Structure of the program :
a) client.c -> contain the code for the client
b) server.c -> contain the code for the server

2. How to run the program :
a) Compile the server.c using gcc [gcc server.c -o server -lpthread]
b) Compile the client.c using gcc [gcc client.c -o client]
c) Run the server using ./server

(Now the above 3 steps are done for you . So just run the ./run.sh file)

3. Run the client file only
[./client]

4. Note : The server continues to run. You can run the client multiple times to send multiple requests to the server from different terminals.
5. The server is multi threaded. It can handle multiple clients at the same time.
6. The client opens a terminal ftp_client> where you can enter the commands.
7. The commands are :
a) get <filename> -> to download a file from the server
b) put <filename> -> to upload a file to the server
c) ls -> to list the files in the server
d) cd <directory> -> to change the directory in the server. 
e) exit -> to exit the client


Example :
./run.sh
./client 
