COMPILER = g++

SRC_DIR = ./src
OBJ_DIR = ./obj
BIN_DIR = ./
INC_DIR = ./inc

TARGET = $(BIN_DIR)/hdr

SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(addprefix $(OBJ_DIR)/, $(notdir $(SRCS:.cpp=.o)))

LIBS =
INCLUDE = -I$(INC_DIR) $(shell pkg-config --cflags opencv) $(shell pkg-config --cflags armadillo)
CPPFLAGS += -Wall -std=c++1z -O2
LDFLAGS +=
LDLIBS += $(shell pkg-config --libs opencv) $(shell pkg-config --libs armadillo)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(COMPILER) $(LDFLAGS) -o $@ $^ $(LDLIBS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(COMPILER) $(CPPFLAGS) $(INCLUDE) -o $@ -c $<

clean:
	$(RM) $(OBJS)

run:
	$(TARGET)
