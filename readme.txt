
Disasteroids 3D is (C)2001 Thom Wetzel, Jr.

Copyright (c) 2001 Thom Wetzel, Jr.

This software is provided 'as-is', without any express 
or implied warranty. In no event will the authors be held 
liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any 
purpose, including commercial applications, and to alter it 
and redistribute it freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you 
	must not claim that you wrote the original software. If you 
	use this software in a product, an acknowledgment in the 
	product documentation would be appreciated but is not required.

2. Altered source versions must be plainly marked as such, and 
	must not be misrepresented as being the original software.

3. This notice may not be removed or altered from any source 
	distribution.
	

-----------------------------------------------------------
2002.02.20-24
-----------------------------------------------------------

Well, I finally gathered some motivation to get started on 
a console for Dis3d and future projects built off this code.

So far, the console doesn't do much but replace UpdateMessage()
with a con_printf() and con_dprintf() for developer messages.  
A user can also enter in a cvar and its value is displayed
on screen.


-----------------------------------------------------------
2002.01.15
-----------------------------------------------------------

Fixed a couple minor things:

1) Joystick x and y values were being displayed on-screen.
   This was some left-over debug code I forgot to remove 
   for the release version.  Rats.

2) Added a line in KillGLWindow() to make sure Dis3D wasn't
   leaving any TaskBar boxes hanging around after closure.



-----------------------------------------------------------
2001/07/19
-----------------------------------------------------------

Have fun!


-----------------------------------------------------------
2001/05/08
-----------------------------------------------------------

I ended up not releasing the game a couple weeks ago.  Things
came up at work and home and I was a little busy!

I took some time and went about implementing a mode that would 
use red/blue 3d glasses.  I ran into a problem involving the 
screen size, and the effect really didn't add much to the game,
so I commented out the lines that display the red/blue 
option in the display menu to get rid of it.  If anyone is
interested in seeing how I went about making it work, just
remove the comments in menus.cpp on lines 196 and 197.

I'm also adding controls so that Winamp (and other music
players) can be controlled from within the game.  Should
be cool.  ;)

-----------------------------------------------------------
2001/02/25
-----------------------------------------------------------

Added something stupid. When the ship rotates, it rolls a little. 
It isn't perfect, but it was something I always wanted to add.

I'm updating the source code for the first time in a long time.
There's a lot of new stuff (gamma, mouse, etc) that might be of 
interest to budding OpenGL dudes and dudettes out there in 
programming land.  Hope SOMEONE finds it useful.

Again, if you write me and don't get a response within a week, 
please resend your message.  My ISP denies this, but I totally 
believe that messages intended for me are getting lost somewhere...

Enjoy!  

The latest EXE will be released next weekend.  :)

ThomW


----------------------------------------------------------
2001/02/07
-----------------------------------------------------------

After a two month hiatus I decided to add some new things to 
the game to add some features that were in the back of my mind

* Color depth control
  
* Gamma/brightness/contrast controls 

There seems to be something wrong with the fullscreen toggle. 
After flipping back and forth a couple times it crashes something 
in KERNEL32.DLL. I'm going to try and look at it later when I get 
some time.  It's probably my video card drivers, though.


-----------------------------------------------------------
2000/12/13
-----------------------------------------------------------

Added code to make sure rocks don't get too dark, while 
maintaining the coloring from prior versions.
  
In windowed mode, the mouse is only acquired when necessary. 
Something's still screwy if the user switches to windowed 
from fullscreen - the mouse isn't released, but I looked at 
it, couldn't find a quick fix, so it's staying. *grumble* 

 
-----------------------------------------------------------
2000/12/09
-----------------------------------------------------------

I redid the resolutions screen as an option screen (I had 
this on my 'to-do list' from the last update).
  
I moved the shields display to lower left.
  
I changed the hard difficulty level. On hard, there should be 
twice as many rocks per level, and they should be quite a bit 
faster than on Normal. 


-----------------------------------------------------------
2000/12/05
-----------------------------------------------------------

I added the ability to map two keys to each action. It's a 
little funky, but if you have rhythm you can really get some 
awesomely fast shots going.
  
NeHe changed his OpenGL base code and I merged his changes into 
the game -- it was a minor thing in the fullscreen toggle code 
that makes a little more sense. He also made some changes so 
that the game doesn't claim 100% of the processor's time for 
itself when minimized. 


-----------------------------------------------------------
2000/12/03
-----------------------------------------------------------

Somewhere between games of Counter-Strike I found time to make 
the following changes. I'm addicted to that stupid game -- I 
was up playing until 3am. *zzz*

Mouse control implemented in DirectInput. It fixes the timing 
problems seen in the FPS display and hyperspace in the betas 
since mouse control was added. I tried to make it that the mouse 
is only acquired while the game is being played. While in menus 
or paused, the mouse should be released so that it can be used 
in other apps. 

I know I made more changes than that, but I can't remember what 
they are.


-----------------------------------------------------------
2000/11/27
-----------------------------------------------------------

Fixed volume at startup bug
  
Shields recharge between rounds - 2 * level% of total


-----------------------------------------------------------
2000/11/23 (Some of these may be duplicates)
-----------------------------------------------------------

Mouse buttons are now mappable. I need to find a way to let users 
map multiple keys to one action -- maybe like in Quake where two 
keys can have same action. Just to let you know, I used the 
Windows API to do it, so only three buttons are available. I could 
have used DirectInput, but it would have involved more work and the 
buttons wouldn't have been mappable.
  
Added code to catch Hyperspace Abusers. After hyperspacing 3 times, 
your chances of being warped into the center of a rock increase with 
each warp in a ten second period
  
Volume control - it only affects the in-game volume - not the master 
control - just like you asked ;)
  
Added graphic to make percentage displays in menu look cooler. Also 
cleaned up menus a little (very little in fact)
  
Shield power display using the boxes used in the menus
  

