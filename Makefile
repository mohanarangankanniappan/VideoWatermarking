all: 
	g++ -std=c++11 Source/WMInterface.cpp Source/logger.cpp Source/WMOpenCV.cpp `pkg-config --cflags --libs opencv4` -g -I Include/ -o liblogowatermark.so -fPIC -shared
	g++ -fPIC -std=c++11 Source/main.cpp `pkg-config --cflags --libs opencv4` -g -I Include/ -L. -llogowatermark -o watermarker

clean:
	rm -f watermarker liblogowatermarker.so
