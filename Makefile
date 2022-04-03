CC = g++
CXXFLAGS = -lX11 -lGL -lpthread -lpng -lstdc++fs -std=c++17 -Wfatal-errors -w
app: Boids.cpp  ; $(CC) $(CXXFLAGS)  -o app Boids.cpp olcPixelGameEngine.cpp\
		Boid.cpp Object.cpp Entity.cpp Button.cpp Slider.cpp Counter.cpp

clean:  ; -rm $(objects) app error.txt log

debug:  ; $(CC) $(CXXFLAGS) -o app -g Boids.cpp olcPixelGameEngine.cpp\
		Boid.cpp Object.cpp Entity.cpp Button.cpp Slider.cpp Counter.cpp

log:  ; $(CC) $(CXXFLAGS) 2> error.txt -o app Boids.cpp olcPixelGameEngine.cpp\
		Boid.cpp Object.cpp Entity.cpp Button.cpp Slider.cpp Counter.cpp
