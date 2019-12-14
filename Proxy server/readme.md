
#Proxy server 
program contains three .cpp files
1. server.cpp
2. ps.cpp
3. client.cpp
-------------------------------------------------------------------------------------------------------------------------------
Working
(user can only input in client's terminal)
client will request for a file to proxy server, if proxy server has the requested file then it will fulfill the request of the client else it(proxy server) will make request to main server which is server.cpp for the same file, if the main server has the requested file(file requested by proxy server(ps.cpp)) then it will send file to proxy server and proxy server will send this file to client meanwhile proxy server also maintains a cache of 5 files. If the cache is full and a new file is requested then the least recently used file is removed from the proxy server's folder and the cache. If you want to stop the proxy server code then type 'stop' without an inverted comma in the client's terminal and all the cached files will be deleted automatically. 
-------------------------------------------------------------------------------------------------------------------------------

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
