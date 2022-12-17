# SimplePictureTool
A simple GUI that using the ImageMagick and imgp software, for batch processing of .PNG or .JPG image files in a simple way.
<br>
#### Features of the GUI: <br />
 * Allows to compress all image files in a directory.
 * Allows to reduce the image-size.
 * Allows to remove META-Data from image files.

#### Requierment: <br />
* Qt 5.14 (or higher) - Download Qt here: https://www.qt.io/download
* ImageMagick - https://imagemagick.org/index.php
* imgp - https://github.com/jarun/imgp

#### Get the latest build of SimplePictureTool for Linux:<br />
<a href="https://github.com/pagaco-swita/SimplePictureTool/releases">https://github.com/pagaco-swita/SimplePictureTool/releases</a>

#### How to build the Application:

```
$ sudo apt-get install libkf5filemetadata-dev locales qt5-qmake-bin libkf5coreaddons-dev libkf5config-dev qtbase5-dev tzdata qtchooser qt5-qmake libqt5opengl5-dev baloo-kf5-dev qtbase5-dev-tools imagemagick libqt5gui5 libqt5widgets5 libqt5core5a

$ cd ./src
$ qmake (or qmake -qt5)
$ make -f Makefile
``` 

#### Screenshot: <br />
![Screenshot](https://github.com/pagaco-swita/SimplePictureTool/blob/main/screenshot.png)
