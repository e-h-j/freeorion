http://freeorion.psitronic.de/

This is a mainly statically linked version of FreeOrion for
(i386) GNU/Linux. You won't need to compile any sources.

Download the "latest.tar.gz", to receive the latest version or
try the nightly builds in "download/nightly".

A discussion can be followed here:
http://www.freeorion.org/forum/viewtopic.php?t=1792

Pleas report bugs on the forums here:
http://www.freeorion.org/forum/viewforum.php?f=25


1. Install
============
- Download the latest archive by
   executing the following commands in a console / command-line prompt:
     cd /tmp
     wget http://freeorion.psitronic.de/download/latest.tar.gz
   or downloading it with your browser and put it into /tmp

- Use graphical installer:
   Unpack latest.tar.gz and run "setup.sh". The graphical
   installer will guide you.
   This will install a menu-entry and, if installing as root,
   a binary "freeorion-start" into your path.
     cd /tmp
     tar xfz latest.tar.gz
     sudo freeorion/setup.sh


   Or you may unpack latest.tar.gz anywhere on your system
   and use it directly. e.g.:
     cd /usr/local/games
     tar xfz /tmp/latest.tar.gz
  Then you can execute /usr/local/games/freeorion/freeorion
  to run the game


2. Problems?  
============
Any problem according this static release should be reportet here:
http://www.freeorion.org/forum/viewtopic.php?t=1792
Please do report gameplay errors in other forum parts!

Or join IRC:
Server:  irc.freenode.net
Channel: #freeorion

Debugging:
To run freeorion in gdb, you can execute the wrapper script
with parameter --gdb. It sets env vars and runs gdb.
You may also download the debugging symbols for nightly
builds on http://freeorion.psitronic.de/download/nightly/ .



3. Sources
============
Can be found here:
http://sourceforge.net/svn/?group_id=75752

Some sources used to statically link FreeOrion for linux
can be found in the subdirectory "src" here.

Linux installer scripts can be found in subdirectory 
loki_setup inside the FreeOrion svn dir.



4. Licensing
============
Artistic or creative content is licensed under the Creative Commons
Attribution-Share-Alike 2.5 license.
(http://creativecommons.org/licenses/by-sa/2.5/)
This includes artwork, music, sounds, text that determines how art,
music or sound is played or displayed in game, game content such as
technologies, buildings, specials and their associated in-game
text, and the scripts that determine their in-game functionality.

Game content scripts are also licensed under the GNU General Public
License, Version 2 (http://www.gnu.org/licenses/licenses.html).

Everything else is licensed under the GNU General Public License,
Version 2.
