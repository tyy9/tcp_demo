Target=server
Target2=client
Src2=$(wildcard src/*.c)
MAIN_Src=bin/server_main.c
MAIN_Src2=bin/client_main.c
Lib=-L./lib
Lib_Object=-lpthread
CC=arm-linux-gcc
$(Target2) $(Target):$(Src2) $(MAIN_Src2)
	 gcc $(Src2) $(MAIN_Src2) -o $(Target2)  $(Lib_Object)
	$(CC) $(Src2) $(MAIN_Src) -o $(Target)  $(Lib_Object)
.PHONY:scp
scp:
	scp ${Target}  root@192.168.59.170:/root/user/socket_demo

.PHONY:clean
clean:
	rm ${Target} ${Target2} ${Target}
