binaryName = prim
graphBinaryName = graph_generator
primBinaryName = prim
srcDir = ./src/
graphSrcDir = $(srcDir)graph/
fileSrcDir = $(srcDir)file/
primSrcDir = $(srcDir)prim/
buildDir = ./build/
binDir = ./bin/

includeFlags = -I /usr/include/graphviz
linkDir = -L /usr/lib/x86_64-linux-gnu/graphviz/
linkLibraries = -lgvc -lcgraph

graphObjectFiles = $(buildDir)graph.o $(buildDir)file_module.o
primObjectFiles = $(buildDir)prim_st.o $(buildDir)file_module.o $(buildDir)graph.o

CCFLAGS = -Wall $(includeFlags)
 
all: graph prim

graph: graph.o file_module.o
	$(CC) $(CCFLAGS) $(linkDir) $(graphSrcDir)main.c $(graphObjectFiles) $(linkLibraries) -o $(binDir)$(graphBinaryName)

prim: prim_st.o file_module.o graph.o
	$(CC) $(CCFLAGS) $(linkDir) $(primSrcDir)main.c $(primObjectFiles) $(linkLibraries) -o $(binDir)$(primBinaryName)

graph.o: $(graphSrcDir)graph.c $(graphSrcDir)graph.h
	$(CC) $(CCFLAGS) -c $(graphSrcDir)graph.c -o $(buildDir)graph.o
	
file_module.o: $(fileSrcDir)file_module.c $(fileSrcDir)file_module.h
	$(CC) $(CCFLAGS) -c $(fileSrcDir)file_module.c -o $(buildDir)file_module.o

prim_st.o: $(primSrcDir)prim_st.c $(primSrcDir)prim_st.h
	$(CC) $(CCFLAGS) -c $(primSrcDir)prim_st.c -o $(buildDir)prim_st.o

test:
	$(binDir)$(graphBinaryName) -s 10 -f graph.grf -p 70
	$(binDir)$(primBinaryName) -f graph.grf

test_graph:
	$(binDir)$(graphBinaryName) -s 10 -f graph.grf -p 70

debug_graph:
	 gdb --args $(binDir)$(graphBinaryName) -s 5 -f graph.grf -p 70

debug: CCFLAGS += -DDEBUG -g
debug: clean all

clean:
	rm -f $(buildDir)*.o
	rm -f $(binDir)*
	rm -f *.grf
