objects = src/main.o src/system.o


atm : $(objects)
	cc -o atm $(objects)

clean :
	rm -f $(objects)
	rm -rf atm
