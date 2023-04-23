binaryName = prim
graphBinaryName = graph
srcDir = ./src/
graphSrcDir = $(srcDir)graph/
buildDir = ./build/
binDir = ./bin/
objectFiles = $(buildDir)graph.o
CCFLAGS = -Wall
 
all: graph

graph: graph.o
	$(CC) $(CCFLAGS) $(graphSrcDir)main.c $(objectFiles) -o $(binDir)$(graphBinaryName)

graph.o: $(graphSrcDir)graph.c $(graphSrcDir)graph.h
	$(CC) $(CCFLAGS) -c $(graphSrcDir)graph.c -o $(buildDir)graph.o

debug: CCFLAGS = -DDEBUG -g
debug: clean all

clean:
	rm -f $(buildDir)*.o
	rm -f $(binDir)*.*
