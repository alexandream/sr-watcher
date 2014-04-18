screen-resolution-watcher
==========================

Small utility that listens for X11 resolution changes and blocks until such
event happens.

Mainly developed because I needed a way to automatically update some settings
upon guest operating system (under virtualbox) screen resizing.

Right now, this is how I'm using it

    while ~/.bin/wait-for-resolution-change
    do 
    	nitrogen --restore &&
    	lxpanelctl restart &&
       	sleep 1s; 
    done&

Its only dependency is xlibs, and I've merely compiled it with

    gcc wait-for-resolution-change.c `pkg-config --cflags --libs x11` \
     -o wait-for-resolution-change
