all:
	c++ *.cpp -o reforge
debug:
	c++ -Wall -Wextra -Werror -g3 -DDEBUG *.cpp -o reforge
clean:
	rm -f reforge