# tumem - turing machine emulator
**tumem** is a simple virtual turing machine. It outputs step-by-step tape chages to the standard output.

## Build
```
sh build.sh
```
This repository includes a script that builds **tumem** with g++.

## Run
```
./tumem <rules.txt> <tape.txt>
```
Run **tumem** by providing two text files:
1. `rules.txt` which describes the rules of the machine.
2. `tape.txt` which holds the initial content of the tape.

Examples for these files can be found in the [examples](/examples) directory.
