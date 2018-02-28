MPI_TOK=MPI_SPECIFIC_

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(OPT) $< -c -o $@

$(MPI_TOK)%.o: %.cpp
	$(MPI_CXX) $(MPICXXFLAGS) $(OPT) -c $< -o $@

%: %.o; $(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^ $(LIBS)

%: $(MPI_TOK)%.o; $(MPI_CXX) $(MPICXXFLAGS) $(LDFLAGS) -o $@ $^ $(LIBS_MPI)
