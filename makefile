binaryName = prim
graphBinaryName = graph_generator
primBinaryName = prim
srcDir = ./src/
graphSrcDir = $(srcDir)graph/
fileSrcDir = $(srcDir)file/
primSrcDir = $(srcDir)prim/
drawSrcDir = $(srcDir)draw/
buildDir = ./build/
binDir = ./bin/

includeFlags = -I /usr/include/graphviz
linkDir = -L /usr/lib/x86_64-linux-gnu/graphviz/
linkLibraries = -lm -lgvc -lcgraph -lpthread

graphObjectFiles = $(buildDir)graph.o $(buildDir)file_module.o $(buildDir)draw_graph.o
primObjectFiles = $(buildDir)prim_st.o $(buildDir)prim_mt.o $(buildDir)file_module.o $(buildDir)graph.o

CCFLAGS = -Wall $(includeFlags)
 
all: graph prim

graph: graph.o file_module.o draw.o
	$(CC) $(CCFLAGS) $(linkDir) $(graphSrcDir)main.c $(graphObjectFiles) $(linkLibraries) -o $(binDir)$(graphBinaryName) $(linkLibraries)

prim: prim_st.o prim_mt.o file_module.o graph.o
	$(CC) $(CCFLAGS) $(linkDir) $(primSrcDir)main.c $(primObjectFiles) $(linkLibraries) -o $(binDir)$(primBinaryName)

draw.o: $ $(drawSrcDir)draw_graph.c $(drawSrcDir)draw_graph.h
	$(CC) $(CCFLAGS) -c $(drawSrcDir)draw_graph.c -o $(buildDir)draw_graph.o

graph.o: $(graphSrcDir)graph.c $(graphSrcDir)graph.h
	$(CC) $(CCFLAGS) -c $(graphSrcDir)graph.c -o $(buildDir)graph.o
	
file_module.o: $(fileSrcDir)file_module.c $(fileSrcDir)file_module.h
	$(CC) $(CCFLAGS) -c $(fileSrcDir)file_module.c -o $(buildDir)file_module.o

prim_st.o: $(primSrcDir)prim_st.c $(primSrcDir)prim_st.h
	$(CC) $(CCFLAGS) -c $(primSrcDir)prim_st.c -o $(buildDir)prim_st.o

prim_mt.o: $(primSrcDir)prim_mt.c $(primSrcDir)prim_mt.h
	$(CC) $(CCFLAGS) -c $(primSrcDir)prim_mt.c -o $(buildDir)prim_mt.o

compare: 
	$(binDir)$(graphBinaryName) -s 30 -f graph.grf -p 70
	$(binDir)$(primBinaryName) -f graph.grf
	$(binDir)$(primBinaryName) -f graph.grf -t 2


test:
	$(binDir)$(graphBinaryName) -s 30 -f graph.grf -p 70
	$(binDir)$(primBinaryName) -f graph.grf

test_mt:
	$(binDir)$(graphBinaryName) -s 30 -f graph.grf -p 70
	$(binDir)$(primBinaryName) -f graph.grf -t 2

test_graph:
	$(binDir)$(graphBinaryName) -s 5000 -f graph.grf -p 70 -m

gdb_graph:
	 gdb --args $(binDir)$(graphBinaryName) -s 600 -f graph.grf -p 70

debug_prim:
	$(binDir)$(graphBinaryName) -s 6 -f graph.grf -p 70
	gdb -ex 'b prim_st.c:84' --args $(binDir)$(primBinaryName) -f graph.grf 

debug: CCFLAGS += -DDEBUG -g
debug: clean all

clean:
	rm -f $(buildDir)*.o
	rm -f $(binDir)*
	rm -f *.grf
	rm -f *.png