-----------------------------------------------------------
2000/11/23
-----------------------------------------------------------

	I've been playing around and added a bunch of cool 
	stuff in the last month or so.  

	I added code that controls the player ship's top speed, 
	so the ship won't be able to travel faster than one 
	screen height/width, so it won't be able to disappear

	Added player rotate speed adjust to control menu. Use 
	the arrow keys to change the value from 0% to 100%. 
	Setting the adjust value to 50% would set the rotation 
	speed to the original value. 

	Made a change to the collision detection routine - 
	there shouldn't be any rocks that you can fly and shoot 
	right through.

	Added mouse support - cursor shows up in window mode, but 
	rotation works fine.  Also added support in menu to enable 
	and control sensitivity. 

	Added volume control code to make it easier to listen
	to MP3's while playing Dis3d.  ;)

	Also added code to penalize a player for abuse of hyperspace.
	Your chances of warping into the middle of a rock increase
	everytime hyperspace is used in a 10 second period.  I
	totally cheated on this one.  I didn't put any of the variables
	into the actors type.  I just made a bunch of variables
	to handle this problem as quickly as possible.  :(
	
	Shields power display - using the boxes I added (oh 
	yeah - i forgot to add that above) to help make the menu's
	'slider' options look a little cooler.  I modified the 
	font.bmp to change two useless characters into one empty
	and one full box for slider controls, and thought they
	might make a cool way to display the shield power.

	I need to thank Agust "Nemesis1" for all of his excellent
	suggestions.  Many of these latest fixes were prompted by 
	his emails.  Thanks again, Agust!!!
	
	In fact, thanks to everyone who wrote concerning the most
	recent version.  I wrote a reponse to every message sent
	to me, and if you wrote and didn't get a reply.  Blame 
	my ISP.  :)


-----------------------------------------------------------
2000/10/22
-----------------------------------------------------------
	
	Added the top secret cheats menu.  You'll never 
	guess what the activation code is.  ;)

	I'm off to distribute the game to its 10 fans!  Enjoy!

-----------------------------------------------------------
2000/10/21
-----------------------------------------------------------

	Played with timing code.  I think I found a good way to
	position the timing updates in the program to jumps when
	another app's running in the background, and also think
	that vsync timing is better when mouse is moved over 
	window and keys are held.  I thought of this yesterday
	while I was driving home from Salisbury, MD, ending a 
	sucky two day four-hour (one way) commute.  :P

-----------------------------------------------------------
2000/10/19
-----------------------------------------------------------
	
	Changed some joystick stuff 
		* Mapped hyperspace to button 3
		* Changed shields from down to button 2
		
	Changed logo to color website version - it's sexier
	
	Made other little adjustments here and there.  I still 
	haven't even looked at the vsync stuff, but come hell
	or highwater, v1.2 is going out this weekend!!!
	
	Got another email today - this time from a French 
	game magazine that would like to publish the game on their
	cover disc.  Sweeet!
	
	
-----------------------------------------------------------
2000/10/18
-----------------------------------------------------------

	Long time no work.  In my last update I said I was releasing
	1.2 the next day -- uh, I guess that didn't happen ;)

	A lot of people have been contacting me about the game
	lately, so I thought it would probably be a good idea
	to release the stuff I've been working on lately.

	I'm making one more attempt at fixing the vsync problem
	before sending this stupid thing out the door, so to 
	speak.
	
	For fun, I finally fixed the stereo panning so that it 
	works correctly.  I must have been doing something wrong
	the whole time.  I tried adding a stereo seperation
	multiplier, started at 0.8 and worked down to 0.1.  
	Eureka!  At 0.1 it works awesome!  Killer.  ;)


-----------------------------------------------------------
2000/08/21
-----------------------------------------------------------

	Well, it's been a while since a release, and I've been
	sitting on a TON of new stuff.  I was at my nephews' house
	and fired up the most recently released version, 1.1 and 
	laughed at how lame it was compared with the version I've
	been playing for the last couple months.  It's nothing
	HUGE, just a bunch of little stuff that makes 1.2 so much
	better.
	
	I added a couple of things I've been meaning to just to be
	done.  Hyperspace and highscore difficulty/spacesize indicators.
	They're both hacks, but they're both features I wanted to 
	add before releasing 1.2.  Hopefully they won't cause any
	problems --- ;)
	
	Other than improving the two things I just added, the only 
	additional thing I'd love to do to the game is make it head-to-head
	on one PC.  Unfortunately it's not going to happen.  The main game
	code is just too messy and I don't feel like writing it all over 
	again.  I'm moving on and starting a new project, hopefully something
	I can release as shareware down the road.  Don't worry -- my goal
	is to make something cool that I will sell for less than $10 and 
	make it worth the fee...
	
	Oh well, I'm going to upload the files tonight and send 
	out the press releases, etc tomorrow.  
	

-----------------------------------------------------------
2000/07/16
-----------------------------------------------------------
	
	I've been spending the weekend going over the timing code,
	fixing up the menus, creating difficulty settings, and
	doing general cleanup in preparation of the next release
	of the game.  I'm going to send out copies of the game
	to some of the guys and see if they report any problems
	with the game before releasing it.


-----------------------------------------------------------
2000/07/13
-----------------------------------------------------------

	EUREKA!

	Well, looks like my suspicions were correct all along.

	Rendering > 700 fps is a total waste PLUS it was the
	reason why Dis3d ran so lousy with vsync enabled.  With
	the latest changes I made today, DrawGLScene is throttled
	back to update at 120fps while the SwapBuffer is updated
	at 60fps.  It works so well on my GeForce that I'm not 
	sure it's working *wink*.  It also works well on my P2/266
	equiped with a Voodoo2, so I must be on to something.  

	Another thing affecting the game was the keyboard repeat 
	rate.  Don't know why, but that problem has also been
	fixed by the rendering throttle.

	If anyone gets this code, compiles it and has any problems
	with the throttling LET ME KNOW.  I'm thinking that I may
	need to figure out some way of getting (or computing) the
	computer's refresh rate or capability and program the game
	to automatically throttle itself back for that machine. 
	I'm going to do some testing on a range of computers in my
	office next week to see if I need to be concerned about 
	trying to maintain 60 fps.

	If anyone plays around with the code and finds a way to
	improve it at all, LET ME KNOW.  This is a first attempt, 
	and someone may already know a better way of doing this.

	One question I have now, is should I be Sleep()ing every
	cycle so that Dis3d isn't hogging the processor?  If so,
	how long is cool...  I need to try this out. :)

	OTHER COOL STUFF

	1. MOTION BLUR
	This isn't ready for 'prime time' but it's amusing.
	It's only available if you change the '#if 1' in 
	DrawGLBackground() to '#if 0', so it's something only a
	few elite people who bother to recompile the source
	will ever get to see. ;)

	2.  WORLD ROTATION EXPERIMENT
	Instead of the ship rotating, everything else on the 
	screen is rotated around the player's ship while it
	stays pointing right on the screen.  This is another 
	developer's-exclusive and it's activated by setting 
	EnableVomitComet = 1 in DrawGLScene.  Within seconds
	you'll be able to see why this isn't going to be 
	implemented in a release of the game -- ha ha.

		

