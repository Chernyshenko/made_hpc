#!/bin/bash
FILE=Linux
if [ -f "$FILE" ]; then
    echo "course"
else 
    echo "very easy"
    touch "$FILE"
    echo "course is very easy" > $FILE
fi
