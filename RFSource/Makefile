CXXFLAGS = -D RPI
all: codesend sniffer
        
codesend: RCSwitch.o shared_mutex.c codesend.o
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $+ -o $@ -lwiringPi -pthread -lrt
        
sniffer: RCSwitch.o sniffer.o
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $+ -o $@ -lwiringPi
        
clean:
	$(RM) *.o codesend sniffer

