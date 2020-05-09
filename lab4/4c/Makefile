#ME: Darshini Gupta
#EMAIL: darshinigupta0711@gmail.com
#ID: 004928411

files = lab4c_tls.c lab4c_tcp.c README Makefile
default:
	gcc -g lab4c_tcp.c -o lab4c_tcp -lmraa -lm -Wall -Wextra
	gcc -g lab4c_tls.c -o lab4c_tls -lmraa -lm -lssl -lcrypto -Wall -Wextra

clean:
	rm -rf lab4c_tcp lab4c_tls
	rm -rf  lab4c-004928411.tar.gz

dist:
	tar -czvf lab4c-004928411.tar.gz $(files)

