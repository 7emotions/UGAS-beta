# UGAS-beta
<!-- badges: start -->
<!-- badges: end -->

The goal of UGAS-beta is to help to automatically aim.

## Installation

### HikCamera SDK

1. Visit [MVS](https://www.hikrobotics.com/cn/machinevision/service/download?module=0) to download the HikVision MVS.
2. Then unzip it and cd into the folder.
3. Run the command blow to install the MVS(x86_64 for PC).
	`sudo dpkg -i ***.deb`
4. Run the command blow to run MVS.
	`sudo /opt/MVS/bin/MVS.sh`

PS: Adding its lib to PATH is NOT recommended, for it may cause some conflicts with Qt.

### OpenCV

1. Visit [OpenCV](https://opencv.org/releases/) to download the latest version of OpenCV.
2. Then unzip it and cd into the folder.
3. Run the command blow to install OpenCV.
```
	mkdir build
	cd build
	cmake -D CMAKE_BUILD_TYPE=Release -D CMAKE_INSTALL_PREFIX=/usr/local .. 
	make
	sudo make install
	sudo /bin/bash -c 'echo "/usr/local/lib" > /etc/ld.so.conf.d/opencv.conf'
	sudo ldconfig
```
4. Run the command blow to check the version of OpenCV.
```
	pkg-config --modversion opencv
```

## Usage

### UGAS-beta

1. Clone this repo.
2. Run the command blow to complie.

```
	cd UGAS-beta
	mkdir build
	cd build
	cmake ..
	make
```
3. Run `./ugas-beta` to run it.


