# Install GCC with make

1. download and install msys2 :

https://www.msys2.org/

2. use the blue msys2 terminal
```bash
pacman -Syu
pacman -Su
pacman -S gcc
pacman -S make
```

3. Add msys64\usr\bin to path
ex path : C:\msys64\usr\bin

# Install and use GTK for C
1. in powershell terminal
Go back to desktop using
```bash
$home\desktop
```
then

```bash
wsl --install
```

it is possible to start ubuntu bash using
```bash
wsl.exe -d Ubuntu
```

2. restart pc

3. use ubuntu terminal
```bash
sudo apt update
sudo apt install build-essential libgtk-3-dev pkg-config
```

4. Using vscode ubuntu terminal
```bash
sudo apt update
sudo apt install libgtk-3-dev
```
