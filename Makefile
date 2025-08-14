all:
	cc catone.c -o catone

install:
	make
	sudo cp catone /usr/local/bin/catone
	sudo cp catone.1 /usr/local/share/man/man1/catone.1 2>/dev/null || echo "Manpage not installed."
	sudo mandb || echo "Manpages not updated."
	make clean

uninstall:
	sudo rm -f /usr/local/bin/catone
	sudo rm -f /usr/local/share/man/man1/catone.1
	make clean

clean:
	rm -f catone
