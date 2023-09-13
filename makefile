binaryName = prim
graphBinaryName = gengraph
primBinaryName = prim
drawBinaryName = drawgraph
srcDir = ./src/
graphSrcDir = $(srcDir)graph/
fileSrcDir = $(srcDir)file/
primSrcDir = $(srcDir)prim/
drawSrcDir = $(srcDir)draw/
buildDir = ./build/
binDir = ./bin/

graphIncludeFlags = -I/usr/local/include/graphviz
primIncludeFlags =
linkDir = -L /usr/local/lib/graphviz/
linkLibraries = -lm -lpthread
primLinkLibraries = -lm -lpthread
drawLinkLibraries =  -L/usr/local/lib -lcgraph -lgvc 

graphObjectFiles = $(buildDir)graph.o $(buildDir)file_module.o
drawObjectFiles = $(buildDir)draw_graph.o $(buildDir)graph.o $(buildDir)file_module.o 
primObjectFiles = $(buildDir)prim_st.o $(buildDir)prim_mt.o $(buildDir)file_module.o $(buildDir)graph.o

CCFLAGS = -Wall -Wno-maybe-uninitialized
 
all: graph prim draw

debug: debug_graph debug_prim debug_draw

draw: graph.o file_module.o draw.o
	$(CC) $(CCFLAGS) $(linkDir) $(drawSrcDir)main.c $(drawObjectFiles) -lm $(drawLinkLibraries) -o $(binDir)$(drawBinaryName)	

graph: graph.o file_module.o
	$(CC) $(CCFLAGS) $(linkDir) $(graphSrcDir)main.c $(graphObjectFiles) $(linkLibraries) -o $(binDir)$(graphBinaryName) $(linkLibraries)

prim: prim_st.o prim_mt.o file_module.o graph.o
	$(CC) $(CCFLAGS) $(primSrcDir)main.c $(primObjectFiles) $(primLinkLibraries) -o $(binDir)$(primBinaryName)

draw.o: $ $(drawSrcDir)draw_graph.c $(drawSrcDir)draw_graph.h
	$(CC) $(CCFLAGS) -c $(drawSrcDir)draw_graph.c $(includeFlags) $(drawLinkLibraries) -o $(buildDir)draw_graph.o

graph.o: $(graphSrcDir)graph.c $(graphSrcDir)graph.h
	$(CC) $(CCFLAGS) -c $(graphSrcDir)graph.c -o $(buildDir)graph.o
	
file_module.o: $(fileSrcDir)file_module.c $(fileSrcDir)file_module.h
	$(CC) $(CCFLAGS) -c $(fileSrcDir)file_module.c -o $(buildDir)file_module.o

prim_st.o: $(primSrcDir)prim_st.c $(primSrcDir)prim_st.h
	$(CC) $(CCFLAGS) -c $(primSrcDir)prim_st.c -o $(buildDir)prim_st.o

prim_mt.o: $(primSrcDir)prim_mt.c $(primSrcDir)prim_mt.h
	$(CC) $(CCFLAGS) -c $(primSrcDir)prim_mt.c -o $(buildDir)prim_mt.o

# test params
GRAPH_SIZE = 12
GRAPH_NAME = graph.grf
EDGE_PERCENTAGE = 40
GRAPH_TITLE = "Grafo Teste"

compare: 
	$(binDir)$(graphBinaryName) -s $(GRAPH_SIZE) -f $(GRAPH_NAME) -p $(EDGE_PERCENTAGE)
	$(binDir)$(primBinaryName) -f $(GRAPH_NAME)
	$(binDir)$(primBinaryName) -f $(GRAPH_NAME) -t 2

test_prim:
	$(binDir)$(primBinaryName) -f $(GRAPH_NAME)

test_mt:
	$(binDir)$(primBinaryName) -f $(GRAPH_NAME) -t 2

test_graph:
	$(binDir)$(graphBinaryName) -s $(GRAPH_SIZE) -f $(GRAPH_NAME) -p $(EDGE_PERCENTAGE)

test_draw:
	$(binDir)$(drawBinaryName) -f $(GRAPH_NAME) -t $(GRAPH_TITLE)

test_debug_graph:
	$(binDir)$(graphBinaryName) -s $(GRAPH_SIZE) -f $(GRAPH_NAME) -p $(EDGE_PERCENTAGE) -m

read_graph:
	$(binDir)$(primBinaryName) -f $(GRAPH_NAME) -p

gdb_graph:
	 gdb --args $(binDir)$(graphBinaryName) -s $(GRAPH_SIZE) -f $(GRAPH_NAME) -p $(EDGE_PERCENTAGE)

gdb_draw:
	 gdb --args $(binDir)$(drawBinaryName) -f $(GRAPH_NAME) -t $(GRAPH_TITLE)

gdb_prim:
	gdb -ex 'b file_module.c:65' --args $(binDir)$(primBinaryName) -f $(GRAPH_NAME)

gdb_mt:
	gdb -ex 'b prim_mt.c:105' --args $(binDir)$(primBinaryName) -f $(GRAPH_NAME) -t 2

debug_prim: CCFLAGS += -DDEBUG -g
debug_prim: prim

trace_prim: CCFLAGS += -DTRACE -DDEBUG -g
trace_prim: prim

lock_prim:	CCFLAGS += -DLOCK -g
lock_prim:	debug_prim

lock_graph:	CCFLAGS += -DLOCK -g
lock_graph:	debug_graph

debug_draw: CCFLAGS += -DDEBUG -g
debug_draw: draw 

debug_graph: CCFLAGS += -DDEBUG -g
debug_graph: graph



# debug: clean all

clean:
	rm -f $(buildDir)*.o
	rm -f $(binDir)*
	rm -f *.grf
	rm -f *.png
	rm -f *.dot
	rm -f *.svg
