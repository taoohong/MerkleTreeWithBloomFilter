test: ./test/test.cpp ./src/md5.cpp ./src/bloomfilter.cpp ./src/murmurhash2.cpp ./src/merkle_tree.cpp
	g++ ./test/test.cpp ./src/md5.cpp ./src/bloomfilter.cpp ./src/murmurhash2.cpp ./src/merkle_tree.cpp -o test

.PHONY:clean
clean:
	rm -f *.o test