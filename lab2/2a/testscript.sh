#!/bin/bash
rm -rf lab2_add.csv
rm -rf lab2_list.csv
#1
./lab2_add --iterations=10 --threads=2 >> lab2_add.csv
./lab2_add --iterations=20 --threads=2 >> lab2_add.csv
./lab2_add --iterations=40 --threads=2 >> lab2_add.csv
./lab2_add --iterations=80 --threads=2 >> lab2_add.csv
./lab2_add --iterations=100 --threads=2 >> lab2_add.csv
./lab2_add --iterations=1000 --threads=2 >> lab2_add.csv
./lab2_add --iterations=10000 --threads=2 >> lab2_add.csv
./lab2_add --iterations=100000 --threads=2 >> lab2_add.csv

./lab2_add --iterations=10 --threads=4 >> lab2_add.csv
./lab2_add --iterations=20 --threads=4 >> lab2_add.csv
./lab2_add --iterations=40 --threads=4 >> lab2_add.csv
./lab2_add --iterations=80 --threads=4 >> lab2_add.csv
./lab2_add --iterations=100 --threads=4 >> lab2_add.csv
./lab2_add --iterations=1000 --threads=4 >> lab2_add.csv
./lab2_add --iterations=10000 --threads=4 >> lab2_add.csv
./lab2_add --iterations=100000 --threads=4 >> lab2_add.csv

./lab2_add --iterations=10 --threads=8 >> lab2_add.csv
./lab2_add --iterations=20 --threads=8 >> lab2_add.csv
./lab2_add --iterations=40 --threads=8 >> lab2_add.csv
./lab2_add --iterations=80 --threads=8 >> lab2_add.csv
./lab2_add --iterations=100 --threads=8 >> lab2_add.csv
./lab2_add --iterations=1000 --threads=8 >> lab2_add.csv
./lab2_add --iterations=10000 --threads=8 >> lab2_add.csv
./lab2_add --iterations=100000 --threads=8 >> lab2_add.csv

./lab2_add --iterations=10 --threads=12 >> lab2_add.csv
./lab2_add --iterations=20 --threads=12 >> lab2_add.csv
./lab2_add --iterations=40 --threads=12 >> lab2_add.csv
./lab2_add --iterations=80 --threads=12 >> lab2_add.csv
./lab2_add --iterations=100 --threads=12 >> lab2_add.csv
./lab2_add --iterations=1000 --threads=12 >> lab2_add.csv
./lab2_add --iterations=10000 --threads=12 >> lab2_add.csv
./lab2_add --iterations=100000 --threads=12 >> lab2_add.csv

./lab2_add --iterations=10 --threads=2 --yield >> lab2_add.csv
./lab2_add --iterations=20 --threads=2 --yield >> lab2_add.csv
./lab2_add --iterations=40 --threads=2 --yield >> lab2_add.csv
./lab2_add --iterations=80 --threads=2 --yield >> lab2_add.csv
./lab2_add --iterations=100 --threads=2 --yield >> lab2_add.csv
./lab2_add --iterations=1000 --threads=2 --yield >> lab2_add.csv
./lab2_add --iterations=10000 --threads=2 --yield >> lab2_add.csv
./lab2_add --iterations=100000 --threads=2 --yield >> lab2_add.csv

./lab2_add --iterations=10 --threads=4 --yield >> lab2_add.csv
./lab2_add --iterations=20 --threads=4 --yield >> lab2_add.csv
./lab2_add --iterations=40 --threads=4 --yield >> lab2_add.csv
./lab2_add --iterations=80 --threads=4 --yield >> lab2_add.csv
./lab2_add --iterations=100 --threads=4 --yield >> lab2_add.csv
./lab2_add --iterations=1000 --threads=4 --yield >> lab2_add.csv
./lab2_add --iterations=10000 --threads=4 --yield >> lab2_add.csv
./lab2_add --iterations=100000 --threads=4 --yield >> lab2_add.csv

./lab2_add --iterations=10 --threads=8 --yield >> lab2_add.csv
./lab2_add --iterations=20 --threads=8 --yield >> lab2_add.csv
./lab2_add --iterations=40 --threads=8 --yield >> lab2_add.csv
./lab2_add --iterations=80 --threads=8 --yield >> lab2_add.csv
./lab2_add --iterations=100 --threads=8 --yield >> lab2_add.csv
./lab2_add --iterations=1000 --threads=8 --yield >> lab2_add.csv
./lab2_add --iterations=10000 --threads=8 --yield >> lab2_add.csv
./lab2_add --iterations=100000 --threads=8 --yield >> lab2_add.csv

