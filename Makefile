CXX = g++
CXXFLAGS = -std=c++17 -I./include -O2 -Wall
SRC = src/Request.cpp src/RequestQueue.cpp src/WebServer.cpp src/Firewall.cpp src/Configuration.cpp src/Logger.cpp src/Utils.cpp src/LoadBalancer.cpp src/JobSwitch.cpp
OBJS = $(SRC:.cpp=.o)
TARGET = lb_sim

all: $(TARGET)

$(TARGET): main.cpp $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) main.cpp $(OBJS)

src/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) src/*.o

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run