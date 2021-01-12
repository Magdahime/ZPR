# ZPR - Readme

## Authors

 - Magdalena Majkowska
 - Bartłomiej Janowski

## List of used libraries

 - OpenMP
 - Boost (requires min. Boost 1.75.0)
	 - Boost.JSON
     - Boost.Test
     - Boost.Interprocess
     - Boost.Lockfree
     - Boost.DLL
 - Boost
 - [Webview](https://github.com/webview/webview/)
 - SFML
 - Python.h (Embedded Python)
 
## Installing dependencies

As the project uses Boost.JSON, which is included with Boost only since version 1.75.0, it is needed (as of January 2021) to install it from source or from untrusted repositories.

We have provided a script that installs all needed dependecies, as well as downloads and builds required Boost libraries.

Run on linux:
```
chmod +x scripts/config.sh
./scripts/config.sh 
```

Compilation on Windows is a little more sophisticated, as Webview and SFML have to be built and have to have their .dll's placed in ./bin, but it can be done with: 
```
.\scripts\config.ps1
```
This script can only be ran in Powershell.

# Compilation
Use
```
scons
```
in the main project directory (the same in which this README is located) to compile the program.

To compile the tests, run:
```
scons test=1
```

# Running the program
The resulting executable will be located at *bin/CreatureSIM*. Before running please ensure that ***OMP_WAIT_POLICY*** enviromental variable is set to ***"PASSIVE"***. On Linux this can be done by just running:
```
OMP_WAIT_POLICY="PASSIVE"
./bin/CreatureSIM
```