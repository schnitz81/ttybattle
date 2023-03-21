if ! [ -e beings ]; then
	echo "No executable found. Preparing to build..."; sleep .6
	echo -n "Checking gcc..."; sleep .4
	if [ -z "$(which gcc)" ]; then
		echo -e "failed!\n\nMake sure gcc is installed.\n"
		exit 1
	fi
	echo "success!"; sleep .4

	echo -n "Checking curses..."; sleep .4
	if [[ ! -n $(find /usr/* -name 'curses.h') ]]; then
		echo -e "failed!\n\ncurses.h not found. Make sure curses (most often ncurses) is installed.\n"
		exit 1
	fi
	echo "success!"; sleep .4

	echo "Proceeding to build executable..."; sleep .6

	echo 'Building...'
	gcc -Wall -c main.c &&
	gcc -Wall -c actions.c &&
	gcc -Wall -c cpu.c &&
	gcc -Wall -c gameengine.c &&
	gcc -Wall -c menus.c &&
	gcc -o ttybattle main.o actions.o cpu.o gameengine.o menus.o -lcurses -lmenu &&

	if [ $? -eq 0 ]; then
		echo "Build successful. Starting executable..."; sleep 1
		./ttybattle
	else
		echo "Build failed. Unable to start."
	fi
	
else
	echo -e "\nExecutable found. Starting..."; sleep 1
	./ttybattle
fi



