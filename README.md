# About
This repository contains work from my university course 'real time systems - operating systems'

Each folder corelates to tasks from single laboratories. Some of them are also containing subdirectories for work done at the time of laboratory and after.

## Contents of each folder

### LAB_01
**prosty_skrypt_1** - Creates 'probny_plik' and writes given argument to it
**prosty_skrypt_2** - Delets 'probny_plik'

### LAB_02
**powitanie.sh** - Greets user differently on working days and weekends
**policz.sh** - Counts files in current directory. If given argument **-a** counts also hidden files.
**lista.sh** - Prints each given argument with information if there exists file with that name. If there is directory with given name then it prints given argument (directory name) and if it's empty.
**szukaj.sh** - Finds all files in given path, of given extension, and  edited in given amount of days before, then creates tar archive from them with specified name. Needs arguments: path, extension, amount of days since edited, archive name. Might not work on evry system since it uses -readable argument in find.

### LAB_03
Scripts and programs to showcase praent/child process, self multiplying, taking processor resources by constatnly making calculations, signal comunication and handling and pipelines.

### LAB_05
'hello world' programs to familiarize with strace/ltrace and truss (solaris) and syscalls.

### LAB_06
Programs showcasing comunication through pipes (files as arguments).

### LAB_07
Program crating a chatting room through shared memory. Lots of VT100/ASCII escape codes for fun although they are coded into program in a way that may make your eyes bleed.

### LAB_08
Programs for familiarizing with multi-threaded programming. Primes - program that counts sum of N consecutive prime numbers, one program single threaded one for given amount of threads. Multithreaded counts exact sum.

### LAB_09
We were given some programs with bugs to diagnose.
Dipontelini - multithreaded programming task; Vivaldini has to divide plots left by Dipontelini to his sons, sons have same chance to get a plot by drawing lots. If they draw free plot thay take it and send information to Vivaldini if its taken they loose their chance.

### LAB_10
Multithreading exercises - password cracking programs, dictionary attacks.


