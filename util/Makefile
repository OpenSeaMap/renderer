all: tagfilter checkdate

checkdate: checkdate.c
	gcc -o checkdate checkdate.c

tagfilter: tagfilter.o seamaps.o
	gcc -o tagfilter tagfilter.o seamaps.o

tagfilter.o: tagfilter.c seamaps.h

seamaps.o: seamaps.c seamaps.h

