include ../../make.inc

all:	xaxpy \
		xsv \
		xsgemm \
		xsgesv \
		xnrm2 \
		xdot \
		xvvt \
		xma \
		xgemv \
		xvtm

LIBRARIES= $(LAPACKELIB) $(LAPACKLIB) $(BLASLIB)

lib: dense.o base.o $(LIBRARIES)

xaxpy: axpy.o lib
	$(CXX) $(CXXFLAGS) axpy.o dense.o base.o $(LIBRARIES) -o $@
	./$@

xsv: sv.o lib
	$(CXX) $(CXXFLAGS) sv.o dense.o base.o $(LIBRARIES) -o $@
	./$@

xsgemm:	sgemm.o lib
	$(CXX) $(CXXFLAGS) sgemm.o $(LIBRARIES) -o $@
	./$@

xsgesv: sgesv.o lib
	$(CXX) $(CXXFLAGS) sgesv.o $(LIBRARIES) -o $@
	./$@

xnrm2: nrm2.o lib
	$(CXX) $(CXXFLAGS) nrm2.o dense.o base.o $(LIBRARIES) -o $@
	./$@

xdot: dot.o lib
	$(CXX) $(CXXFLAGS) dot.o dense.o base.o $(LIBRARIES) -o $@
	./$@

xvvt: vvt.o lib
	$(CXX) $(CXXFLAGS) vvt.o dense.o base.o $(LIBRARIES) -o $@
	./$@

xma: ma.o lib
	$(CXX) $(CXXFLAGS) ma.o dense.o base.o $(LIBRARIES) -o $@
	./$@

xgemv: gemv.o lib
	$(CXX) $(CXXFLAGS) gemv.o dense.o base.o $(LIBRARIES) -o $@
	./$@

xvtm: vtm.o lib
	$(CXX) $(CXXFLAGS) vtm.o dense.o base.o $(LIBRARIES) -o $@
	./$@

clean:
	-rm -f x*.out *.o