-----------------------------------------------------------
2000/07/11
-----------------------------------------------------------
	
	Well, I've been playing with a lot of things over the
	past week or so.  Made a big breakthrough today going
	through DrawGLScene.  I was using a lot of unnecessary
	calls throughout the main rendering routine, and have
	seen big strides made through the removal of these 
	useless OpenGL commands.  Very cool.

	Also seperated out a couple of routines for profiling
	purposes.  I wanted to make sure that drawing the 
	background or setting up lights wasn't taking longer
	than it should.  

	I'm still not happy with the vsync situation, and that
	is the only thing that I need to find a correction to
	before releasing a new version of the game.

	Started working on the new game in a big way the other
	night.  Modelling -- oof.


-----------------------------------------------------------
2000/06/27
-----------------------------------------------------------

	Removed all of the useless MatrixMode() calls.
	
	Changed the ship remaining display.  Instead of drawing
	the player ship model for every remaining life, I created
	a 32x32 image of the ship that I'm using instead.  That 
	should speed things up a little.  Redrawing the player's
	model was pretty piggish...  ;)

-----------------------------------------------------------
2000/06/25 - 26
-----------------------------------------------------------

	While running the game on Wan's laptop, we noticed that
	the game didn't 'stutter' while in Pre-Copernicus mode.
	I looked at the code and found that the space rendering
	code has useless MatrixMode calls all over the place.
	Removed those, and things seem pretty zippy now :)

	The stuttering has been bothering me for a while, and
	it really cool to find the cause of it!!!  :)

	Went through on the 26th and looked for useless GL calls...


-----------------------------------------------------------
2000/06/23 - 25
-----------------------------------------------------------

	Updated Dis3d's website in my spare time.  Looks 
	much cooler now ;)


-----------------------------------------------------------
2000/06/12 - 2000/06/22
-----------------------------------------------------------

	Working on getting save values implemented.  Had a 
	major rewrite of the menu system to integrate the Cvars
	into it.  Now each menu item can point to a Cvar so the
	menu's displays are tied directly to their corresponding
	variable.  Makes things much cleaner.  :)

	Figured out how to toggle Vsync.  Working great, even
	though some cards (ATI mobility) doesn't support the
	call...  ?  If anyone reads this and knows another way
	around this, let me know.  :)


-----------------------------------------------------------
2000/06/09 - 06/11
-----------------------------------------------------------

	I've spent much of the last several nights (and days)
working on a console variable system, similar to that used
by Quake and its offspring.  I got a functional system
working before peeking at the Q1 source code, and much to
my surpise, it looked pretty much like Quake's!!!  Way cool!
One thing I liked better about Quake's system were the
function names, so I kind of adopted those to my system. :)

	I still need to load in a cfg file and get the settings
into the game on startup, but I'm on my way :)

-----------------------------------------------------------
2000/06/03 - 06/08
-----------------------------------------------------------
	
	I worked on menu/key mapping code into the wee hours
	of the morning.  It works pretty nice, but there's one
	place in the menuing code that I'm not 100% happy with,
	but I can live with it.
	
	In future games, I'm going to implement a cvars system
	like JC did in Quake, and the problem I'm having with 
	the menu system will be eliminated, since each menu
	item can have a pointer to one of the cvars.  The way 
	the code is now, I'd have to switch all my BOOL's over
	to floats (or possibly ints) so that each menu item
	that displays a value could actually update itself
	based on the source variable's value... 
	
	Hmmm... it may happen yet *whimper*
	
	The outstanding things before 1.3 (or 2.0...?) are:
	
	1. Saving/retrieving settings to INI file
	2. Make sure all keystrokes now have menu options
	3. Look at doing 2-players on one keyboard mode
	
	If I get 2-player mode working, the next release will
	be labled 2.0 and will be the final rev of Dis3d.  If
	I don't it will be version 1.3.
	
	At this point(6/8/2000 2:00am) over 16,000 people
	have downloaded v1.1 from 3dfiles.com alone!  Woohoo!
	

-----------------------------------------------------------
2000/06/01 
-----------------------------------------------------------

	Well, my 'beta testers' are a bunch of slackers.  Not 
	ONE of them got back to me with feedback on the latest
	changes to the program...

	Oh well.  I guess I'm a slacker too ;)

	I removed and replaced Ryan Haski's Timing.c and timing.h
	files from the project and replaced them with nehe's
	new timing code from tutorial 21.  I didn't like having
	stuff in the code that wasn't 100% created for distribution
	in a project like this.  Thanks to Ryan for the old code
	and thanks to Jeff for the new stuff!  Cool.

	I'm compiling the program for release, and instead of being
	a sap about version numbers, I'm releasing this as 1.1.
	I should have been a man about it and released the initial
	version as 1.0, but I wimped out at the last minute and 
	called it 0.2.  Wimpy wimpy wimpy!

	I'm going to try and release this bad boy tonight!


-----------------------------------------------------------
2000/05/17 
-----------------------------------------------------------

	Finally replaced the lastkey/key copy for...loop
	with memcpy.  Thanks to whoever that was on gamedev.net's
	message board who answered my (admittedly) stupid question.
	I don't know why this didn't work, but in MSDN's help
	I SWEAR they show sizeof(one element) * sizeof(array)

	In other news, I finally made the SCREEN_ variables
	do something.  If you're screwing around and want to
	try the game in a larger world, try doubling the values
	of the WORLD_ height, width and their half values.  It 
	works, but it's not fun, and the offscreen rocks are 
	still being rendered, but it DOES work. :)

	Release?  Very soon -- as soon as my 'beta testers'
	get back to me ;)

