
# game-framework

Application framework for developing 2d or console c++ applications


## Dependencies:
    - All projects require openssl
    - Tests and demos as well as any project that depends on gamelib must have sfml 2.x.x. The 3.x.x API has changed and this project has not been updated for it
    - Tests requires boost to be installed. Only the unittest module.

## Building, Running, Tests

This project is cross platform and will build on any platform premake5 supports, or any platform where you can generate a build file yourself.
It is highly reccomended to build this for linux, although build times through wsl2 are very slow due to disk emulation limitations, so i have added native windows steps.

1. Install premake5
2. premake5 <your platform target> (gmake, xcode, vs2022)
4. On linux, this is it. If you have used your package manager to install boost, sfml and openssl development packages, you can make config=release or config=debug
5. On windows, you need to do vcpkg install. You need to make sure vcpkg doesnt install SFML 3.x.x which can be a pain. Reccomended building vcpkg yourself and checking out an older release where 2.x.x is default, or putting a pull request into this repo to fix it to run on 3.x.x
6. Additionally, on windows, premake5 does not generate the pch settings properly, you need to open the project in vs2022+ and change the precompiled header from "Use" to "Create" for bqpch.cpp
3. Trigger the build with your preffered build tools using the generated scripts. Keep in mind the demos have res folders that need to be moved to their build directories.
