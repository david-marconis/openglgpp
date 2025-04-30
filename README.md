# OpenGL playground
![image](https://github.com/user-attachments/assets/27635352-8284-47c6-9ae2-06abc0242317)
# Install and run
Requires Conan and Cmake to run. The following code install cmake using apt and Conan using pip in a virtual env.
```bash
sudo apt install cmake
python3 -m venv .venv
. .venv/bin/activate
pip install conan==1.66.0
```

```bash
mkdir -p build
# Installs missing dependencies and system dependencies.
# If installing system dependencies is unwanted then remove 'sudo -E env PATH="$PATH"' and the -c flag
sudo -E env PATH="$PATH" conan install . -if build --build=missing -c tools.system.package_manager:mode=install
cmake -B build
cmake --build build

#Run
build/bin/openglcpp
```