-----------------------------------------------------------
2000/05/16 22:00 - 24:00
-----------------------------------------------------------
	
	I've been working the last couple of days on a model
	for the next game.
	
	Also been researching multiplayer code.  Fixed some 
	code on nate's site at scuzzy.net (SP?)
	
	Just released a pre-build exe to some people to get 
	feedback on the changes.  
	
	Added some more code to the lookup table routines.  Also
	visited nvidia's site and found fastmath routines that
	I want to roll into Dis3D.	

-----------------------------------------------------------
2000/05/11 9:00 10:30
-----------------------------------------------------------
	
	Moved Dis3D Dev Page to my LMNOpc server so I could
	add counter to see if anyone's visiting the stupid page

	Wrote update about lookup tables on Dev Page

	Implemented Marty R's code to fix the speed problem.
	Thanks a lot, Marty!  


-----------------------------------------------------------
2000/05/09 10:30 - 11:30
-----------------------------------------------------------

	Played around with optimizations.  
	
	I hope this is right, but I converted all functions'
	parameters to const references where I could.
	
	Converted some routines (like CollisionTest) to inline
	functions in cases where I know they're routines
	that aren't a lot of code but are called frequently
	
	Fixed little things as I found them.


-----------------------------------------------------------
2000/05/07 11:00-11:30
-----------------------------------------------------------

	Played around with a bunch of stuff today regarding
	timing.  Changed over all (i hope) of the code dealing
	with time (and there's lots of it) from long milliseconds
	to float seconds.  I'm doing this to try and prevent
	some of the rounding problems I've been having with
	super-small velocities getting rounded down to zero.
	I'm also hoping that it will be easier to work in 
	fractional seconds instead of milliseconds.  I'm 
	really comfortable in ms, since that's what all my 
	programs at work are based in, but I think that 
	float seconds may be the way to go here.  We'll see...
	
	Another stupid change to make when I'm trying to 
	ready a release version.  I should have waited until
	AFTER I made a release.  Nuts.
	
	CODE FREEZE from this point forward (yeah right)
	

-----------------------------------------------------------
2000/05/07 15:30-15:45
-----------------------------------------------------------
	Marty R. sent me an equation to use to calculate the 
	ship's drift.  Added it to code and merrily played the
	game for ten minutes.   Woo hoo!


-----------------------------------------------------------
2000/05/07
-----------------------------------------------------------

	I've been slacking lately, well programming wise.  There
	has been a little progress in the past couple of days,
	but I'm obviously not updating this file as frequently
	as I should be.
	
 	For the last couple of hours I was working on getting 
 	Jeff's latest OpenGL code base changes from nehe.gamedev.net
 	rolled into the code base and eliminating the problems
 	that were caused by the full screen/window toggle.
 	There's still some choppiness after toggling one way
 	to another, but it works. :)
 	
 	I'm also excited to send the latest version to some
 	of the beta guys, because I'm hoping the window class
 	termination will fix the crash on exit problem that
 	some people have been seeing.
 	
 	The latest release is suffering because I didn't really
 	have any clear objectives with this release when I started
 	making changes.  I'm going to try sending out pre-release
 	executables today to some of the guys that e-mailed me
 	about the game, and hopefully a new release will be 
 	available this week.

-----------------------------------------------------------
2000/05/02 - 10:00 11:00
-----------------------------------------------------------

	I'm having my usual problems with the DSL line...

	Playing and tweaking -- tweaking and playing...

	Answering e-mails...

	I think I'm ready for the second release.

-----------------------------------------------------------
2000/05/01 - 5:00 - 5:15
-----------------------------------------------------------

	Added flames to ship thrust.  It's cool looking, 
	even if it is unrealistic.  I KNOW that space is 
	a vacuum, okay?  :)
	
-----------------------------------------------------------
2000/04/30 - 23:00 - 00:30
-----------------------------------------------------------

	Jeff Molofee is my hero.  He sent me a code change
	that properly does the player-centric 2-layer
	texture backdrop.  Jeff also cleaned up my space 
	texture -- it looks awesome!  Thanks again, Jeff!
	
TODO: Well, there are only two things that I want to do
	before the next release:
	
	1) See if there's something I can do about the strange
		UFO travel in P/C mode

	2) Create a flag to make sure that the star data isn't
		being updated when there aren't any stars on the
		screen.
	
UPDATE! While writing #2's blurb I thought to myself
	'Self, you could have written the code in less time
	than it took to write this statement.  Duh!'  So
	after writing the statement, I went, added the flags,
	did a quick check to make sure everything worked,
	then came back and wrote THIS blurb.  Hopefully I
	will learn from these experiences and instead of 
	writing about it, JUST DO IT!

	In fact, I was just thinking that today.  I was 
	flipping through the latest Andre LaMonthe game
	programming book to see what he had to say about
	particle systems, and it turns out I do almost
	the exact same thing in my code as he writes in the
	book.  What does this tell me?  Stop buying those
	stupid computer books and just code.  Writing code
	is the only way to learn how to write code. 

	Duh.

	Now there's only one thing TODO before release...

	Stupid UFO!

	I'm going to update my remote code base and go to bed!

	Work tomorrow :(  


-----------------------------------------------------------
2000/04/30 - on and off through the day - about 4 hours
-----------------------------------------------------------
	
	Got the player-centric (Pre-Copernicus) mode working.
	My initial idea to scroll the space behind the player
	isn't going to work unless I do some thinking, so 
	I think we all know that's not going to happen :)

	I came up with an idea that I like MUCH better.  I'm
	using space mode 1 (mild motion) and sticking a star
	field on top of it.  The star code was VERY easy to 
	implement and it looks great (I hope you agree!)

	Played A LOT through the day as I got P/C mode working
	to see if it was worth keeping.  It turns out I like 
	it more than the 'normal' mode.  Cool!  
	
	I need to thank Marty R. for the suggestion.  Thanks!

-----------------------------------------------------------
2000/04/29 - 20:00 21:00
-----------------------------------------------------------

	Moved player code outside of actors[] group to 
	facilitate possibility of player-centric play mode
	where the player doesn't move from the center of the
	screen but everything moves around him.  We'll see.

	Also played a bit... of course ;)

