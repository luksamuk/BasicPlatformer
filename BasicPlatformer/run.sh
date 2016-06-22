#!/bin/bash
if [ $# -eq 0 ]
  then
    ../bin/Debug/SonicEngine $2
else
    if [ "$1" == "debug" ]
      then
        ../bin/Debug/SonicEngine $2
    elif [ "$1" == "release" ]
      then
        ../bin/Release/SonicEngine $2
    else
        echo Couldn\'t understand target \"$1\".
        echo Use either \"debug\" or \"release\".
    fi
fi