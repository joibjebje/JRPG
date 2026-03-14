# JRPG :: Programming Assignment 4 :: Spring 2026
---
### Note:
This assignment is being submitted a day late. I didn't really start it until the evening before submission day. I also liked the assignment description and instead of submitting something half finished, I decided to take a point deduction and submit something I can be just a little bit more proud of :)
<br>
I always compiled and ran using WSL, no idea how this compiles/runs on macOS and Windows
---

## Compilation
`$ make`

### Disclaimer:
If for some reason you need to recompile after any changes (if needed for some reason)
`$ make clean && make`

## Running
`$ ./game`

## Features
- Battle System
- Hero Selection Screen
- Hero Details Screen
- Worlds, Levels, Battles
- Impeccable story and lore 
- Boss fight... kind of
- Unique abilities

## How to Play:
The main main menu is quite simple. You have 2 options (Play, Quit). It doesn't get simpler than that.\
Then there is the Play menu which contains the Worlds menu and the Heroes menu.\
\
You will want to start by going into the Heroes menu to assign heroes.\
You do this by using the commands: `a x` or `assign x` where `x` is the left-most number in the hero list.\
You don't HAVE to do this before jumping into a level as you will be presented with a hero selection screen before each level starts.\
You can only have 3 heroes active at once. Meaning you cannot assign a fourth hero. You must unassign the hero with the same command you used to assign the hero.\
\
You can also look at hero details using the commands: `d x` or `details x` where `x` is the left-most number in the hero list. Same as before with the assignment.\
In the Hero Details screen; you can see details about the hero, like HP, Abilities, etc.\
\
There's only ONE world in this build, and any level is accessible. I was on my way to implement unlocking levels by completing the one before it... but ran out of time.\
\
NO PERSISTENCE: You cannot save your progress. There's no real reason anyways at stage.

## What I wanted to do
- implement the overworld mentioned in the assignment description.
- make levels more interactive, with branched paths, and collectibles to find on the way.
- Unlocking heroes: this build has all of the heroes available at your disposal.
- Actual programmable boss fight system (with phases)