PROBLEM: Noticed that the vapor trail from the player ship
	would go the wrong way.  

	Fixed it.  The old way stupid when all i had to do was
	add or subtract 180.  Duh!  Just so everyone knows, 
	I'm very stupid.


-----------------------------------------------------------
2000/04/29 - 17:00 - 18:30
-----------------------------------------------------------

PROBLEM: When Windows does something in the background and
	the player is thrusting, the ship ends up careening out
	of control accross the screen.

	GetTickCount() wasn't accurate enough.  It took a lot
	of digging around the MSDN to figure that out, but 
	QueryPerformanceTimer is (supposedly) 100% accurate.

	I tried writing my own timing routines using 
	QueryPerformanceTimer() but failed -- ended up using
	code from Ryan Haksi (see comment at top of timing.c
	for details)  Big thanks to him for the code!  I'm 
	going to have another stab at it, because I'd like 
	to do it My Way -- easier to read and understand 
	(sorry Ryan) :)


-----------------------------------------------------------
2000/04/29 - 15:00 15:30 
-----------------------------------------------------------
	
	Working on key bind data structure to allow 
	player to redefine keys to his/her liking.
	Got the structure setup, as well as text labels
	to help guide the key binding later.  The special
	mode keys are going to be a problem (frame rate display, etc.)
	so I'm going to have to figure out a way to work around
	that.  

	I definitely need to consider implementing a gamestate
	variable to store whether the help screen is up, a game
	is in session, if the user is entering their initials
	etc.  The current scheme is getting too complicated,
	but I'm worried about complications because of the
	house of cards I've built.  
	


	LESSON TO EVERYONE: Design software before programming.  



	If I would have thought that Disateroids would someday
	have highscore entry and a title screen, and possibly
	user-definable key mapping, the structure of the program 
	would be very different.  Very different indeed. :P

-----------------------------------------------------------
2000/04/29 - 9:30 - 11:50
-----------------------------------------------------------

	I spent most of the morning surfing the net for 
	information on fixing the timing skips and for info
	about gamma control.  I think the timing jumps may
	be caused by the use of GetTickCount() which is 
	inaccurate when rendering more than 100 frames/sec,
	which this game is doing.  I couldn't find any info
	about an easy way to implement gamma control, either :(

TO DO:	These are some things I'd like to fix/do/try before
		the next release of the game

	1. Gamma control
	2. Look into game speed problem when turning ship
	3. Try always spawning ship invincible
	4. Lower Thrust *
	5. Allow users to remap game keys
	6. Create new mode where ship stays centered on 
		screen and the objects move around it.  I'm
		not going to rotate the gameworld, just 
		translate everything on the X and Y

FUTURE:	Things I'm considering for future versions of the game

	1. Powerups hidden in saucers/rocks


	Now down to business...


PROBLEM: Found ancient bug -- thrust clicking noise

	Finally found a bug that's been bothering me ever
	since I implemented the sound (which was days into the
	project).  The player thrust sound always 'clicked',
	and it was caused by velocity * deltatime.  Since
	the game loop is running so fast, it is possible that
	deltatime was zero, causing velocity to equal zero.  
	The thrust sound is stopped when velocity == 0 and
	is played continuously when it is ! 0.  Basically the
	static was being caused by the thrust sound being
	turned on and off quickly.  Woo hoo!

	This really demonstrates my need for using a timer
	more accurate than GetTickCount().


PROBLEM: Player thrusts too fast.  

	This is actually subjective :) but if Gary Mc Taggart
	from Valve Software says it's too fast, it's too fast.
	(But then again, I got an e-mail from George W Bush 
	as well, so who knows if it's really Gary or some 
	l33t a$$h0l3.  Possibly the latter)

	I had to change the player thrust speed anyway (because
	of the ancient bug above), so I needed to get a new
	value for the acceleration anyway.  

	The ship has two thrust speeds - one for each difficulty
	setting.  The ship thrusts much slower on the easier
	difficulty setting.


-----------------------------------------------------------
2000/04/26 - 22:00 - 01:00
-----------------------------------------------------------

-----------------------------------------------------------
 I read over my earlier comments, and found that it was 
 hard to figure out why some of the changes were made, and 
 I was the one who made them!  I decided to adopt a 
 PROBLEM/SOLUTION style log so that other people might find
 it easier to track why changes were made when they were.
-----------------------------------------------------------

PROBLEM:  Player spawning in rocks

	There has been code in the program to make sure that
	this doesn't happen, but a bug crept into it.  I had
	code to detect collisions in three places:
		1. Saucer spawn
		2. Player spawn
		3. Main CollisionDetection routine
	
	The code in the player spawn routine was different 
	from the code in the other two routines and didn't 
	work correctly.  

	I added the CollisionTest function to prevent things
	like this from happening in the future.


PROBLEM:  Player invincibility at spawn not working

	I never noticed this because the player spawn routine
	wasn't properly detecting collisions, but if the player
	is unable to spawn for x seconds, the player is
	supposed to spawn with the shields up (invincible)
	for five seconds.

	Basically, it would have never worked before, because
	of a million different things.  It now works.


PROBLEM: ???

	I changed the position of g_nLevelNum's initialization
	in the code but I don't know why.  It took fifteen 
	minutes to get it working again, but I don't remember
	why I started messing with it in the first place.
	I hope it doesn't bork anything up...


PROBLEM: Hitting U a lot in attract mode then starting
	game would create situation where game would begin
	with just player and one stray rock

	There was a bug in FindFreeActor where the routine
	could find an actor that was one higher than UBound.
	Fixed it!


PROBLEM: Difficulty Levels unbalanced.  Easy mode too
	slow, normal mode gets too hard with increasingly
	faster rocks.

	Changed both of the difficulty modes:

	ORIGINAL RECIPE: Unchanged from version 0.1

	SLOW ROASTED: Rocks start slow and slowly get faster.
	

