all:	xsgemm \
			xsgesv \
			xaxpy	\
			xsv

LIBRARIES= -llapacke -llapack -lblas

xaxpy: axpy.o dense.o base.o $(LIBRARIES)
	g++ -g axpy.o dense.o base.o $(LIBRARIES) -o $@
	./$@

xsv: sv.o dense.o base.o $(LIBRARIES)
	g++ -g sv.o dense.o base.o $(LIBRARIES) -o $@
	./$@

xsgemm:	sgemm.o $(LIBRARIES)
	g++ -g sgemm.o $(LIBRARIES) -o $@
	./$@

xsgesv:	sgesv.o $(LIBRARIES)
	g++ -g sgesv.o $(LIBRARIES) -o $@
	./$@

clean:
	rm -f x* *.o
