CXX = g++

CXXFLAGS = -std=c++20 -I./share -I/opt/homebrew/Cellar/hiredis/1.2.0/include

LDFLAGS = -L/opt/homebrew/Cellar/hiredis/1.2.0/lib -lhiredis

TARGET = chat_server

SRCS = $(wildcard *.cpp) $(wildcard share/*.cc)
OBJS = $(SRCS:.cpp=.o) $(patsubst %.cc, %.o, $(wildcard share/*.cc))

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS) $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

share/%.o: share/%.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f Main.o Session.o SessionManager.o ThreadPool.o Listener.o PacketManager.o RedisHelper.o

