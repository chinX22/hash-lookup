This is a program that makes use of a custom hash table to let the user add and find data using a user-generated
hex key, it will also print the contents of the table and how many probes from collisions there are

compile: g++ hash.cpp main.cpp
Usage ./a.out table_size hash_function(Last7|XOR) collision_method(Linear|Double)

Commands
A key val   - Add key/value pair to table
F key       - Find the value of the key in the table
P           - Print the total probes needed to find all keys
Q           - Quit
?           - Print commands list