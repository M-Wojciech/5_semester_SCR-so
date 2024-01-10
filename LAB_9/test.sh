#!/bin/bash

sum=0

for i in {1..100}
do
  sum=$(($sum+$(./a.out | wc -l)))
done

echo "$sum"