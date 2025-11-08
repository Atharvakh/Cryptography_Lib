CXX = g++
CXXFLAGS = -std=c++17 -I.
LDFLAGS = -lntl -lgmp

SRC = main.cpp symmetric/symmetric.cpp asymmetric/ecdlp.cpp asymmetric/dlp.cpp
OUT = crypto

all:
	$(CXX) $(CXXFLAGS) $(SRC) -o $(OUT) $(LDFLAGS)

run: all
	./$(OUT)

clean:
	rm -f $(OUT)
