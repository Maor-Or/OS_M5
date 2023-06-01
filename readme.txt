Task readme.txt file:

system enviroment: ubuntu 22.04

first, inorder to run the files, open a terminal window, for the main program.
Navigate in the window to the current folder containing the files
and run "make" or "make all" 

running the program:
running program: ./st_pipeline <N> (optional: )<seed>
while N is the amount of numbers, and seed is a random seed to genarate
example (1): ./st_pipeline 2
example (2): ./st_pipeline 2 123123123
afterwards, the clients connect to the server and the reactor object is collecting the clients' fds, 
and messages can be sent to the server from all client "in parallel".

To shut down the server/ any connected client - cntl+c

see ex_screenshot for example