# Builds the static lib 'libymlib.a'.
rm *.o libymlib.a
g++ -c -O3 -Wall -Wno-write-strings -s -L/usr/lib -DNDEBUG digidrum.cpp -o digidrum.o
g++ -c -O3 -Wall -Wno-write-strings -s -L/usr/lib -DNDEBUG Ym2149Ex.cpp -o Ym2149Ex.o
g++ -c -O3 -Wall -Wno-write-strings -s -L/usr/lib -DNDEBUG YmMusic.cpp -o YmMusic.o
g++ -c -O3 -Wall -Wno-write-strings -s -L/usr/lib -DNDEBUG LzhLib.cpp -o LzhLib.o
g++ -c -O3 -Wall -Wno-write-strings -s -L/usr/lib -DNDEBUG YmLoad.cpp -o YmLoad.o
g++ -c -O3 -Wall -Wno-write-strings -s -L/usr/lib -DNDEBUG YmUserInterface.cpp -o YmUserInterface.o
ar -r -s libymlib.a *.o
