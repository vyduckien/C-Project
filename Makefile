
output: app.o
	gcc app.o -o app

app.o: app.c bmp.h
	gcc -c -Wall -pedantic -g app.c binary.c
# test: test.o
# 	gcc test.o -o  test
# test.o: test.c bmp.h
# 	gcc -c -Wall -pedantic -g test.c
clean: 
	del *.o	output