# List of libraries to install
**SFML** for graphics
```
sudo apt-get install libsfml-dev
```
**BOOST** for unit testing and synchronization (mutexes)
```
sudo apt install libboost-all-dev  
```
**WEBKIT_GTK** needed for webview to function properly
```
sudo apt install libwebkit2gtk-4.0-dev
```
**OpenMP** task parallelization
```
sudo apt install libomp-dev
```
**WEBVIEW** for displaying statistics and graphical user interface

For Linux
```
chmod +x config.sh
./scripts/config.sh 
```
For Windows 
```
.\scripts\config.ps1
```
It will only clone the repository to its place in lib directory.

# Compilation
Use
```
scons
```
in the main project directory (the same in which this README is located) to compile the program.

# Running the program
The resulting executable will be located at *bin/CreatureSIM*. As you will be able to see it is very much work in progress.