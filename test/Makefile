include ../make.inc

all:	xaxpy \
			xsv \
			xsgemm	\
			xsgesv	\
			xnrm2	\
			xdot	\
			xvvt	\
			xma	\
			xgemv	\
			xvtm
			
LIBRARIES= $(LAPACKELIB) $(LAPACKLIB) $(BLASLIB)

xaxpy: axpy.o dense.o base.o $(LIBRARIES)
	$(CXX) $(CXXFLAGS) axpy.o dense.o base.o $(LIBRARIES) -o $@
	./$@

xsv: sv.o dense.o base.o $(LIBRARIES)
	$(CXX) $(CXXFLAGS) sv.o dense.o base.o $(LIBRARIES) -o $@
	./$@

xsgemm:	sgemm.o $(LIBRARIES)
	$(CXX) $(CXXFLAGS) sgemm.o $(LIBRARIES) -o $@
	./$@

xsgesv: sgesv.o $(LIBRARIES)
	$(CXX) $(CXXFLAGS) sgesv.o $(LIBRARIES) -o $@
	./$@

xnrm2: nrm2.o dense.o base.o $(LIBRARIES)
	$(CXX) $(CXXFLAGS) nrm2.o dense.o base.o $(LIBRARIES) -o $@
	./$@

xdot: dot.o dense.o base.o $(LIBRARIES)
	$(CXX) $(CXXFLAGS) dot.o dense.o base.o $(LIBRARIES) -o $@
	./$@

xvvt: vvt.o dense.o base.o $(LIBRARIES)
	$(CXX) $(CXXFLAGS) vvt.o dense.o base.o $(LIBRARIES) -o $@
	./$@

xma: ma.o dense.o base.o $(LIBRARIES)
	$(CXX) $(CXXFLAGS) ma.o dense.o base.o $(LIBRARIES) -o $@
	./$@

xgemv: gemv.o dense.o base.o $(LIBRARIES)
	$(CXX) $(CXXFLAGS) gemv.o dense.o base.o $(LIBRARIES) -o $@
	./$@

xvtm: vtm.o dense.o base.o $(LIBRARIES)
	$(CXX) $(CXXFLAGS) vtm.o dense.o base.o $(LIBRARIES) -o $@
	./$@

clean:
	-rm -f x* *.o
