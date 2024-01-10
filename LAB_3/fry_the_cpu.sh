#!/bin/bash

a=1
b=1

f=0

while [ 1 ]
do
	f=$(( a + b ))
	a=$b
	b=$f
done

