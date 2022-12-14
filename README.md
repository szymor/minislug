# 2016/09/27
## Clement CORDE - c1702@yahoo.com

After some time, I finally decided to release my old sources. I'm not into coding anymore, I don't know what my sources are worth, but as I get messages from time to time from guys asking me how I did this or that, well, here it is.

As I said, I don't code anymore and I have bad memories. Therefore I don't know if I'll be able to answer questions...

I quickly tried to compile the game, it worked on the machine I'm on, i.e. 32 bits Debian 8.x.

So, what do we got here, and how should you proceed?


## Folders:
- minislug0	> The game

- config0	> Configuration utility

- edtile0	> Level editing tool

- exechk0	> Checksum setup for final binary program

- psdprot0	> PSD file scrambler

## Additional file:
slug_hiscore_entry0.tar.gz	> Alternate "menu.c" file, with high score name entry for machines without keyboard. Check lines 954 and 958 to 960.


## Building the game:
I used "make" and "Code::blocks", depending on what machine I was on. Please note that as the YM lib is written in C++, you'll have to use the g++ linker.

Step one, you have to build the YM library (by Leonard/OXG). Enter the "minislug0" folder, then the "ymlib" folder. I think it's better to use a static ".a" file, but one of my friends had to use a ".so" on a Mac system. You have a ".sh" and a Makefile for each version (make -f Makefile_<a|so>). Once it's done, copy the lib in the main folder.

Step two, back in the main folder, "make" the game. That's it, the "protection" is disabled by default, the game should start.

Default keys are: arrows for movement, "v" to shoot, "c" to jump and "x" to throw a grenade.

You don't like the default keys? No problem, just build and run the config utility. Run the utility in the game folder or copy the "mslug.cfg" file into the game folder. (Game has to be restarted!).


## Level editing tool:
Be careful with it. The tool works fine, but keep in mind that I designed it for my exclusive use. I made an online help with the F1 key, but it is far from being user friendly. I include it in the package so you can have a look at the levels, there are some checks on the data in the game, but it's nevertheless easy to have the game crashing with a bad edition!

There is a "command file" for the monsters parameters, named "cmd.txt", in the main folder. This file is also included inside the .edt files, in case it's destroyed or changed. If changes are made, the program will check for the parameters and "remap" the existing monsters' parameters. This has a limit though: if I remember correctly, the program doesn't check the paramters' names, so better add new parameters AFTER the existing ones!

The "edt" file format is included. Note that I got lazy at the end, but it's possible to strip some data to make the file lighter, if you have a space limit. (Crunching the files is of course also a possibility).


## A word about the "protection":
I made a kind of protection, not a copy protection, but only to make sure that someone would not poke some bytes somewhere and then complain that the game is not working, or even worse, spread a buggy version.

So, there is a checksum in the program file. Once the you "made" the file, run the exechk program on the binary, and you're done.

I turned off this security in the sources, to turn it on, uncomment the "define EXE_CHECKSUM" at line 1090 in file main.c.

There is also the same kind of trick on the graphic files. The file format used is Photoshop's PSD. The provided files are unscrambled. If you run the psdprot program on the PSD files, the files will be unreadable. The operation is reversible, so no worries.

And finally, there is also a checksum on the level files (.edt).


## Grabbing the sprites:
There are about 5000 sprites used in the game. Grabbing the sprites takes some time (well, a few seconds), so while making the game I decided to grab the sprites once and then to use binary files. This speeds up the loading a lot.

If you need to insert a new sprite, you'll have to turn on the sprite grabbing. This is done by setting  the "define SPR_SAVE" to 1 in the "sprites_inc.h" file. Launch the game, the sprites will be grabbed and saved (only in the current bitlength! i.e. 32 OR 64 bits!). Revert the value to 0 once your new sprite is grabbed.


## Final notes:
Have fun! :)

Oh, and if ever you make a build on an exotic platform, let me know, I'll put it online. (I have a big endian version somewhere, if needed).


# 2022/12/14
## vamastah aka szymor

Well, I noticed the port of this game on GCW0, so I thought it would be nice to see it on other platforms. I uploaded it to GitHub, cleaned the repository a bit, updated makefiles, etc.

It seems the game in the current stage of development will run only on 32 bit platforms. I had issues on running it on some computers, maybe sdl12-compat (SDL 1.2 runtime compatibility library using SDL 2.0) messes something up - it is not the first SDL 1.2 game I have troubles with to run on modern PCs.

In the end I hope to run it on MiyooCFW, RetroFW and other retro handheld systems. I do not promise anything though, I do not plan to spend all my time on gamedev.

--- EOF
