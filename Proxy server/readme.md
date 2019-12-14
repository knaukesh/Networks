#Proxy server 
program contains three .cpp files
1. server.cpp
2. ps.cpp
3. client.cpp

How to run this proxy server
1. place all these three files in different locations.
2. open a terminal at all respective locations and type the following command sequentially.
3. for server.cpp run command 
	a. g++ server.cpp
	b. ./a.out 10000
4. for ps.cpp run command
	a. g++ ps.cpp
	b. ./a.out 9899 10000
5. for client.cpp
	a. g++ client.cpp
	b. ./a.out 9899
6. maintain the order 3. 4. 5. and you can change the port number of your choice.
-------------------------------------------------------------------------------------------------------------------------------
