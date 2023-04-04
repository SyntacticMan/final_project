binaryName = prim
srcDir = ./src/
buildDir = ./build/
binDir = ./bin/
objectFiles = #$(buildDir)xml.o $(buildDir)lua.o $(buildDir)planeta.o $(buildDir)menu.o $(buildDir)menu_principal.o
 
all: 
	$(CC) $(CCFLAGS) $(srcDir)main.c $(objectFiles) -o $(binDir)$(binaryName)

#xml: $(srcDir)xml.cpp $(srcDir)xml.h
#	$(CXX) $(CXXFLAGS) -c $(srcDir)xml.cpp -o $(buildDir)xml.o
#
#menu: $(srcDir)menu.cpp $(srcDir)menu.h
#	$(CXX) $(CXXFLAGS) -c $(srcDir)menu.cpp -o $(buildDir)menu.o
#
#menu_principal: $(srcDir)menu_principal.cpp $(srcDir)menu_principal.h
#	$(CXX) $(CXXFLAGS) -c $(srcDir)menu_principal.cpp -o $(buildDir)menu_principal.o
#
#lua: $(srcDir)lua.cpp $(srcDir)lua.h
#	$(CXX) $(CXXFLAGS) -c $(srcDir)lua.cpp -o $(buildDir)lua.o
#
#planeta: $(srcDir)planeta.cpp $(srcDir)planeta.h
#	$(CXX) $(CXXFLAGS) -c $(srcDir)planeta.cpp -o $(buildDir)planeta.o

debug: CCFLAGS = -DDEBUG -g
debug: clean all

clean:
	rm -f $(buildDir)*.o
	rm -f $(binDir)*.gch
	rm -f $(binDir)$(binaryName)
