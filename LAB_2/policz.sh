if [ $@ = "-a" ]
then
    echo "tak"
    liczba_linijek=$(ls -1A | wc -l)
    echo $liczba_linijek
else
    echo "nie"
    liczba_linijek=$(ls -1 | wc -l)
    echo $liczba_linijek
fi

