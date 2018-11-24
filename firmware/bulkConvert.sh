#!/bin/bash

ff=""
for f in *.TXT
do 
    ff="$ff -f $f"
done
gpsbabel -i nmea $ff -o gpx -F "TRACKS.GPX"
