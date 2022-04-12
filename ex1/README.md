### linker attack

#### we intentionally added printed statements to show our work!

#### instructions:
    1. open two terminals.
    2. in one of the terminals- type ' export LD_LIBRARY_PATH="." '
    3. type ./sniff and ./secret in each of them.
    4. in ./secret one- type your password.
    5. type enter twice.
    6. see result.

in this assignment, we were instructed to hack the linker, so that's exactly what we did. 
we complied with our shared library as instructed named .so.6. then at the dynamic linker section of the binary file, we change the offset of the address by 5 (can see binary file by "hexedit ./secret").
then we used fget to read input straight from STDIN into our out char array. 
then we opened a raw socket with another file called sniff (that will represent our server) and sent the given password to it.
steal.c is the file that we stored our scanf in. 
