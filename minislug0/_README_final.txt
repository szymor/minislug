
 Mini Metal Slug Project.
 Done by Clément CORDE.
 Contact: c1702@yahoo.com

 All graphical content (c) Nazca Corp., SNK and PlayMore.

 Using rips by (in no particular order): Gussprint, The Mad Soldier, Random Rebel Soldier, C2000MC/Futaokuu, Random Talking Bush, Kevin Huff, Grey Wolf Leader, Goemar, Magma Dragoon, AzRaezel/Dan, The_Protagonist, Tk big_mickey_k, Grim, @$$h0l3 (what a nickname is that!?), Crazy Slug, Xezan... and some 'unsigned' rips. Hope I'm not forgetting anyone.

 Now using ST-Sound by Leonard/OXG (http://leonard.oxg.free.fr/), and ST era music by David WHITTAKER.



> Foreword:

 Hi there,

 After several breaks (to code a Bomberman, to ready myself for exams, aso...), here is finally the final version of my little project.

 I didn't manage to reach the quality I wanted to (I think it's not that bad either), but I already spent a lot of time on this program, and at the moment, I don't have so much time left.

 The biggest weakpoint is the sound. I only have a few WAV files, and I don't master Audacity, so I did what I could. In addition to this, I know dick in sound programming. Update: Added ST-Sound for music.

 You'll have to excuse a bit of a 'lame' finish as well...



> Controls:

- On the main menu, F1 to switch: 1 credit / 3 credits / infinite credits. Attention though, this doesn't work ingame on purpose.

- F9: Toggle screen mode: Normal 320x224 / 2x 640x448 / TV2x 640x448.
- F10: Toggle full screen / Windowed.
- F12: Pause.

- Up, down, left, right: Move.
- Button A: Shot.
- Button B: Jump.
- Button C: Bomb.

- Down + Button B to exit a vehicle.

 Default keys are the arrows keys, 'V' for Button A, 'C' for Button B and 'X' for Button C.

 When I released the preview, I had comments stating that my key's choice was not really good. So I made a little tool to allow people to set up their own controls. It's a very simple tool, but I didn't want to spend several hours to make a configuration menu in the game. I think that will do anyway.

 I also added joystick support. I only did some tests with an old stick (with some broken buttons) lended by a co-worker, but it seems to work. If a joystick is plugged in, the configuration tool will ask you for the 3 buttons to use (right after the keyboard configuration). Be careful to keep the 4 directions and 3 buttons, and not to set up the jump on 'up', that would make the game impossible to play on some levels.

 Note: When using a 32 bits color mode (or worse, 24 bits), the 2x and TV2x modes seems to use a lot time. I got a feeling that the 16 bits (the game video buffer) to 24/32 bits conversion by SDL is rather time-consuming. For maximum speed, I think it's better to use a 16 bits color mode.



> "high.scr" file :

 Nope, this is not a screen saver, it's the high-scores file. You can delete it to reset the high-scores, a new file will be created.

 In the package, I put one of my scores on the first position. This score was obtained while playing without cheats.

 It's a little objective
 Dans le package, je vous ai mis un score à moi en première position, obtenu en jouant normalement sans cheats. Ca fait déjà un petit objectif même si je ne doute pas qu'on puisse faire mieux (Pour info, le score a été obtenu en finissant le jeu en 1 crédit).



> Useful files :

minislug.exe       - The game, exe for Windows (XP).
minislug_cfg.exe   - Configuration tool, exe for Windows (XP).
elf-minislug-bpp   - The game, elf for 32 bits Linux.
elf-minislug_cfg   - Configuration tool, elf for 32 bits Linux.

mslug.cfg          - Configuration fine (binary).
high.scr           - High-scores file (binary).



<End of File>
