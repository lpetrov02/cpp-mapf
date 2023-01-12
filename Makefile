CXXFLAGS = -Wall -Werror -Wextra -g -std=c++20
OBJECTS = obj/main.o obj/astar.o obj/astarStructs.o obj/astarNodes.o obj/constraints.o obj/forTesting.o obj/cbsStructs.o obj/cbsDsStructs.o

.PHONY: all clean

all: algo

obj:
	mkdir obj
	chmod a=rwx obj

$(OBJECTS): obj/%.o: src/%.cpp
	g++ $(CXXFLAGS) -I include -c $< -o $@

astar.o : astarStructs.h astar.h
astarStructs.o : astarStructs.h astarNodes.h
astarNodes.o : astarNodes.h
constraints.o : constraints.h
forTesting.o : astarNodes.h forTesting.h cbsStructs.h
cbsStructs.o : cbsStructs.h astarStructs.h astarNode.h constraints.h
cbsStructs.o : cbsStructs.h cbsDsStructs.h astarStructs.h astarNode.h constraints.h
main.o : astar.h astarNodes.h astarStructs.h constraints.h forTesting.h cbsStructs.h cbsDsStructs.h

algo : obj $(OBJECTS)
	g++ $(CXXFLAGS) -o $@ $(OBJECTS)

clean :
	$(RM) -f -r obj algo