binaryName = prim
graphBinaryName = graph_generator
srcDir = ./src/
graphSrcDir = $(srcDir)graph/
fileSrcDir = $(srcDir)file/
buildDir = ./build/
binDir = ./bin/
objectFiles = $(buildDir)graph.o $(buildDir)file_module.o
CCFLAGS = -Wall
 
all: graph

graph: graph.o file_module.o
	$(CC) $(CCFLAGS) $(graphSrcDir)main.c $(objectFiles) -o $(binDir)$(graphBinaryName)

graph.o: $(graphSrcDir)graph.c $(graphSrcDir)graph.h
	$(CC) $(CCFLAGS) -c $(graphSrcDir)graph.c -o $(buildDir)graph.o

file_module.o: $(fileSrcDir)file_module.c $(fileSrcDir)file_module.h
	$(CC) $(CCFLAGS) -c $(fileSrcDir)file_module.c -o $(buildDir)file_module.o
	
debug: CCFLAGS = -DDEBUG -g
debug: clean all

clean:
	rm -f $(buildDir)*.o
	rm -f $(binDir)*.*
