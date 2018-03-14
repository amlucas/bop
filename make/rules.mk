MPI_TOK=MPI_SPECIFIC_

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(OPT) $< -c -o $@

$(MPI_TOK)%.o: %.cpp
	$(MPICXX) $(MPICXXFLAGS) $(OPT) -c $< -o $@

%: %.o; $(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^ $(LIBS)

%: $(MPI_TOK)%.o; $(MPICXX) $(MPICXXFLAGS) $(LDFLAGS) -o $@ $^ $(LIBS_MPI)
