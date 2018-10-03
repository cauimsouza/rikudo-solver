if [ ! -d lib ]; then
	echo "Downloading cryptominisat"
	mkdir lib
	wget -O lib/cryptominisat https://github.com/msoos/cryptominisat/releases/download/5.6.5/cryptominisat5-linux-amd64-gauss
	chmod +x lib/cryptominisat
fi

if [ ! -d bin ]; then
	echo "Compile first"
else
	cd bin/
	java -jar gui.jar
	rm *.txt
	cd ..
fi
