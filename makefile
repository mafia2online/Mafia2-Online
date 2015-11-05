all:
	make -C Libraries/sqlite
	make -C Libraries/tinyxml
	make -C Libraries/squirrel
	make -C Libraries/RakNet/Source
	make -C Server

clean:
	make -C Libraries/sqlite clean
	make -C Libraries/tinyxml clean
	make -C Libraries/squirrel clean
	make -C Libraries/RakNet/Source clean
	make -C Server clean
