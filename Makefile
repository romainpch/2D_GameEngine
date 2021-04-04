CPPFLAGS= $(shell sdl2-config --cflags --libs)

moteur : main.cpp ./src/*
	g++ main.cpp $(CPPFLAGS) -lSDL2_Image -o Moteur2D

clean :
	rm Moteur2D
