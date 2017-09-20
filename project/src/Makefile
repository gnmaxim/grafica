# Project: Yellow StarDestroyer

CPP  = g++
CC   = gcc
BIN  = StarDestroyer

OBJ  = main.o StarDestroyer.o Star.o SpaceObject.o mesh.o SecretTech.o Input.o Utils.o SecretTechManager.o Enemy.o EnemyManager.o Space.o Textures.o Minimap.o TextRenderer.o MenuManager.o Game.o Tutorial.o
LINKOBJ  = main.o StarDestroyer.o Star.o SpaceObject.o mesh.o SecretTech.o Input.o Utils.o SecretTechManager.o Enemy.o EnemyManager.o Space.o Textures.o Minimap.o TextRenderer.o MenuManager.o Game.o Tutorial.o
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

SpaceObject.o: SpaceObject.cpp
	$(CPP) -c SpaceObject.cpp -o SpaceObject.o $(CXXFLAGS)

mesh.o: mesh.cpp
	$(CPP) -c mesh.cpp -o mesh.o $(CXXFLAGS)

SecretTech.o: SecretTech.cpp
	$(CPP) -c SecretTech.cpp -o SecretTech.o $(CXXFLAGS)

Input.o: Input.cpp
	$(CPP) -c Input.cpp -o Input.o $(CXXFLAGS)

Utils.o: Utils.cpp
	$(CPP) -c Utils.cpp -o Utils.o $(CXXFLAGS)

SecretTechManager.o: SecretTechManager.cpp
	$(CPP) -c SecretTechManager.cpp -o SecretTechManager.o $(CXXFLAGS)

Enemy.o: Enemy.cpp
	$(CPP) -c Enemy.cpp -o Enemy.o $(CXXFLAGS)

EnemyManager.o: EnemyManager.cpp
	$(CPP) -c EnemyManager.cpp -o EnemyManager.o $(CXXFLAGS)

Space.o: Space.cpp
	$(CPP) -c Space.cpp -o Space.o $(CXXFLAGS)

Textures.o: Textures.cpp
	$(CPP) -c Textures.cpp -o Textures.o $(CXXFLAGS)

Minimap.o: Minimap.cpp
	$(CPP) -c Minimap.cpp -o Minimap.o $(CXXFLAGS)

TextRenderer.o: TextRenderer.cpp
	$(CPP) -c TextRenderer.cpp -o TextRenderer.o $(CXXFLAGS)

MenuManager.o: MenuManager.cpp
	$(CPP) -c MenuManager.cpp -o MenuManager.o $(CXXFLAGS)

Game.o: Game.cpp
	$(CPP) -c Game.cpp -o Game.o $(CXXFLAGS)

Tutorial.o: Tutorial.cpp
	$(CPP) -c Tutorial.cpp -o Tutorial.o $(CXXFLAGS)
