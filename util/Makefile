all: tagfilter checkdate

checkdate: checkdate.c
	gcc -o checkdate checkdate.c

tagfilter: main.o seamaps.o
	gcc -o tagfilter main.o seamaps.o

main.o: main.c seamaps.h

seamaps.o: seamaps.c seamaps.h

