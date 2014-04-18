screen-resolution-watcher
==========================

**This is not fit for *production* use, whatever that may mean. No care at all
has been put into security, and the machine I run this thing on can handle
being fucked up, so I didn't quite worry about it. Use it at your own risk**

Small utility that listens for X11 resolution changes and blocks until such
event happens.

Mainly developed because I needed a way to automatically update some settings
upon guest operating system (under virtualbox) screen resizing.

Right now, there's two ways to use it: as a program which'll block until the
next resolution change occurs; or as a program which'll call another program
whenever a screen resolution change occurs;

When used as a simple waiting process, I used to use it like this:

    while ~/.bin/wait-for-resolution-change
    do 
    	nitrogen --restore &&
    	lxpanelctl restart &&
       	sleep 1s; 
    done&

After adding support for it to call the handler upon change, I started using
it like this:

    ~/.bin/wait-for-resolution-change 1 ~/.bin/on-resolution-change.sh&

This means it'll call on-resolution-change.sh upon screen resolution change and
it'll ignore every resolution change within 1 second of another.

Its only dependency is xlibs and a few POSIX library functions, and I've
merely compiled it with:

    gcc wait-for-resolution-change.c `pkg-config --cflags --libs x11` \
     -o wait-for-resolution-change -std=c99