./lab2_add --iterations=10 --threads=12 --yield >> lab2_add.csv
./lab2_add --iterations=20 --threads=12 --yield >> lab2_add.csv
./lab2_add --iterations=40 --threads=12 --yield >> lab2_add.csv
./lab2_add --iterations=80 --threads=12 --yield >> lab2_add.csv
./lab2_add --iterations=100 --threads=12 --yield >> lab2_add.csv
./lab2_add --iterations=1000 --threads=12 --yield >> lab2_add.csv
./lab2_add --iterations=10000 --threads=12 --yield >> lab2_add.csv
./lab2_add --iterations=100000 --threads=12 --yield >> lab2_add.csv

#2
./lab2_add --iterations=100 --threads=2 >> lab2_add.csv
./lab2_add --iterations=1000 --threads=2 >> lab2_add.csv
./lab2_add --iterations=10000 --threads=2 >> lab2_add.csv
./lab2_add --iterations=100000 --threads=2 >> lab2_add.csv

./lab2_add --iterations=100 --threads=8 >> lab2_add.csv
./lab2_add --iterations=1000 --threads=8 >> lab2_add.csv
./lab2_add --iterations=10000 --threads=8 >> lab2_add.csv
./lab2_add --iterations=100000 --threads=8 >> lab2_add.csv

./lab2_add --iterations=100 --threads=2 --yield >> lab2_add.csv
./lab2_add --iterations=1000 --threads=2 --yield >> lab2_add.csv
./lab2_add --iterations=10000 --threads=2 --yield >> lab2_add.csv
./lab2_add --iterations=100000 --threads=2 --yield >> lab2_add.csv

./lab2_add --iterations=100 --threads=8 --yield >> lab2_add.csv
./lab2_add --iterations=1000 --threads=8 --yield >> lab2_add.csv
./lab2_add --iterations=10000 --threads=8 --yield >> lab2_add.csv
./lab2_add --iterations=100000 --threads=8 --yield >> lab2_add.csv

#3
./lab2_add --iterations=10 --threads=1 >> lab2_add.csv
./lab2_add --iterations=100 --threads=1 >> lab2_add.csv
./lab2_add --iterations=1000 --threads=1 >> lab2_add.csv
./lab2_add --iterations=10000 --threads=1 >> lab2_add.csv
./lab2_add --iterations=100000 --threads=1 >> lab2_add.csv

#4
./lab2_add --threads=2 --iterations=10000 --yield --sync=m >> lab2_add.csv
./lab2_add --threads=4 --iterations=10000 --yield --sync=m >> lab2_add.csv
./lab2_add --threads=8 --iterations=10000 --yield --sync=m >> lab2_add.csv
./lab2_add --threads=12 --iterations=10000 --yield --sync=m >> lab2_add.csv
./lab2_add --threads=2 --iterations=10000 --yield --sync=c >> lab2_add.csv
./lab2_add --threads=4 --iterations=10000 --yield --sync=c >> lab2_add.csv
./lab2_add --threads=8 --iterations=10000 --yield --sync=c >> lab2_add.csv
./lab2_add --threads=12 --iterations=10000 --yield --sync=c >> lab2_add.csv
./lab2_add --threads=2 --iterations=1000 --yield --sync=s >> lab2_add.csv
./lab2_add --threads=4 --iterations=1000 --yield --sync=s >> lab2_add.csv
./lab2_add --threads=8 --iterations=1000 --yield --sync=s >> lab2_add.csv
./lab2_add --threads=12 --iterations=1000 --yield --sync=s >> lab2_add.csv

#5
./lab2_add --threads=1 --iterations=10000 >> lab2_add.csv
./lab2_add --threads=2 --iterations=10000 >> lab2_add.csv
./lab2_add --threads=4 --iterations=10000 >> lab2_add.csv
./lab2_add --threads=8 --iterations=10000 >> lab2_add.csv
./lab2_add --threads=12 --iterations=10000 >> lab2_add.csv
./lab2_add --threads=1 --iterations=10000 --sync=m >> lab2_add.csv
./lab2_add --threads=2 --iterations=10000 --sync=m >> lab2_add.csv
./lab2_add --threads=4 --iterations=10000 --sync=m >> lab2_add.csv
./lab2_add --threads=8 --iterations=10000 --sync=m >> lab2_add.csv
./lab2_add --threads=12 --iterations=10000 --sync=m >> lab2_add.csv
./lab2_add --threads=1 --iterations=10000 --sync=c >> lab2_add.csv
./lab2_add --threads=2 --iterations=10000 --sync=c >> lab2_add.csv
./lab2_add --threads=4 --iterations=10000 --sync=c >> lab2_add.csv
./lab2_add --threads=8 --iterations=10000 --sync=c >> lab2_add.csv
./lab2_add --threads=12 --iterations=10000 --sync=c >> lab2_add.csv
./lab2_add --threads=1 --iterations=10000 --sync=s >> lab2_add.csv
./lab2_add --threads=2 --iterations=10000 --sync=s >> lab2_add.csv
./lab2_add --threads=4 --iterations=10000 --sync=s >> lab2_add.csv
./lab2_add --threads=8 --iterations=10000 --sync=s >> lab2_add.csv
./lab2_add --threads=12 --iterations=10000 --sync=s >> lab2_add.csv

