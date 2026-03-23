CXX      = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I src
TARGET   = card_run

SRCS = src/main.cpp \
       src/RunSession.cpp \
       src/ScoringSystem.cpp \
       src/ShopSystem.cpp \
       src/ModifierFactory.cpp

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRCS)

run: all
	./$(TARGET)

clean:
	rm -f $(TARGET)
