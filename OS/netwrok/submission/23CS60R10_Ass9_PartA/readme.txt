1. Structure of the program :
a) client.c -> contain the code for the client
b) server.c -> contain the code for the server

2. How to run the program :
a) Compile the server.c using gcc [gcc server.c -o server -lpthread]
b) Compile the client.c using gcc [gcc client.c -o client]
c) Run the server using ./server

(Now the above 3 steps are done for you . So just run the ./run.sh file)

3. Run the client file using proper flags and arguments 
[./client <server_ip> <server_port> <num_requests> <interval>]
For the above command to work you need to pass all the arguments and the requests are sent at an interval
of <interval> seconds to the server.

4. Note : The server continues to run. You can run the client multiple times to send multiple requests to the server from different terminals.

5. The log are stored at the ping_log.txt file for the server side and the client side RTT are printed on the terminal.

6. The server is multithreaded and can handle multiple clients at the same time and The server is also capable of handling multiple requests from the same client at the same time.


Example :
./run.sh
./client 127.0.0.1 5566 3 4
