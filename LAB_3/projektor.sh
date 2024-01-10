#!/bin/sh

licznik=0

usr1_func() {
    licznik=$((licznik+1))
    if [ $licznik -gt 1 ]
    then
        exit 1
    else
        echo "na pewno zakonczyc?"
        sleep 5
        licznik=$((licznik-1))
    fi
}


trap '' FPE
trap 'echo "WINDOWS CHANGE - redraw"' WINCH
trap 'usr1_func' USR1
trap 'echo "nie zabijaj O_O"' KILL


while [ 1 ]
do
    echo "projektor: dzialam"
    sleep 5
done
