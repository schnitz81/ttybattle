if ! [ -e ttybattle ]; then
	echo "No executable found. Preparing to build..."
	echo -n "Checking gcc..."
	if [ -z "$(which gcc)" ]; then
		echo -e "failed!\n\nMake sure gcc is installed.\n"
		exit 1
	fi
	echo "success!"

	echo -n "Checking curses..."
	if printf "#include <curses.h>\n" | gcc -E -x c - >/dev/null 2>&1; then
		echo "success!"
	else
		echo -e "failed!\n\ncurses.h header file not found. Make sure curses (most often ncurses) is installed.\n"; exit 1
	fi

	echo -n "Checking menu header..."
	if printf "#include <menu.h>\n" | gcc -E -x c - >/dev/null 2>&1; then
		echo "success!"
	else
		echo -e "failed!\n\nmenu.h header file not found. Dependency missing.\n"; exit 1
	fi

	echo "Proceeding to build executable..."

	echo 'Building...'
	gcc -Wall -c main.c &&
	gcc -Wall -c actions.c &&
	gcc -Wall -c cpu.c &&
	gcc -Wall -c gameengine.c &&
	gcc -Wall -c menus.c &&
	gcc -o ttybattle main.o actions.o cpu.o gameengine.o menus.o -lcurses -lmenu &&

	if [ $? -eq 0 ]; then
		echo "Build successful. Starting executable..."
		./ttybattle
	else
		echo "Build failed. Unable to start."
	fi

else
	echo -e "\nExecutable found. Starting..."
	./ttybattle
fi



