daemonize-win
=============

Daemonize processes under windows with ease.

Under Unix and the likes we can easily daemonize processes with

    command &

but under Microsoft Windows, there is no such easil facility.


Usage
=====

    daemonize.exe COMMAND [ARGUMENTS]
    
That's it...wait, there is something I have to tell you, it will
not do any PATH lookups, the path to the command needs the be relative or
absolute...sorry for that.


Bonus
=====

Also daemonizes GUI applications, the main window will be hidden.
