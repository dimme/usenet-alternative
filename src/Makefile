CXXFLAGS  = -pipe -O2 -Wall -W -ansi -pedantic-errors -DNDEBUG
CXXFLAGS += -Wmissing-braces -Wparentheses -Wold-style-cast
LIBS = clientserver/libclientserver.a
CXX = g++

.PHONY: all clean client server_mem server_disk

all: client server_mem server_disk

client: client.o message_handler.o $(LIBS)
	$(CXX) -o client client.o message_handler.o $(LIBS)

server_mem: server.o database_mem.o article.o newsgroup.o user.o message_handler.o $(LIBS)
	$(CXX) -o server_mem server.o database_mem.o article.o newsgroup.o user.o message_handler.o $(LIBS)

server_disk: server.o database_disk.o article.o newsgroup.o user.o message_handler.o $(LIBS)
	$(CXX) -o server_disk server.o database_disk.o article.o newsgroup.o user.o message_handler.o $(LIBS)


client.o: client.cc
	$(CXX) -c $(CXXFLAGS) client.cc

server.o: server.cc
	$(CXX) -c $(CXXFLAGS) server.cc

database_mem.o: database_mem.cc database.h
	$(CXX) -c $(CXXFLAGS) database_mem.cc

database_disk.o: database_disk.cc database.h
	$(CXX) -c $(CXXFLAGS) database_disk.cc

article.o: article.cc article.h
	$(CXX) -c $(CXXFLAGS) article.cc

newsgroup.o: newsgroup.cc newsgroup.h
	$(CXX) -c $(CXXFLAGS) newsgroup.cc

user.o: user.cc user.h
	$(CXX) -c $(CXXFLAGS) user.cc

message_handler.o: message_handler.cc message_handler.h
	$(CXX) -c $(CXXFLAGS) message_handler.cc


clean:
	rm -rf *.o client server_mem server_disk
