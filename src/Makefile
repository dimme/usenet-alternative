CXXFLAGS  = -pipe -O2 -Wall -W -ansi -pedantic-errors -DNDEBUG
CXXFLAGS += -Wmissing-braces -Wparentheses -Wold-style-cast
LIBS = clientserver/libclientserver.a
CXX = g++

.PHONY: all clean client server1 server2

all: client server1 server2

client: client.o $(LIBS)
	$(CXX) -o client client.o $(LIBS)

server1: server1.o database.o article.o newsgroup.o user.o message_handler.o $(LIBS)
	$(CXX) -o server1 server1.o database.o article.o newsgroup.o user.o message_handler.o $(LIBS)

server2: server2.o database.o article.o newsgroup.o user.o message_handler.o $(LIBS)
	$(CXX) -o server2 server2.o database.o article.o newsgroup.o user.o message_handler.o $(LIBS)




client.o: client.cc client.h
	$(CXX) -c $(CXXFLAGS) client.cc

server1.o: server1.cc server1.h
	$(CXX) -c $(CXXFLAGS) server1.cc

server2.o: server2.cc server2.h
	$(CXX) -c $(CXXFLAGS) server2.cc

database.o: database.cc database.h
	$(CXX) -c $(CXXFLAGS) database.cc

article.o: article.cc article.h
	$(CXX) -c $(CXXFLAGS) article.cc

newsgroup.o: newsgroup.cc newsgroup.h
	$(CXX) -c $(CXXFLAGS) newsgroup.cc

user.o: user.cc user.h
	$(CXX) -c $(CXXFLAGS) user.cc

message_handler.o: message_handler.cc message_handler.h
	$(CXX) -c $(CXXFLAGS) message_handler.cc


clean:
	rm -rf *.o client server1 server2

