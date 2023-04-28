binaryName = prim
graphBinaryName = graph_generator
primBinaryName = prim
srcDir = ./src/
graphSrcDir = $(srcDir)graph/
fileSrcDir = $(srcDir)file/
primSrcDir = $(srcDir)prim/
buildDir = ./build/
binDir = ./bin/
graphObjectFiles = $(buildDir)graph.o $(buildDir)file_module.o
primObjectFiles = $(buildDir)prim_st.o $(buildDir)file_module.o
CCFLAGS = -Wall
 
all: graph prim

graph: graph.o file_module.o
	$(CC) $(CCFLAGS) $(graphSrcDir)main.c $(graphObjectFiles) -o $(binDir)$(graphBinaryName)

prim: prim_st.o file_module.o
	$(CC) $(CCFLAGS) $(primSrcDir)main.c $(primObjectFiles) -o $(binDir)$(primBinaryName)

graph.o: $(graphSrcDir)graph.c $(graphSrcDir)graph.h
	$(CC) $(CCFLAGS) -c $(graphSrcDir)graph.c -o $(buildDir)graph.o

file_module.o: $(fileSrcDir)file_module.c $(fileSrcDir)file_module.h
	$(CC) $(CCFLAGS) -c $(fileSrcDir)file_module.c -o $(buildDir)file_module.o

prim_st.o: $(primSrcDir)prim_st.c $(primSrcDir)prim_st.h
	$(CC) $(CCFLAGS) -c $(primSrcDir)prim_st.c -o $(buildDir)prim_st.o

debug: CCFLAGS = -DDEBUG -g
debug: clean all

clean:
	rm -f $(buildDir)*.o
	rm -f $(binDir)*.*
