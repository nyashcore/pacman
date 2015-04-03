# pacman
Project C++

Taking part in development:

$ cd ~
$ git clone git@github.com:cocos2d/cocos2d-x.git
$ cd cocos2d-x
$ python download-deps.py
$ git submodule update --init
$ ./setup.py
$ source ~/.bashrc
$ git clone git@github.com:nyashcore/pacman.git
$ cocos new MyGame -p com.rksixers.mygame -l cpp
$ cp -r MyGame/cocos2d pacman/
$ cd pacman/
$ cocos run -p linux
