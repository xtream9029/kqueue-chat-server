CXX = g++ 

CXXFLAGS = -std=c++20 -I/opt/homebrew/opt/protobuf/include -I/opt/homebrew/opt/abseil/include -I./share
LDFLAGS = -L/opt/homebrew/opt/protobuf/lib -L/opt/homebrew/opt/abseil/lib -lprotobuf -labsl_strings
TARGET = chat_client

SRCS = $(wildcard *.cpp) $(wildcard share/*.cc)
OBJS = $(SRCS:.cpp=.o) $(patsubst %.cc, %.o, $(wildcard share/*.cc))

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS) $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

share/%.o: share/%.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f Main.o Session.o PacketManager.o

