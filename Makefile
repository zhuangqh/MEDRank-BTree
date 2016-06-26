SRCS = ./src/main.cpp
SRCS += ./src/rand_vector.cpp
SRCS += ./src/projection_generator.cpp
SRCS += ./src/timer.cpp
SRCS += ./src/util.cpp
SRCS += ./src/block_file.cpp
SRCS += ./src/b_node.cpp
SRCS += ./src/b_tree.cpp
SRCS += ./src/query_cache.cpp
SRCS += ./src/medrank.cpp

OBJS=$(SRCS:.cpp=.o)

CXX?=g++

all: $(OBJS)
	$(CXX) -o medrank $(OBJS)
rand_vector.o: rand_vector.h

projection_generator.o: projection_generator.h

util.o: util.h

timer.o: timer.h

block_file.o: block_file.h

b_node.o: b_node.h

b_tree.o: b_tree.h

query_cache.o: query_cache.h

medrank.o: medrank.h

clean:
	rm -rf ./src/*.o medrank

cleanFile:
	rm -rf ./data/*.btree
