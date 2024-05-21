all:
	
	g++ Principal.cpp --std=c++17 -I/usr/local/include/opencv4/ -L/Users/kevinjapa/opencv/build/lib/ -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_imgcodecs -lopencv_video -lopencv_videoio -o vision.bin -Wl,-rpath,/Users/kevinjapa/opencv/build/lib/

run:
	./vision.bin