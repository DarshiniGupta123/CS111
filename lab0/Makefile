#NAME:Darshini Gupta
#EMAIL: darshinigupta0711@gmail.com
#ID: 004928411
files = lab0.c README Makefile backtrace.png breakpoint.png
lab0: lab0.c
	gcc -Wall -Wextra -g -o lab0 lab0.c

clean:
	rm -rf lab0
	rm -rf  lab0-004928411.tar.gz

check:
	./lab0 --segfault --catch --dump-core; \
	if [ $$? -ne 139 ]; \
	then echo "incorrect exit status"; \
	fi

	./lab0; \
	if [ $$? -ne 0 ]; \
	then echo "incorrect exit status"; \
	fi

	./lab0 --segfault --catch; \
	if [ $$? -ne 4 ]; \
	then echo "incorrect exit status"; \
	fi

dist:
	tar -czvf lab0-004928411.tar.gz $(files)
