all:
	c++ -Wall -Wextra -Werror *.cpp -o reforge
debug:
	c++ -Wall -Wextra -Werror -DDEBUG *.cpp -o reforge
clean:
	rm -f reforge