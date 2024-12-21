# Pong

The software in this repository is a multithreaded version of the well-known game **Pong**.  
The game is multiplayer so play it with a friend!

## System requirements
The software runs only on Linux-based systems, it has no heavy requirements for CPU / memory / available space.

## Execute
Download the latest release from [Releases](https://github.com/MatteV02/Pong/releases/) then run it from terminal like this `~/<path-to-executable>/Pong`.
Maximize the terminal window to visualize the game correctly.

## Compile it by yourself
1. Install ncurses library on your system
```bash
sudo apt-get install libncurses5-dev libncursesw5-dev
```
2. Run in the project's folder this command
```bash
make release
```
3. A subfolder `build` will be generated, inside of it you can find your exacutable with name `Pong`
