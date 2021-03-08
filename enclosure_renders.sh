#!/bin/sh

mkdir -p renders

front_view="--camera=-100,-100,100,30.5,19.5,0"
rear_view="--camera=161,139,100,30.5,19.5,0"

front_view_underside="--camera=-100,-100,-100,30.5,19.5,0"

enclosure_only="-Drender_pcb=false"
pcb_only="-Drender_enclosure=false"

openscad "$front_view" -o renders/full_front.png enclosure.scad
openscad "$rear_view" -o renders/full_rear.png enclosure.scad

openscad "$front_view" "$pcb_only" -o renders/pcb_top.png enclosure.scad
openscad "$front_view_underside" "$pcb_only" -o renders/pcb_bottom.png enclosure.scad

openscad "$front_view" "$enclosure_only" -o renders/enclosure_front.png enclosure.scad
openscad "$rear_view" "$enclosure_only" -o renders/enclosure_rear.png enclosure.scad
