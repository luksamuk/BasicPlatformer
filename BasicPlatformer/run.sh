#!/bin/bash
if [ $# -eq 0 ]
  then
    ../bin/Debug/SonicEngine
else
    if [ "$1" == "debug" ]
      then
        ../bin/Debug/SonicEngine
    elif [ "$1" == "release" ]
      then
        ../bin/Release/SonicEngine
    else
        echo Couldn\'t understand target \"$1\".
        echo Use either \"debug\" or \"release\".
    fi
fi