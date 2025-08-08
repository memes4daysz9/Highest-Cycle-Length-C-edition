# Highest "Cycle Length" Algorithm (C++ Edition)
This algorithm was originally written by a friend, which then another friend ([Repo Here](https://github.com/lrockreal/highest-cycle-length)) optimized it in rust. I used this repo as a base for my script inside C++, you can see sections of rust inside the comments.


Side note this is also the first time ive actually made a comprehensive readme

# Installation Guide and first run
1. Clone The Repository:
```sh
git clone https://github.com/memes4daysz9/Highest-Cycle-Length-C-edition.git
```
2. Test if your c++ enviroment works before messing with numbers:
```sh
cd ./Highest-Cycle-Length-C-edition/ #Go to folder
make -j #compile (multithreaded)
./main #run script
```

## Optional: Configure Settings

1. The script by default uses all your availiable threads * 2. meaning if you have a 6 core, 12 thread CPU (like mine), itll use 24 threads because 12 * 2 = 24. this is just a quick way to have the script run slightly better with minimal problems
```c++
// src/main.cpp

const uint64_t CPUthreadCount = static_cast<uint64_t>(std::thread::hardware_concurrency() * 2); // comment out to run custom thread numbers

//const uint64_t CPUthreadCount = 6; // Uncomment to run custom number of threads

const uint64_t Target = 2500;//benchmark value
```

2. Rebuild code
```sh
make # or alternativly make -j
```
3. Test new numbers
```sh
./main
```


# Debugging Compilation (not much)


As a "catch-all" you can just use
```sh
sudo apt update
sudo apt install build-essential
```
on debian systems, not sure about arch, however with this itll try to install amount other things, make and g++(c++ for x86 and ARM)

## if you get a problem with the code not compiling on the first attempt. try:
```sh
sudo apt install g++
```
on debian systems or 
```sh
sudo pacman -S gcc
```
for arch. other distros + windows can figure it out yourself


## if theres a problem with make, try
```sh
sudo apt install make
```
on debian systems. or use
```sh 
sudo pacman -S make
```
for arch users, for windows, theres a website to go to.
