for nazwa in "$@"
do
    if [ ! -e "$nazwa" ];
    then
	    echo "$nazwa: plik nie istnieje"
    elif [ -f "$nazwa" ];
    then
	    echo "$nazwa: plik istnieje"
    elif [ -d "$nazwa" ];
    then
	    if [ -s "$nazwa" ];
	    then
		    echo "$nazwa: katalog"
	    else
		    echo "$nazwa: katalog pusty"
	    fi
    fi
done
