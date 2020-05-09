#NAME:Darshini Gupta
#EMAIL: darshinigupta0711@gmail.com
#ID: 004928411

files = lab2_list.c lab2b_list.csv README Makefile SortedList.h SortedList.c lab2b_1.png lab2b_2.png lab2b_3.png lab2b_4.png lab2b_5.png lab2_list.gp testscript2.sh profile.out
build: 
	gcc -Wall -Wextra -g -lpthread -o lab2_list lab2_list.c SortedList.c

clean:
	rm -rf lab2_list
	rm -rf lab2b-004928411.tar.gz

tests:	build
	chmod +x testscript2.sh
	./testscript2.sh

profile:
	rm -rf ./raw.gperf
	LD_PRELOAD=/usr/lib64/libprofiler.so CPUPROFILE=./raw.gperf \
	./lab2_list --threads=12 --iterations=1000 --sync=s
	pprof --text ./lab2_list ./raw.gperf > profile.out
	pprof --list=thread_worker lab2_list raw.gperf >> profile.out
	rm -rf ./raw.gperf

graphs: tests
	chmod +x lab2_list.gp
	gnuplot ./lab2_list.gp

dist: graphs
	tar -czvf lab2b-004928411.tar.gz $(files)
