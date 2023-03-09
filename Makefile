
.PHONY: clean
debug: vigenere.c
	gcc -Wall -g -o $@ $<
vigenere: vigenere.c
	gcc -Wall -o $@ $<

test: test.py
	python3 ./test.py | tee ./tests/test_results.log

dummy: dummy.py
	python3 ./test.py -T

clean:
	@rm -fr vigenere ./tests