#list 1
./lab2_list --threads=1  --iterations=10 >> lab2_list.csv
./lab2_list --threads=1  --iterations=100 >> lab2_list.csv
./lab2_list --threads=1  --iterations=1000 >> lab2_list.csv
./lab2_list --threads=1  --iterations=10000 >> lab2_list.csv
./lab2_list --threads=1  --iterations=20000 >> lab2_list.csv

#list 2
./lab2_list --iterations=1 --threads=2 >> lab2_list.csv
./lab2_list --iterations=10 --threads=2 >> lab2_list.csv
./lab2_list --iterations=100 --threads=2 >> lab2_list.csv
./lab2_list --iterations=1000 --threads=2 >> lab2_list.csv

./lab2_list --iterations=1 --threads=4 >> lab2_list.csv
./lab2_list --iterations=10 --threads=4 >> lab2_list.csv
./lab2_list --iterations=100 --threads=4 >> lab2_list.csv
./lab2_list --iterations=1000 --threads=4 >> lab2_list.csv

./lab2_list --iterations=1 --threads=8 >> lab2_list.csv
./lab2_list --iterations=10 --threads=8 >> lab2_list.csv
./lab2_list --iterations=100 --threads=8 >> lab2_list.csv
./lab2_list --iterations=1000 --threads=8 >> lab2_list.csv

./lab2_list --iterations=1 --threads=12 >> lab2_list.csv
./lab2_list --iterations=10 --threads=12 >> lab2_list.csv
./lab2_list --iterations=100 --threads=12 >> lab2_list.csv
./lab2_list --iterations=1000 --threads=12 >> lab2_list.csv

 #i
./lab2_list --iterations=1 --threads=2 --yield=i >> lab2_list.csv
./lab2_list --iterations=2 --threads=2 --yield=i >> lab2_list.csv
./lab2_list --iterations=4 --threads=2 --yield=i >> lab2_list.csv
./lab2_list --iterations=8 --threads=2 --yield=i >> lab2_list.csv
./lab2_list --iterations=16 --threads=2 --yield=i >> lab2_list.csv
./lab2_list --iterations=32 --threads=2 --yield=i >> lab2_list.csv

./lab2_list --iterations=1 --threads=4 --yield=i >> lab2_list.csv
./lab2_list --iterations=2 --threads=4 --yield=i >> lab2_list.csv
./lab2_list --iterations=4 --threads=4 --yield=i >> lab2_list.csv
./lab2_list --iterations=8 --threads=4 --yield=i >> lab2_list.csv
./lab2_list --iterations=16 --threads=4 --yield=i >> lab2_list.csv
./lab2_list --iterations=32 --threads=4 --yield=i >> lab2_list.csv

./lab2_list --iterations=1 --threads=8 --yield=i >> lab2_list.csv
./lab2_list --iterations=2 --threads=8 --yield=i >> lab2_list.csv
./lab2_list --iterations=4 --threads=8 --yield=i >> lab2_list.csv
./lab2_list --iterations=8 --threads=8 --yield=i >> lab2_list.csv
./lab2_list --iterations=16 --threads=8 --yield=i >> lab2_list.csv
./lab2_list --iterations=32 --threads=8 --yield=i >> lab2_list.csv

./lab2_list --iterations=1 --threads=12 --yield=i >> lab2_list.csv
./lab2_list --iterations=2 --threads=12 --yield=i >> lab2_list.csv
./lab2_list --iterations=4 --threads=12 --yield=i >> lab2_list.csv
./lab2_list --iterations=8 --threads=12 --yield=i >> lab2_list.csv
./lab2_list --iterations=16 --threads=12 --yield=i >> lab2_list.csv
./lab2_list --iterations=32 --threads=12 --yield=i >> lab2_list.csv

 #d
