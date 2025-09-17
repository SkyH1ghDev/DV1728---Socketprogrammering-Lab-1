# __      __.__                           __    _________     _____          __        _________
#/  \    /  \  |__ ___.__.   ____   _____/  |_  \_   ___ \   /     \ _____  |  | __ ___\_____   \
#\   \/\/   /  |  <   |  |  /    \ /  _ \   __\ /    \  \/  /  \ /  \\__  \ |  |/ // __ \ /   __/
# \        /|   Y  \___  | |   |  (  <_> )  |   \     \____/    Y    \/ __ \|    <\  ___/|   |
#  \__/\  / |___|  / ____| |___|  /\____/|__|    \______  /\____|__  (____  /__|_ \\___  >___|
#       \/       \/\/           \/                      \/         \/     \/     \/    \/<___>



CC_FLAGS = -Wall -g -I.
LD_FLAGS = -Wall -L./

OBJECT_FILES = Tokenizer.o AddrInfo.o


all: libcalc test client server


# Custom files
Tokenizer.o: Helpers/Tokenizer.cpp
	$(CXX) $(CC_FLAGS) $(CFLAGS) -c -o Tokenizer.o Helpers/Tokenizer.cpp

AddrInfo.o: Helpers/AddrInfo.cpp
	$(CXX) $(CC_FLAGS) $(CFLAGS) -c -o AddrInfo.o Helpers/AddrInfo.cpp



servermain.o: servermain.cpp
	$(CXX) $(CC_FLAGS) $(CFLAGS) -c servermain.cpp

clientmain.o: clientmain.cpp
	$(CXX) $(CC_FLAGS) $(CFLAGS) -c clientmain.cpp

main.o: main.cpp
	$(CXX) $(CC_FLAGS) $(CFLAGS) -c main.cpp



test: main.o calcLib.o
	$(CXX) $(LD_FLAGS) -o test main.o -lcalc

client: clientmain.o calcLib.o $(OBJECT_FILES)
	$(CXX) $(LD_FLAGS) -o client clientmain.o $(OBJECT_FILES) -lcalc

server: servermain.o calcLib.o $(OBJECT_FILES)
	$(CXX) $(LD_FLAGS) -o server servermain.o $(OBJECT_FILES) -lcalc



calcLib.o: calcLib.c calcLib.h
	gcc -Wall -fPIC -c calcLib.c

libcalc: calcLib.o
	ar -rc libcalc.a -o calcLib.o

clean:
	rm *.o *.a test server client
