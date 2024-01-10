#!/bin/sh

echo "Wybierz instrukcje do wyslania:"
echo "1 - SIGFPE"
echo "2 - SIGWINCH"
echo "3 - SIGUSR1"
echo "4 - SIGKILL"
echo "5 - SIGSTOP"
echo "6 - SIGCONT"

read instrukcja

case "$instrukcja" in
  "1") kill -8 $1 ;;
  "2") kill -28 $1 ;;
  "3") kill -10 $1 ;;
  "4") kill -9 $1 ;;
  "5") kill -19 $1 ;;
  "6") kill -18 $1 ;;
  *) echo "niepoprawna instrukcja"
esac