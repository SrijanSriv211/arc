# <img title="arc" src="res/arc.png" width="32" height="32"> arc
## :wave: Introduction
### :question: What is arc?
> 1. **arc** is a command prompt like terminal.<br>
> 2. Yeah it's just a bit more customizable command prompt.<br>
> 3. I'm building arc as an **execution engine** for my **personal AI assistant** [WINTER](https://github.com/SrijanSriv211/WINTER).

### :question: Why am I working on it?
> I started working on AOs as a hobby project back in 2020. Originally the project was written in Python but then I shifted the project to C#.
> I worked on AOs as to better learn C# programming language or just do some time pass.
> 
> Now I'm rewriting AOs in C++ and I've actually renamed it to arc. I don't know why but it sounds cool.<br>
> The old repository is **https://github.com/SrijanSriv211/AOs**

***

## :toolbox: Getting started
arc is officially untested on other development environments whilst I focus on a Windows build, but you can modify it to work cross-platform.

### :bangbang: Prerequisites
You need to install the following on your machine.
- [Mingw-w64](https://www.mingw-w64.org/downloads/#mingw-builds) >= C++ 20
- [Python](https://www.python.org/downloads) >= 3.12

### :pencil: Getting arc
<ins>**1. Downloading the repository:**</ins>

Start by cloning the repository with `git clone --recursive https://github.com/SrijanSriv211/arc`.

If the repository was cloned non-recursively previously, use `git submodule update --init --recursive` to clone the necessary submodules.

<ins>**2. Running and Compiling arc:**</ins>

1. Run the program:
```console
scripts\build run
```

2. Compile the program:
```console
scripts\build
```
