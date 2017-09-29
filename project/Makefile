# Project: Yellow StarDestroyer

CPP  = g++
CC   = gcc
BIN  = StarDestroyer

OBJ  = main.o StarDestroyer.o Star.o Scenario.o mesh.o SecretTech.o Controlls.o Utils.o SecretTechManager.o XWing.o ResistanceFleet.o Space.o Textures.o SpaceScanner.o TextRenderer.o Menu.o Game.o
LINKOBJ  = main.o StarDestroyer.o Star.o Scenario.o mesh.o SecretTech.o Controlls.o Utils.o SecretTechManager.o XWing.o ResistanceFleet.o Space.o Textures.o SpaceScanner.o TextRenderer.o Menu.o Game.o
LIBS = -L/usr/X11R6 -lGL -lGLU -lglut -lSDL2_image -lSDL2 -lSDL2_ttf -lm
INCS = -I. -I/usr/X11R6/include
CXXINCS= -std=c++11
CXXFLAGS = $(CXXINCS)
CFLAGS = $(INCS)
RM = rm -f

all: $(BIN)


clean:
	${RM} $(OBJ) $(BIN)

$(BIN): $(OBJ)
	$(CPP) $(LINKOBJ) -o $(BIN) $(LIBS)

main.o: main.cpp
	$(CPP) -c main.cpp -o main.o $(CXXFLAGS)

StarDestroyer.o: StarDestroyer.cpp
	$(CPP) -c StarDestroyer.cpp -o StarDestroyer.o $(CXXFLAGS)

Star.o: Star.cpp
	$(CPP) -c Star.cpp -o Star.o $(CXXFLAGS)

Scenario.o: Scenario.cpp
	$(CPP) -c Scenario.cpp -o Scenario.o $(CXXFLAGS)

mesh.o: mesh.cpp
	$(CPP) -c mesh.cpp -o mesh.o $(CXXFLAGS)

SecretTech.o: SecretTech.cpp
	$(CPP) -c SecretTech.cpp -o SecretTech.o $(CXXFLAGS)

Controlls.o: Controlls.cpp
	$(CPP) -c Controlls.cpp -o Controlls.o $(CXXFLAGS)

Utils.o: Utils.cpp
	$(CPP) -c Utils.cpp -o Utils.o $(CXXFLAGS)

SecretTechManager.o: SecretTechManager.cpp
	$(CPP) -c SecretTechManager.cpp -o SecretTechManager.o $(CXXFLAGS)

XWing.o: XWing.cpp
	$(CPP) -c XWing.cpp -o XWing.o $(CXXFLAGS)

ResistanceFleet.o: ResistanceFleet.cpp
	$(CPP) -c ResistanceFleet.cpp -o ResistanceFleet.o $(CXXFLAGS)

Space.o: Space.cpp
	$(CPP) -c Space.cpp -o Space.o $(CXXFLAGS)

Textures.o: Textures.cpp
	$(CPP) -c Textures.cpp -o Textures.o $(CXXFLAGS)

SpaceScanner.o: SpaceScanner.cpp
	$(CPP) -c SpaceScanner.cpp -o SpaceScanner.o $(CXXFLAGS)

TextRenderer.o: TextRenderer.cpp
	$(CPP) -c TextRenderer.cpp -o TextRenderer.o $(CXXFLAGS)

Menu.o: Menu.cpp
	$(CPP) -c Menu.cpp -o Menu.o $(CXXFLAGS)

Game.o: Game.cpp
	$(CPP) -c Game.cpp -o Game.o $(CXXFLAGS)
