include ../../make.inc

all:	cal_loss

cal_loss:
	( ${PYTHON} cal_loss.py weight ../../data/logistic.sample > loss.txt ; tail -2 loss.txt )

clean:
	-rm -f loss.txt

