# Install and run
```bash
#Build
mkdir build
cd build
conan install ..
cmake .. -D MAIN_CLASS=projection.cpp
cmake --build .

#Run
cd ..
build/bin/openglcpp
```
