binaryName = prim
srcDir = ./src/
buildDir = ./build/
binDir = ./bin/
objectFiles = $(buildDir)graph.o
CCFLAGS = -Wall
 
all: graph
	$(CC) $(CCFLAGS) $(srcDir)main.c $(objectFiles) -o $(binDir)$(binaryName)

graph:
	$(CC) $(CCFLAGS) -c $(srcDir)graph.c -o $(buildDir)graph.o

debug: CCFLAGS = -DDEBUG -g
debug: clean all

clean:
	rm -f $(buildDir)*.o
	rm -f $(binDir)*.gch
	rm -f $(binDir)$(binaryName)
