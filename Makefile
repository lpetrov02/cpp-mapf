CXXFLAGS = -Wall -Werror -Wextra -g -std=c++20
OBJECTS = obj/astar.o obj/astarStructs.o

.PHONY: all clean

all: algo

obj:
	mkdir obj
	chmod a=rwx obj

$(OBJECTS): obj/%.o: src/%.cpp
	g++ $(CXXFLAGS) -I include -c $< -o $@

astar.o : astarStructs.h
astarStructs.o : astarStructs.h

algo : obj $(OBJECTS)
	g++ $(CXXFLAGS) -o $@ $(OBJECTS)

clean :
	$(RM) -f -r obj algo