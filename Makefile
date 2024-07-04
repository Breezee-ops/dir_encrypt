files := encrypt.cpp

encrypt: $(files)
	g++ encrypt.cpp -o encrypt

clean:
	rm -f encrypt
