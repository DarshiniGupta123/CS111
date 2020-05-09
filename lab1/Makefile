#NAME:Darshini Gupta
#EMAIL: darshinigupta0711@gmail.com
#ID: 004928411
files = lab1.c pg98.txt README Makefile bash.sh dash.sh report.pdf

simpsh: lab1.c
	gcc -Wall -Wextra -g -o simpsh lab1.c

clean:
	rm -rf simpsh
	rm -rf  lab1-004928411.tar.gz

check:
	./simpsh; \
	if [ $$? -ne 0 ]; \
	then echo "incorrect exit status"; \
	fi

	./simpsh --bogus; \
	if [ $$? -ne 1 ]; \
	then echo "incorrect exit status"; \
	fi

	./simpsh --rdonly bogusfile.txt; \
	if [ $$? -ne 1 ]; \
	then echo "incorrect exit status"; \
	fi

dist:
	tar -czvf lab1-004928411.tar.gz $(files)



