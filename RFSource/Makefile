CXXFLAGS = -D RPI
all: codesend sniffer
        
codesend: RCSwitch.o shared_mutex.c RFOutlet.c codesend.o
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $+ -o $@ -lwiringPi -pthread -lrt
        
sniffer: RCSwitch.o RFOutlet.c sniffer.o
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $+ -o $@ -lwiringPi
        
clean:
	$(RM) *.o codesend sniffer

