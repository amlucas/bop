%.o: %.cpp; $(CXX) $(CXXFLAGS) $(OPT) $< -c -o $@
%: %.o; $(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^ $(LIBS)

