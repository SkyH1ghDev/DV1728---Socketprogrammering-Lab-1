CC_FLAGS = -Wall -g -I.
LD_FLAGS = -Wall -L./

HELPER_SOURCES = Helpers/Tokenizer.cpp


all: libcalc test client server

helpers.o: $(HELPER_SOURCES)
	$(CXX) $(CC_FLAGS) $(CFLAGS) -c $(HELPER_SOURCES) -o helpers.o

servermain.o: servermain.cpp
	$(CXX) $(CC_FLAGS) $(CFLAGS) -c servermain.cpp

clientmain.o: clientmain.cpp
	$(CXX) $(CC_FLAGS) $(CFLAGS) -c clientmain.cpp 

main.o: main.cpp
	$(CXX) $(CC_FLAGS) $(CFLAGS) -c main.cpp

test: main.o calcLib.o
	$(CXX) $(LD_FLAGS) -o test main.o -lcalc

client: clientmain.o calcLib.o helpers.o
	$(CXX) $(LD_FLAGS) -o client clientmain.o helpers.o -lcalc

server: servermain.o calcLib.o helpers.o
	$(CXX) $(LD_FLAGS) -o server servermain.o helpers.o -lcalc

calcLib.o: calcLib.c calcLib.h
	gcc -Wall -fPIC -c calcLib.c

libcalc: calcLib.o
	ar -rc libcalc.a -o calcLib.o

clean:
	rm *.o *.a test server client
