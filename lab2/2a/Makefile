#NAME:Darshini Gupta
#EMAIL: darshinigupta0711@gmail.com
#ID: 004928411

files = lab2_add.c lab2_list.c lab2_add.csv lab2_list.csv README Makefile SortedList.h SortedList.c lab2_add-1.png lab2_add-2.png lab2_add-3.png lab2_add-4.png lab2_add-5.png lab2_add.gp testscript.sh lab2_list-1.png lab2_list-2.png lab2_list-3.png lab2_list-4.png lab2_list.gp

build: 
	gcc -Wall -Wextra -g -lpthread -o lab2_add lab2_add.c
	gcc -Wall -Wextra -g -lpthread -o lab2_list lab2_list.c SortedList.c

clean:
	rm -rf lab2_add
	rm -rf lab2_list
	rm -rf lab2a-004928411.tar.gz

tests:	build
	chmod +x testscript.sh
	./testscript.sh

graphs: tests
	chmod +x lab2_add.gp
	chmod +x lab2_list.gp
	gnuplot ./lab2_add.gp
	gnuplot ./lab2_list.gp

dist: graphs
	tar -czvf lab2a-004928411.tar.gz $(files)
