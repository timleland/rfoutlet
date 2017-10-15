CXXFLAGS = -D RPI
all: codesend sniffer
        
codesend: RCSwitch.o codesend.o
        $(CXX) $(CXXFLAGS) $(LDFLAGS) $+ -o $@ -lwiringPi                                            
        
sniffer: RCSwitch.o sniffer.o
        $(CXX) $(CXXFLAGS) $(LDFLAGS) $+ -o $@ -lwiringPi
        
clean:
        $(RM) *.o codesend sniffer

