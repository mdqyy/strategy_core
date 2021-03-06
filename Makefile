#
#  Top Level Makefile for strategy_core
#  Version 1.0.0
#

include make.inc

all: logistic_reg logistic_reg_test matrix_test

lib:

logistic_reg: lib
	( cd projects/LogisticReg/ ; $(MAKE) )

logistic_reg_test:
	( cd test/LogisticReg/ ; $(MAKE) )

matrix_test: lib
	( cd test/matrix/ ; $(MAKE) )

logistic_reg_clean:
	( cd projects/LogisticReg/ ; $(MAKE) clean )

logistic_reg_test_clean:
	( cd test/LogisticReg/ ; $(MAKE) clean )
	
matrix_test_clean:
	( cd test/matrix/ ; $(MAKE) clean )

update:	svn git

svn:
	( Date=$(date) ; svn commit -m ${Date}" chenqinyu" ; )

git:
	( git add . -A ; Date=$(date) ; git commit -m ${Date}" chenqinyu" ; git push -f ; )

clean: logistic_reg_clean logistic_reg_test_clean matrix_test_clean