-----------------------------------------------------------
2000/04/26 - 19:00 - 20:00
-----------------------------------------------------------

	Installed Visual C++ 6.0 and SP3.  Hopefully this will
	fix the GPF problem that various people have had.

	Implemented pause.  Cool!

	Made rocks get incrementally faster with each round
	in both difficulty settings.  I have to play test this
	to make sure that it's a good speed.

	Moved some of key input code around so that keys
	active during game over are all together, and moved
	the Difficulty mode toggle out of that area because
	the D key can be used during gameplay to view the
	current setting.

-----------------------------------------------------------
2000/04/26 - 21:00 - 22:00
-----------------------------------------------------------
	
	Tried a couple of different things to make the 
	rock speed adjustable.  Ended up making the
	difficulty level toggleable easy/normal.



***********************************************************




-----------------------------------------------------------
2000/04/24 - 00:00
-----------------------------------------------------------
	Released version 0.1 of the game!

-----------------------------------------------------------
2000/04/23 - 9:30 to 11:15
----------------------------------------------------------
	Played around with force feedback based on suggestion
	from Jeff Molofee.  Created multiple effects that could
	be triggered based on the distance from the player ship
	to the destroyed object.  Didn't work real well, but
	I eliminated the effect felt when firing because it 
	interrupted other effects.

-----------------------------------------------------------
2000/04/19 - (5:00 - 5:30) (20:00-21:00)
-----------------------------------------------------------
	
	Wrote renderenable and renderdisable in old code base
	to try and eliminate flickering problem caused by
	repeated glEnable calls.  Success!

	At night, added routines into main codebase and created
	renderset to make it easier to implement toggleable
	rendering options.

	Text is now rendered correctly no matter what rendering
	features are enabled/disabled except for in wireframe
	mode.

	Fixed GPF bug that occurs when highscore.dat
	file is read-only and game trys to save highscores

	Moved the loop to deactivate lights to end of lighting
	setup so that only unused lights are disabled at the
	end of the loop, rather than having all of the lights
	used last cycle disabled, only to have some of them
	reenabled seconds later.
	
	Rearranged the 'space motion modes' so that 0 is no
	space motion and 2 is the max.  2 is still the default.


-----------------------------------------------------------
2000/04/17 - 10:30 - 11:30
-----------------------------------------------------------

	Removed backfaces from player ship model using 
	Milkshape and reimported with 3D Exploration.

	Removed 'racing stripe' from player ship texture

	Looked at possibility of replacing remaining lives
	ship models with a more-lightweight alternative --
	a character in the font.  Font chars are too small
	to make this a viable option.
	
-----------------------------------------------------------
2000/04/12 - 7:00-7:15
-----------------------------------------------------------

	Looked at textures and decided that the ship and blast
	textures didn't have to be 256x256.  Downsized them
	to 128x128 and played with all the textures to see if
	they could be shrunk to reduce the size of the EXE
		
-----------------------------------------------------------
2000/04/13 - 
-----------------------------------------------------------
	
	Fixed the cursor display problem I caused when
	I removed the debris' texture.  Now I disable
	GL_TEXTURE_2D before drawing the cursor.

-----------------------------------------------------------
2000/04/12 - (17:15-18:00) and (19:45-8:45)
-----------------------------------------------------------
	
	At work, I created a new player model.
	At home, I merged the new model into the main codebase.

	Removed the texture stuff from the debris --
	it used to use texture[0], which now belongs to the
	player ship.  Before texture[0] was the 'IDB_BLANK'
	texture.

	Watched Friends...

	Finished up texture.  Looks really cool.  Still have
	bounding box problem, but the ship model looks cool. :)

	Tweaked xcoll and ycoll on objects to improve collision
	detection.  The small rocks no longer have to be hit
	with pinpoint accuracy.  Sweeet!
	

-----------------------------------------------------------
2000/04/12 - (17:30-17:45) and (20:45-21:00)
-----------------------------------------------------------

	Wrote code to display actor bounding boxes after work.
	Merged code with base code, and added bounding box 
	mode.  X key toggles it.

-----------------------------------------------------------
2000/04/11 - 11:30 - 11:45
-----------------------------------------------------------
	
	Added wireframe mode.  Textures have to be disabled
	for the full effect.  Toggling lighting and blending
	leads to some interesting effects.  I may come back 
	and fix the program so that text is rendered correctly
	later.

-----------------------------------------------------------
2000/04/08	- 20:00 - 20:50
-----------------------------------------------------------

	Fixed problem where cursor would appear in multiple
	places if the current highscore was equal to an existing
	highscore

	Fixed problem with top centered highscore not being 
	updated when user would enter their initials as new
	highest score
	
	Shots now spawn from the tip of the player instead of 
	the middle of the player

	Added speed regulation to space motion mode 1, which
	was added by Jeff.  I noticed that mode 1 ran at a 
	slower pace on my P2/266 than on my C/450.  I always
	thought that mode 1 looked cool, but moved too fast,
	and after seeing it on the 266, I decided that maybe
	it looks slower on his computer, and I checked and 
	noticed that the background's rate wasn't controlled.
	Now it is.

-----------------------------------------------------------
2000/04/04	- 13:00 - 13:15
-----------------------------------------------------------

  Added help screen (H or F1)
  There's some ugly code as a result of this addition, but
  I think it's probably worth it.

-----------------------------------------------------------
2000/04/04	- 12:00pm - 12:15pm
-----------------------------------------------------------

  Added sound toggle key (S)

  Created application icon

  Fixed the highscore entry cursor so that it doesn't
  stay on the last character of the player's initials.
  Also fixed the backspace key to work like a backspace
  key instead of a delete key.


-----------------------------------------------------------
2000/04/03	- 12:15pm - 12:45pm
-----------------------------------------------------------

  Changed g_nSpaceInMotion's default mode to the subtle
  motion -- it makes the stars appear to sparkle while
  minimizing motion sickness ;)

  Changed credits a little.  Added models and texture
  credit to myself, and modified Jeff's a little.

  Textured new player model in TrueSpace, only to import
  it into 3dExploration to find the textures all jumbled.
  I need to change my modelling program to a program
  more suited to low-poly modelling and skinning.  Milkshape
  is probably the way to go -- it's cheap and it will 
  export the objects into a format 3D Exploration can 
  convert.  What a waste of time.

  Also went through the code and removed dead code, 
  added some comments -- real quick cleanup.

