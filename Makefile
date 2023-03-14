.PHONY: clean

vigenere: vigenere.c
	gcc -Wall -o $@ $<

tests: tests.py
	mkdir -p ./tests && python3 ./tests.py | tee ./tests/test_results.log

dummy: dummy.py
	python3 ./tests.py -T

clean:
	@rm -fr vigenere ./tests
