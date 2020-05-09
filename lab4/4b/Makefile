#NAME: Darshini Gupta
#EMAIL: darshinigupta0711@gmail.com
#ID: 004928411

files = lab4b.c README Makefile 
default: 
	gcc -g lab4b.c -o lab4b -lmraa -lm -Wall -Wextra

clean:
	rm -rf lab4b
	rm -rf  lab4b-004928411.tar.gz

check:
	./lab4b --nonexistantoption; \
	if [ $$? -ne 1 ]; \
	then echo "incorrect exit status"; \
	fi

	{ echo "BOGUSCOMMAND"; } | ./lab4b; \
	if [ $$? -ne 1 ]; \
	then echo "wrong exit status for bad command"; \
	fi
         
	{ echo "PERIOD=2"; sleep 2; echo "OFF"; } | ./lab4b; \
	if [ $$? -ne 0 ]; \
	then echo "incorrect exit status for valid commands"; \
	fi
dist:
	tar -czvf lab4b-004928411.tar.gz $(files)