-----------------------------------------------------------
2000/04/02 - (6:30pm-7:30pm) (10:30pm - 11:00pm)
-----------------------------------------------------------

  Worked on new player model in TrueSpace.  Not really
  happy with it yet.


-----------------------------------------------------------
2000/04/01 - 4:30pm - 7:00pm
-----------------------------------------------------------

  Created Disasteroids logo for title screen, and got it
  displaying on the 'title screen'.

  Wrote code to display author and copyright information.
  I need to check with Jeff to see how he wants to be 
  credited for his work on the project.

  Created glPrintCentered to make it easy for me to print
  text centered on the screen instead of hand-coding it
  each time I want centered text.  I'm using it for almost 
  all of the on-screen printing now instead of glPrint.

  Almost done!

-----------------------------------------------------------
2000/03/31 - 5:00pm - 5:45pm
-----------------------------------------------------------
	
  Added status message display in the form of g_sMessage 
  and UpdateMessage to allow me to post messages on the 
  screen when the user does things like activate and 
  deactivate the joystick.

  Wrote code to display message when user toggles on/off
  the various rendering and control options

  I also figured out how to center the message using the 
  cursor code as a reference.  That code sure came in 
  handy!  Thanks again, Jeff!


-----------------------------------------------------------
2000/03/29 - 12:00noon - 12:15pm
-----------------------------------------------------------

  Found the last code Jeff sent me, which was a refinement
  of the highscore entry.  He created a cursor in his 
  routine that I lifted the code to help the user enter
  his/her initials should they get a high score.

  
-----------------------------------------------------------
2000/03/28 - 11:30pm - 1:00am
-----------------------------------------------------------

	Finally got around to updating highscore entry code
	so that initials are entered using the keyboard.  I 
	also updated the message shown on the screen during
	highscore entry

	Cleaned up unnecessary key handling and highscore
	entry code

  	Fixed force feedback.  I was not returning the correct
	value from the force feedback capability detection.


-----------------------------------------------------------
2000/03/28 - 10:00pm-11:00pm 
-----------------------------------------------------------

	Experimented in the office with importing all of the 
	textures into the program as source code instead of 
	using resource files, but that didn't work out.  I 
	got compiler errors for my efforts.

	Changed the rendering of the blasts back to being 3D.
	Also changed them from being drawn three times to them
	being drawn twice.  Also tweaked their fade appearance.

	Rewrote keyboard handling -- using lastkeys[] instead
	of individual keystates for function keys.  This was
	also done to make highscore entry using keyboard
	possible	

-----------------------------------------------------------
2000/03/15 - 5:00pm - 6:00pm
-----------------------------------------------------------

	Took about 2 seconds to fix the highscore overwriting
	problem, and about 59:58 to create a Half-Life model
	for Gannon...  ha ha ha

	I need to refine and remap highscore handling next --
	maybe tonight.

-----------------------------------------------------------
2000/03/14 - 11:00pm - 12:00midnight
-----------------------------------------------------------

	I've been slacking.

	Blending toggle checked before enabling blending
	when drawing background

	Got initial entry working. 

-----------------------------------------------------------
2000/03/11 - 5:07pm - 5:45pm
-----------------------------------------------------------

	Added xcolldist and ycolldist values to actorinfo
	structure.  Now each actor has its own x and y
	collision variables.  I'm still not happy about
	collision detection.  It's still flaky.  One second
	a rock will fly right through the player, and the next
	rock may destroy it.  Changing the player model may
	allieviate the problem -- make the ship more square.
	Better yet, make the ship square and compute collision
	distances based on radii of objects (circles rather than
	boxes).  This may be too processor-intensive.

-----------------------------------------------------------
2000/03/11 - 12:00 Noon
-----------------------------------------------------------

	Fixed something I thought of when creating PlayerSpawn
	routine in CreateSaucer -- only look at active rocks :)

-----------------------------------------------------------
2000/03/10 - 4:00pm 
-----------------------------------------------------------
	I've been sick, so I haven't been able to work on 
	the game :(

4:00
	Based on feedback from Jeff M, I removed the rest of 
	the DirectInput initialization MessageBoxes so that 
	3dfx cards don't choke on startup if a joystick isn't 
	connected.

4:10
	In an attempt to fix the problem with the ship's thrust
	not working when compiling a release version, I moved
	the line that resets velocity to zero until the END of 
	the if block, instead of right after the Y calculation.
	I don't know why it works now, but it does.  This is
	just one of those damn C things I hate.  If anyone
	comes across this message in the future reads this and
	knows why -- please let me know!!!

4:15 - 4:30
	Got rid of vanity tag -- I'm not that vain.  Also moved
	score and FPS rendering so that they're always on top.

6:30 - 8:00
	Looked at changes Jeff made to code from a couple of days
	ago.  Added some comments based on his additions.  Also
	added space movement code he implemented.  I set it up 
	as toggleable (M key)

8:00 - 8:30
	Started thinking about how to implement high score entry.
	Made quick LoadHighScores and SaveHighScores routines.  
	The tough part's going to be the intial entry.

11:00 - 12:30
	Changed all BOOL's to bool's,  Stupid -- I know.

	Surfed the net

	(I think) I added g_ to all global variables

	Wrote SpawnPlayer function to prevent player from being
	spawned in the middle of an enemy.  I don't know if this 
	works, but if the player isn't respawned within 10 seconds,
	they are spawned invincible for a couple of seconds :)

12:35
	Went to bed.
  
-----------------------------------------------------------
2000/03/08
-----------------------------------------------------------
	Worked on the logo and website design.  Trying to make
	logo that isn't millions of vertexes.  Don't know if 
	that's going to happen.

-----------------------------------------------------------
2000/03/07 - 09:15 TO 12:00
-----------------------------------------------------------

	Got joystick working and implemented some force
	feedback effects.  I'm using an Interact Hammerhead FX,
	which is a gamepad that shakes.  The effects are pretty
	funny on that, but I don't know how they'd be on a 
	real joystick.

