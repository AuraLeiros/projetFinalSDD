# projetFinalSDD
Projet final SDD
 
Anna Stanischewsky\
Aura Leirós García



### ...

#### hachage.c

- Implementation du hash

We implement the FNV-1a as our hashing function, designed for speed and high-collision at expense of the security (that is negligeable at our use case), we use the 64-bit (uint64) implementation that should work in all systems but windows compiled with Microsoft Visual C++ (that compiles using 32-bit). The use of the prime number specifically for the 64 version allows for avalanche effect that maximizes the possibility of uniqueness.

The idea behind the algorithm is that in every iteration of the loop 

~~~c
while (*str){
    hash ^= (uint8)(*str++);
    hash *= PRIME_NUMBER
}
~~~

The first line "XORes" to mix bits, so that every small change implies a bit change in the hash, that is augmented by the second line that does an avalanche effect (spreading around bits with the maximum potential).


