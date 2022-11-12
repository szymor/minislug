# Builds the dynamic lib.
rm *.o libymlib.so 
g++ -c -fPIC -O3 -Wall -Wno-write-strings -s -L/usr/lib -DNDEBUG digidrum.cpp -o digidrum.o
g++ -c -fPIC -O3 -Wall -Wno-write-strings -s -L/usr/lib -DNDEBUG Ym2149Ex.cpp -o Ym2149Ex.o
g++ -c -fPIC -O3 -Wall -Wno-write-strings -s -L/usr/lib -DNDEBUG YmMusic.cpp -o YmMusic.o
g++ -c -fPIC -O3 -Wall -Wno-write-strings -s -L/usr/lib -DNDEBUG LzhLib.cpp -o LzhLib.o
g++ -c -fPIC -O3 -Wall -Wno-write-strings -s -L/usr/lib -DNDEBUG YmLoad.cpp -o YmLoad.o
g++ -c -fPIC -O3 -Wall -Wno-write-strings -s -L/usr/lib -DNDEBUG YmUserInterface.cpp -o YmUserInterface.o
g++ -shared -o libymlib.so *.o