-----------------------------------------------------------
2000/03/07 - 05:00 TO 05:30
-----------------------------------------------------------

	Started implementation of DirectInput from FFDonuts
	DirectX SDK

-----------------------------------------------------------
2000/03/07 - Lunch 
-----------------------------------------------------------

	Replaced cool nebulous space background with 
	dull stars in hopes of fixing horiz and vert lines.
	I think they're more noticable now. :(

-----------------------------------------------------------
2000/03/06 - 09:30pm to
-----------------------------------------------------------

	Stuck vanity tag in lower right corner of screen.

	Used Corel draw to create logo.  I like the design,
	but the 3D model is too big :(  I think any logo
	I create will be too large to be modeled.

	Played.  A lot.

  	I REALLY fixed the rock spawning problem.  Pretty 
	simple.  The rocks were from 0-29.  Well, when 
	FindFreeActor returns, the spawn routine determines
	if a free slot was found if the function returned 
	a value other than zero.  Oops!

	Textured UFO with a very simple texture.  TrueSpace
	stinks for texturing but it's VERY easy for modelling.
	

-----------------------------------------------------------
2000/03/06 - 06:00pm
-----------------------------------------------------------

	Played with dynamic lighting -- colored based on
	actor, and blasts' lights fade as their lives end
	woo hoo!

	Resorted order in which blasts are drawn.  Also 
	disabled depth buffer while they are drawn.  According
	to Brian Hook, the proper order to draw things
	are:
	1. Solid objects
	2. Turn off z-sort
	3. Draw transparents
	I was doing this, except for with the blasts.  The
	black squares around the blast are less apparent, but
	not totally gone.  I may model a flat circle to draw 
	the blasts on to eliminate the problem.  We'll all
	just have to wait and see...

	I may have fixed the problem with the number of rocks,
	but I didn't get a chance to really test it.  
	UPDATE:  I didn't really fix it...

	Also looked at 2 of the things that are bothering
	me most right now -- 
	  * Missing thrust sound
	  * Ship won't move up and down when the 
		program's compiled for Release
	No new revelations.  I'm going to have to solve this!!!

	Coming soon... DirectInput?  

-----------------------------------------------------------
2000/03/06 - Lunch
-----------------------------------------------------------

	Added background and fixed texture binds

-----------------------------------------------------------
2000/03/05	9:00am - 11:00am
-----------------------------------------------------------

	Rolling in changes made by NeHe dealing with fonts and 
	initialization in InitGL.  Thanks again!
  
-----------------------------------------------------------
2000/03/04  7:00pm
-----------------------------------------------------------
	Got dynamic lighting working. Looks really cool!

-----------------------------------------------------------
2000/03/04 12:00 Noon
-----------------------------------------------------------

CHANGES:	
	I started implementing the linked list data for the actors, but I left
	the code in my Zip Drive at work.  Boo hoo.  

	Just to try it out, I implemented stereo sound.  Not too impressed.
	I think there might be something wrong with the sound that's played
	when the player fires.  The stereo effect is very drastic between
	right and left halves of the screen.

	NeHe finally convinced me to shrink the bullets.  
	Bye bye bye phat bullets!

	Made a bunch of changes to the saucer to make it smarter.  It now
	follows a path for a number of seconds, then changes its direction.
	I also setup the saucer to spin faster, and to wobble as it flys
	through the screen.  Looks cool.  Still dumb as mud, though.
  
	CreateSaucer was rewritten to prevent the saucers from being
	spawned in the middle of a rock.  I also turned CreateSaucer
	into a function so that the main routine doesn't reset 
	lTimeForNewSaucer (or whatever it's called).

	Moved model data to modeldata.h to make code a little more
	managable.  

  
-----------------------------------------------------------
2000/03/02 12:00 Midnight
-----------------------------------------------------------

CHANGES:
	Added glEnable(GL_NORMALIZE); to InitGL according to 16 Common Mistakes
	the lighting gets messed up when usingScalef because the normals no longer 
	equal 1.

	Did super preliminary saucer motion jerking across the screen.  This
	will have to be greatly improved.

	Implemented saucer fire.  

	Moved player collision detection to function to make it easier to 
	check for collisions between different objects.  Right now the 
	collisions are a little off.  I think I'm going to have to implement
	bounding boxes for each of the models to make it easy to check 
	collisions.  The saucer is the worst right now because its current
	bounds are a square, while the object is wider than it is tall.

	The change to the collision detection was a major pain in the rump. 

	I'm going to prevent UFO's from spawning after the game's over then
	go to bed.


-----------------------------------------------------------
2000/03/02 5:00pm
-----------------------------------------------------------

UPDATED:
	Finally found cause of rock's texturing problem.  STUPID mistake in the 
	display list generation code.  I was reading in the player's texture
	data instead of the rocks' for the rocks.  Doh!

	Added enemy saucer model.  Doesn't do anything yet.


-----------------------------------------------------------
2000/03/01
-----------------------------------------------------------
	
UPDATED:
	Player shields are now recharged at the beginning of each ship instead
	of each level

	Eliminated 2-second delay when starting a new game.
	  
	Implemented Q&D particle system using Debris as base.

	Particle smoke trails follow the blaster

	5% of the debris spawns flaming particles -- looks cool, but just like 
	the blasts the colors need work.  I'm using GL coloring, and I can't
	get a good mix of yellow/orange/red that looks like fire.

	One thing that kind of disturbs me is that the particles and blasts
	don't mix well.  There are times when they mix, and the blast seems
	to draw black where there should be particles.  Don't really know
	why -- maybe I have to change the drawing order.

	Started writing info to track progress on game.  Should be interesting
	to read when everything's done ;)

  TODO:
	Things are getting very messy.  I think the next step will have to 
	be getting all of the items in linked lists and going through each
	item in a list and rendering instead of the array.  That should
	speed things up considerably, since I'm sure a lot of time
	is being wasted constantly searching for free elements in the
	actors array.

	I've noticed that if I increase the value of MAX_ACTORS by fifty, the
	framerate dips considerably. (Is that thing correct?  300fps???)
	
-----------------------------------------------------------
-----------------------------------------------------------
