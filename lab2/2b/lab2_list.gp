#! /usr/bin/gnuplot
#
# purpose:
#	 generate data reduction graphs for the multi-threaded list project
#
# input: lab2_list.csv
#	1. test name
#	2. # threads
#	3. # iterations per thread
#	4. # lists
#	5. # operations performed (threads x iterations x (ins + lookup + delete))
#	6. run time (ns)
#	7. run time per operation (ns)
#	8. average lock-wait time
#
# output:
#	lab2_list2b_1.png ... cost per operation vs threads and iterations
#	lab2_list2b_2.png ... threads and iterations that run (un-protected) w/o failure
#	lab2_list2b_3.png ... threads and iterations that run (protected) w/o failure
#	lab2_list2b_4.png ... cost per operation vs number of threads
#
# Note:
#	Managing data is simplified by keeping all of the results in a single
#	file.  But this means that the individual graphing commands have to
#	grep to select only the data they want.
#
#	Early in your implementation, you will not have data for all of the
#	tests, and the later sections may generate errors for missing data.
#

# general plot parameters
set terminal png
set datafile separator ","


set title "List-1: Throughput vs. Number of threads (Mutex and Spin-Lock)"
set xlabel "Threads"
set logscale x 2
#unset xrange
set xrange [0.75:]
set ylabel "Throughput per Operation(ns)"
set logscale y 10 
set output 'lab2b_1.png'
set key left top

plot \
     "< grep -e 'list-none-m,[0-9]*,1000,1,' lab2b_list.csv" using ($2):(1000000000/$7) \
	title 'list w/mutex' with linespoints lc rgb 'blue', \
     "< grep -e 'list-none-s,[0-9]*,1000,1,' lab2b_list.csv" using ($2):(1000000000/$7) \
	title 'list w/spin-lock' with linespoints lc rgb 'green'


set title "List-2: Wait-for-Lock and Average Operation Time by Threads (Mutex)"
set xlabel "Threads"
set logscale x 2
#unset xrange
set xrange [0.75:]
set ylabel "Time (Wait-for-Lock and Operation Time)"
set logscale y 
set output 'lab2b_2.png'
set key left top
plot \
     "< grep -e 'list-none-m,[0-9]*,1000,1,' lab2b_list.csv" using ($2):($7) \
        title 'average time' with linespoints lc rgb 'blue', \
     "< grep -e 'list-none-m,[0-9]*,1000,1,' lab2b_list.csv" using ($2):($8) \
        title 'wait-for-lock' with linespoints lc rgb 'green'

set title "List-3: Successful Iterations "
set xlabel "Threads"
set logscale x 2
#unset xrange
set xrange [0.75:]
set ylabel "Iterations that Succeeded"
set logscale y 
set output 'lab2b_3.png'
set key left top
plot \
     "< grep -e list-id-none lab2b_list.csv" using ($2):($3) \
        title 'unprotected' with points lc rgb 'blue', \
     "< grep -e list-id-m lab2b_list.csv" using ($2):($3) \
        title 'mutex' with points pointsize 2 lc rgb 'green', \
     "< grep -e list-id-s lab2b_list.csv" using ($2):($3) \
     	title 'spin-lock' with points lc rgb 'violet'


set title "List-4: Throughout with varying List Sizes (Mutex)"
set xlabel "Threads"
#unset xrange
set xrange [0.75:]
set ylabel "Throughput per operation(ns)"
set logscale y 10
set output 'lab2b_4.png'
set key right top
plot \
     "< grep -e 'list-none-m,[0-9]*,1000,1,' lab2b_list.csv" using ($2):(1000000000/$7) \
     	title 'lists=1' with linespoints lc rgb 'blue', \
     "< grep -e 'list-none-m,[0-9]*,1000,4,' lab2b_list.csv" using ($2):(1000000000/$7) \
     	title 'lists=4' with linespoints lc rgb	'green', \
     "<	grep -e 'list-none-m,[0-9]*,1000,8,' lab2b_list.csv" using ($2):(1000000000/$7) \
     	title 'lists=8' with linespoints lc rgb 'red', \
     "< grep -e 'list-none-m,[0-9]*,1000,16,' lab2b_list.csv" using ($2):(1000000000/$7) \
     	title 'lists=16' with linespoints lc rgb 'violet', \

set title "List-5: Throughout with varying List Sizes (Spin-Lock)"
set xlabel "Threads"
#unset xrange
set xrange [0.75:]
set ylabel "Throughput per operation(ns)"
set logscale y 10
set output 'lab2b_5.png'
set key right top
plot \
     "< grep -e 'list-none-s,[0-9]*,1000,1,' lab2b_list.csv" using ($2):(1000000000/$7) \
        title 'lists=1' with linespoints lc rgb 'blue', \
     "< grep -e 'list-none-s,[0-9]*,1000,4,' lab2b_list.csv" using ($2):(1000000000/$7) \
        title 'lists=4' with linespoints lc rgb 'green', \
     "< grep -e 'list-none-s,[0-9]*,1000,8,' lab2b_list.csv" using ($2):(1000000000/$7) \
        title 'lists=8' with linespoints lc rgb 'red', \
     "< grep -e 'list-none-s,[0-9]*,1000,16,' lab2b_list.csv" using ($2):(1000000000/$7) \
        title 'lists=16' with linespoints lc rgb 'violet', \
