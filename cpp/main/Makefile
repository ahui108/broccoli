all: mainc.bin mainc++.bin
	@echo "Done"

mainc.bin: main.c
	@echo "$\@target set: '"$@"'"
	@echo "$\%target lib name:'"$%"'"
	@echo "$\<dep: '"$<"'"
	@echo "$\?newer dep: '"$?"'"
	@echo "$\^dep wo dup: '"$^"'"
	@echo "$\+dep wt dup: '"$+"'"

    gcc -E $?

	gcc -g -o $@ $?

mainc++.bin: main.c
	g++ -g -o $@ $?

clean:
	rm -rf mainc.bin mainc++.bin