./lab2_list --iterations=1 --threads=2 --yield=d >> lab2_list.csv
./lab2_list --iterations=2 --threads=2 --yield=d >> lab2_list.csv
./lab2_list --iterations=4 --threads=2 --yield=d >> lab2_list.csv
./lab2_list --iterations=8 --threads=2 --yield=d >> lab2_list.csv
./lab2_list --iterations=16 --threads=2 --yield=d >> lab2_list.csv
./lab2_list --iterations=32 --threads=2 --yield=d >> lab2_list.csv

./lab2_list --iterations=1 --threads=4 --yield=d >> lab2_list.csv
./lab2_list --iterations=2 --threads=4 --yield=d >> lab2_list.csv
./lab2_list --iterations=4 --threads=4 --yield=d >> lab2_list.csv
./lab2_list --iterations=8 --threads=4 --yield=d >> lab2_list.csv
./lab2_list --iterations=16 --threads=4 --yield=d >> lab2_list.csv
./lab2_list --iterations=32 --threads=4 --yield=d >> lab2_list.csv

./lab2_list --iterations=1 --threads=8 --yield=d >> lab2_list.csv
./lab2_list --iterations=2 --threads=8 --yield=d >> lab2_list.csv
./lab2_list --iterations=4 --threads=8 --yield=d >> lab2_list.csv
./lab2_list --iterations=8 --threads=8 --yield=d >> lab2_list.csv
./lab2_list --iterations=16 --threads=8 --yield=d >> lab2_list.csv
./lab2_list --iterations=32 --threads=8 --yield=d >> lab2_list.csv

./lab2_list --iterations=1 --threads=12 --yield=d >> lab2_list.csv
./lab2_list --iterations=2 --threads=12 --yield=d >> lab2_list.csv
./lab2_list --iterations=4 --threads=12 --yield=d >> lab2_list.csv
./lab2_list --iterations=8 --threads=12 --yield=d >> lab2_list.csv
./lab2_list --iterations=16 --threads=12 --yield=d >> lab2_list.csv
./lab2_list --iterations=32 --threads=12 --yield=d >> lab2_list.csv

 #il
./lab2_list --iterations=1 --threads=2 --yield=il >> lab2_list.csv
./lab2_list --iterations=2 --threads=2 --yield=il >> lab2_list.csv
./lab2_list --iterations=4 --threads=2 --yield=il >> lab2_list.csv
./lab2_list --iterations=8 --threads=2 --yield=il >> lab2_list.csv
./lab2_list --iterations=16 --threads=2 --yield=il >> lab2_list.csv
./lab2_list --iterations=32 --threads=2 --yield=il >> lab2_list.csv

./lab2_list --iterations=1 --threads=4 --yield=il >> lab2_list.csv
./lab2_list --iterations=2 --threads=4 --yield=il >> lab2_list.csv
./lab2_list --iterations=4 --threads=4 --yield=il >> lab2_list.csv
./lab2_list --iterations=8 --threads=4 --yield=il >> lab2_list.csv
./lab2_list --iterations=16 --threads=4 --yield=il >> lab2_list.csv
./lab2_list --iterations=32 --threads=4 --yield=il >> lab2_list.csv

./lab2_list --iterations=1 --threads=8 --yield=il >> lab2_list.csv
./lab2_list --iterations=2 --threads=8 --yield=il >> lab2_list.csv
./lab2_list --iterations=4 --threads=8 --yield=il >> lab2_list.csv
./lab2_list --iterations=8 --threads=8 --yield=il >> lab2_list.csv
./lab2_list --iterations=16 --threads=8 --yield=il >> lab2_list.csv
./lab2_list --iterations=32 --threads=8 --yield=il >> lab2_list.csv

./lab2_list --iterations=1 --threads=12 --yield=il >> lab2_list.csv
./lab2_list --iterations=2 --threads=12 --yield=il >> lab2_list.csv
./lab2_list --iterations=4 --threads=12 --yield=il >> lab2_list.csv
./lab2_list --iterations=8 --threads=12 --yield=il >> lab2_list.csv
./lab2_list --iterations=16 --threads=12 --yield=il >> lab2_list.csv
./lab2_list --iterations=32 --threads=12 --yield=il >> lab2_list.csv

 #dl
./lab2_list --iterations=1 --threads=2 --yield=dl >> lab2_list.csv
./lab2_list --iterations=2 --threads=2 --yield=dl >> lab2_list.csv
./lab2_list --iterations=4 --threads=2 --yield=dl >> lab2_list.csv
./lab2_list --iterations=8 --threads=2 --yield=dl >> lab2_list.csv
./lab2_list --iterations=16 --threads=2 --yield=dl >> lab2_list.csv
./lab2_list --iterations=32 --threads=2 --yield=dl >> lab2_list.csv

