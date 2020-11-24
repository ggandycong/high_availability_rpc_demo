INC_DIR:= . /usr/local/include/zookeeper/ 
SRCS:= $(wildcard ./*cc )
OBJS:= $(patsubst %.cc, %.o, $(SRCS))
LIBS:= -lpthread -lzookeeper_mt 
#LIBS:= -lpthread -lzookeeper_st

CXX:= g++

CXXFLAGS:= -w -g -std=c++11 $(addprefix -I, $(INC_DIR)) $(LIBS) -Wl,-rpath="/usr/local/lib"

EXE:= zk_test

all: $(EXE)

$(EXE):$(OBJS)
	$(CXX) -o $(EXE) $(OBJS) $(CXXFLAGS)

clean:
	rm -rf $(EXE)
	rm -rf $(OBJS)
