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
1. without linux : 
    if pkg-congif isn't installed, run these commands
  ```
    pacman -Syu
    pacman -S mingw-w64-x86_64-toolchain
    pacman -S mingw-w64-x86_64-gtk3
    pacman -S mingw-w64-x86_64-pkg-config
  ```
  Then 
  ```
   pacman -S mingw-w64-x86_64-gtk3
   ```
1. Using Linux : in powershell terminal
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
1. restart pc

2. use ubuntu terminal
```bash
sudo apt update
sudo apt install build-essential libgtk-3-dev pkg-config
```

1. Using vscode ubuntu terminal
```bash
sudo apt update
sudo apt install libgtk-3-dev
```

1. make sure you're using the right bash with vscode :
Bash should be MINGW64 ! And the path you use should be C:\msys64\mingw64\bin
MINGW64 bash should be in this path C:\msys64\msys2_shell.cmd 

If VScode doesn't let you do whatever you need to, do 
```
Ctrl+Shift+P → Preferences: Open Settings (JSON)
```
then inside JSON put : 
```
"terminal.integrated.profiles.windows": {
  "MSYS2 MinGW64": {
    "path": "C:\\msys64\\msys2_shell.cmd",
    "args": ["-mingw64", "-defterm", "-no-start", "-here"],
    "icon": "terminal-bash"
  }
},
"terminal.integrated.defaultProfile.windows": "MSYS2 MinGW64",
"terminal.integrated.env.windows": {
  "MSYSTEM": "MINGW64"
}
```
# Install PostgreSQL with msys64
1. 
   1. 0 Open MINGW64 bash (/!\ important)
Then run 
```
pacman -Syu
pacman -Su
pacman -S mingw-w64-x86_64-postgresql
```
   1. 1 If installation fails, maybe the mirror is not up to date:
run this command (if nano is not installed, must be installed first or find file and edit with any text editor)
```
nano /etc/pacman.d/mirrorlist.mingw64
```
Write inside the file named mirrorlist.mingw64
```
Server = https://mirror.clarkson.edu/msys2/mingw/x86_64/
```
Then to save do :
 ctrl + 0
 enter
 and then close using ctrl + X

Clear the package cache before trying again
```
pacman -Scc
```
After changin mirror, redo step 1.0
```
pacman -Sy
pacman -S mingw-w64-x86_64-postgresql
```

2. Check if installation worked properly
```
which psql
psql --version
```
postgresql should be in this path = /mingw64/bin/psql

3. Initialize database using 
```
initdb -D ~/pgsql/data
```
then start server
```
pg_ctl -D ~/pgsql/data -l logfile start
```
For stopping server : 
```
pg_ctl -D ~/pgsql/data stop
```

# Check if everything is installed :
GTK :
`pkg-config --modversion gtk+-3.0`

GCC : `gcc -v`

Make : `make --version`

postgresql : `psql --version`


# installation cjson

```bash
sudo apt-get install libcjson-dev
```