./lab2_list --iterations=1 --threads=4 --yield=dl >> lab2_list.csv
./lab2_list --iterations=2 --threads=4 --yield=dl >> lab2_list.csv
./lab2_list --iterations=4 --threads=4 --yield=dl >> lab2_list.csv
./lab2_list --iterations=8 --threads=4 --yield=dl >> lab2_list.csv
./lab2_list --iterations=16 --threads=4 --yield=dl >> lab2_list.csv
./lab2_list --iterations=32 --threads=4 --yield=dl >> lab2_list.csv

./lab2_list --iterations=1 --threads=8 --yield=dl >> lab2_list.csv
./lab2_list --iterations=2 --threads=8 --yield=dl >> lab2_list.csv
./lab2_list --iterations=4 --threads=8 --yield=dl >> lab2_list.csv
./lab2_list --iterations=8 --threads=8 --yield=dl >> lab2_list.csv
./lab2_list --iterations=16 --threads=8 --yield=dl >> lab2_list.csv
./lab2_list --iterations=32 --threads=8 --yield=dl >> lab2_list.csv

./lab2_list --iterations=1 --threads=12 --yield=dl >> lab2_list.csv
./lab2_list --iterations=2 --threads=12 --yield=dl >> lab2_list.csv
./lab2_list --iterations=4 --threads=12 --yield=dl >> lab2_list.csv
./lab2_list --iterations=8 --threads=12 --yield=dl >> lab2_list.csv
./lab2_list --iterations=16 --threads=12 --yield=dl >> lab2_list.csv
./lab2_list --iterations=32 --threads=12 --yield=dl >> lab2_list.csv

#3
./lab2_list --iterations=1 --threads=32 --yield=dl --sync=m >> lab2_list.csv
./lab2_list --iterations=10 --threads=32 --yield=dl --sync=m >> lab2_list.csv
./lab2_list --iterations=100--threads=32 --yield=dl --sync=m >> lab2_list.csv
./lab2_list --iterations=1000 --threads=32 --yield=dl --sync=m >> lab2_list.csv

./lab2_list --iterations=1 --threads=12 --yield=dl --sync=m >> lab2_list.csv
./lab2_list --iterations=10 --threads=12 --yield=dl --sync=m >> lab2_list.csv
./lab2_list --iterations=100 --threads=12 --yield=dl --sync=m >> lab2_list.csv
./lab2_list --iterations=1000 --threads=12 --yield=dl --sync=m >> lab2_list.csv

./lab2_list --iterations=1 --threads=32 --yield=dl --sync=s >> lab2_list.csv
./lab2_list --iterations=10 --threads=32 --yield=dl --sync=s >> lab2_list.csv
./lab2_list --iterations=100--threads=32 --yield=dl --sync=s >> lab2_list.csv
./lab2_list --iterations=1000 --threads=32 --yield=dl --sync=s >> lab2_list.csv

./lab2_list --iterations=1 --threads=12 --yield=dl --sync=s >> lab2_list.csv
./lab2_list --iterations=10 --threads=12 --yield=dl --sync=s >> lab2_list.csv
./lab2_list --iterations=100 --threads=12 --yield=dl --sync=s >> lab2_list.csv
./lab2_list --iterations=1000 --threads=12 --yield=dl --sync=s >> lab2_list.csv

#4
./lab2_list --threads=1 --iterations=1000 --sync=m >> lab2_list.csv
./lab2_list --threads=2 --iterations=1000 --sync=m >> lab2_list.csv
./lab2_list --threads=4 --iterations=1000 --sync=m >> lab2_list.csv
./lab2_list --threads=8 --iterations=1000 --sync=m >> lab2_list.csv
./lab2_list --threads=12 --iterations=1000 --sync=m >> lab2_list.csv
./lab2_list --threads=16 --iterations=1000 --sync=m >> lab2_list.csv
./lab2_list --threads=24 --iterations=1000 --sync=m >> lab2_list.csv
./lab2_list --threads=1 --iterations=1000 --sync=s >> lab2_list.csv
./lab2_list --threads=2 --iterations=1000 --sync=s >> lab2_list.csv
./lab2_list --threads=4 --iterations=1000 --sync=s >> lab2_list.csv
./lab2_list --threads=8 --iterations=1000 --sync=s >> lab2_list.csv
./lab2_list --threads=12 --iterations=1000 --sync=s >> lab2_list.csv
./lab2_list --threads=16 --iterations=1000 --sync=s >> lab2_list.csv
./lab2_list --threads=24 --iterations=1000 --sync=s >> lab2_list.csv
