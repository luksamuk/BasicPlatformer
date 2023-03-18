############################################################
# SonicEngine                                              #
# -----------                                              #
# Targets of interest:                                     #
# - all:     Builds the game (Currently on debug profile). #
# - debug:   Builds the game (Debug profile).              #
# - release: Builds the game (Release profile).            #
# - assets:  Compresses game data.                         #
# - clean:   Deletes object files.                         #
# - delete:  Deletes compiled game.                        #
############################################################


CXX       = g++ --std=c++11
MKDIR     = mkdir -p
DEL       = rm -rf
LDD       = ld -r

OUTPUT    = SonicEngine

SRC_DIR   = ./
RES_DIR   = $(SRC_DIR)resources/
OBJ_DIR   = ../obj/
OUT_DIR   = ../bin/Debug/

CXX_OBJ   = -c
CXX_FLAGS = -g -Wall
CXX_OUT   = -o
OF_FLAGS  = -DDEBUG_ENABLED
OF_DEPS   = -lSDL2 -lSDL2_image -lGL -lGLEW -lopenal -logg -lvorbis \
            -lvorbisfile -lphysfs
OF_LIBS   = -loficinad  $(OF_DEPS)

PROJ_OBJS_SCREENS = $(OBJ_DIR)TitleScreen.o $(OBJ_DIR)PresentationScreen.o \
					$(OBJ_DIR)OptionsScreen.o $(OBJ_DIR)LoadingScreen.o \
					$(OBJ_DIR)LevelSelectScreen.o $(OBJ_DIR)LevelScreen.o

PROJ_OBJS_GAME    = $(OBJ_DIR)Effect.o $(OBJ_DIR)Player.o $(OBJ_DIR)Solid.o \
					$(OBJ_DIR)EggWalker.o

PROJ_OBJS_SYS     = $(OBJ_DIR)EffectSpawner.o $(OBJ_DIR)Grid.o \
					$(OBJ_DIR)Parallax.o $(OBJ_DIR)LevelData.o \
					$(OBJ_DIR)DebugUtil.o $(OBJ_DIR)Main.o

PROJ_LIBS = $(OBJ_DIR)Screens.o $(OBJ_DIR)GameObjects.o \
			$(OBJ_DIR)SystemObjects.o

LIBS      = $(OF_LIBS)
FLAGS     = $(CXX_FLAGS) $(OF_FLAGS) -I$(SRC_DIR)

.PHONY: clean delete dirs assets



### Main target

all: debug

release: CXX_FLAGS = -O3
release: OF_FLAGS  =
release: OF_LIBS   = -loficina $(OF_DEPS)
release: OUT_DIR   = ../bin/Release/
release: clean game

debug: game

game: dirs $(PROJ_LIBS)
	$(CXX) $(PROJ_LIBS) $(FLAGS) $(LIBS) $(CXX_OUT) $(OUT_DIR)$(OUTPUT)

### Phony Targets

assets:
	cd $(RES_DIR);  \
	make
	mv data.dat $(OUT_DIR)

clean:
	$(DEL) $(OBJ_DIR)*.o

delete:
	$(DEL) $(OUT_DIR)$(OUTPUT)

dirs:
	@$(MKDIR) $(OUT_DIR) $(OBJ_DIR)



### Subtargets

## Screens

$(OBJ_DIR)TitleScreen.o: $(SRC_DIR)TitleScreen.cpp
	$(CXX) $(CXX_OBJ) $(FLAGS) $(SRC_DIR)TitleScreen.cpp $(CXX_OUT) $(OBJ_DIR)TitleScreen.o

$(OBJ_DIR)PresentationScreen.o: $(SRC_DIR)PresentationScreen.cpp
	$(CXX) $(CXX_OBJ) $(FLAGS) $(SRC_DIR)PresentationScreen.cpp $(CXX_OUT) $(OBJ_DIR)PresentationScreen.o

$(OBJ_DIR)OptionsScreen.o: $(SRC_DIR)OptionsScreen.cpp
	$(CXX) $(CXX_OBJ) $(FLAGS) $(SRC_DIR)OptionsScreen.cpp $(CXX_OUT) $(OBJ_DIR)OptionsScreen.o

$(OBJ_DIR)LoadingScreen.o: $(SRC_DIR)LoadingScreen.cpp
	$(CXX) $(CXX_OBJ) $(FLAGS) $(SRC_DIR)LoadingScreen.cpp $(CXX_OUT) $(OBJ_DIR)LoadingScreen.o

$(OBJ_DIR)LevelSelectScreen.o: $(SRC_DIR)LevelSelectScreen.cpp
	$(CXX) $(CXX_OBJ) $(FLAGS) $(SRC_DIR)LevelSelectScreen.cpp $(CXX_OUT) $(OBJ_DIR)LevelSelectScreen.o

$(OBJ_DIR)LevelScreen.o: $(SRC_DIR)LevelScreen.cpp $(SRC_DIR)objects/EggWalker.hpp
	$(CXX) $(CXX_OBJ) $(FLAGS) $(SRC_DIR)LevelScreen.cpp $(CXX_OUT) $(OBJ_DIR)LevelScreen.o


$(OBJ_DIR)Screens.o: $(PROJ_OBJS_SCREENS)
	$(LDD) $(PROJ_OBJS_SCREENS) $(CXX_OUT) $(OBJ_DIR)Screens.o



## GameObjects

$(OBJ_DIR)Effect.o: $(SRC_DIR)Effect.cpp
	$(CXX) $(CXX_OBJ) $(FLAGS) $(SRC_DIR)Effect.cpp $(CXX_OUT) $(OBJ_DIR)Effect.o

$(OBJ_DIR)Player.o: $(SRC_DIR)Player.cpp
	$(CXX) $(CXX_OBJ) $(FLAGS) $(SRC_DIR)Player.cpp $(CXX_OUT) $(OBJ_DIR)Player.o

$(OBJ_DIR)Solid.o: $(SRC_DIR)Solid.cpp
	$(CXX) $(CXX_OBJ) $(FLAGS) $(SRC_DIR)Solid.cpp $(CXX_OUT) $(OBJ_DIR)Solid.o

$(OBJ_DIR)EggWalker.o: $(SRC_DIR)objects/EggWalker.cpp $(SRC_DIR)objects/EggWalker.hpp
	$(CXX) $(CXX_OBJ) $(FLAGS) $(SRC_DIR)objects/EggWalker.cpp $(CXX_OUT) $(OBJ_DIR)EggWalker.o

$(OBJ_DIR)GameObjects.o: $(PROJ_OBJS_GAME)
	$(LDD) $(PROJ_OBJS_GAME) $(CXX_OUT) $(OBJ_DIR)GameObjects.o


## SystemObjects

$(OBJ_DIR)EffectSpawner.o: $(SRC_DIR)EffectSpawner.cpp
	$(CXX) $(CXX_OBJ) $(FLAGS) $(SRC_DIR)EffectSpawner.cpp $(CXX_OUT) $(OBJ_DIR)EffectSpawner.o

$(OBJ_DIR)Grid.o: $(SRC_DIR)Grid.cpp
	$(CXX) $(CXX_OBJ) $(FLAGS) $(SRC_DIR)Grid.cpp $(CXX_OUT) $(OBJ_DIR)Grid.o

$(OBJ_DIR)Parallax.o: $(SRC_DIR)Parallax.cpp
	$(CXX) $(CXX_OBJ) $(FLAGS) $(SRC_DIR)Parallax.cpp $(CXX_OUT) $(OBJ_DIR)Parallax.o

$(OBJ_DIR)Main.o: $(SRC_DIR)main.cpp
	$(CXX) $(CXX_OBJ) $(FLAGS) $(SRC_DIR)main.cpp $(CXX_OUT) $(OBJ_DIR)Main.o

$(OBJ_DIR)LevelData.o: $(SRC_DIR)LevelData.cpp $(SRC_DIR)LevelData.hpp
	$(CXX) $(CXX_OBJ) $(FLAGS) $(SRC_DIR)LevelData.cpp $(CXX_OUT) $(OBJ_DIR)LevelData.o

$(OBJ_DIR)DebugUtil.o: $(SRC_DIR)OFSDebugUtil.cpp $(SRC_DIR)OFSDebugUtil.hpp
	$(CXX) $(CXX_OBJ) $(FLAGS) $(SRC_DIR)OFSDebugUtil.cpp $(CXX_OUT) $(OBJ_DIR)DebugUtil.o

$(OBJ_DIR)SystemObjects.o: $(PROJ_OBJS_SYS)
	$(LDD) $(PROJ_OBJS_SYS) $(CXX_OUT) $(OBJ_DIR)SystemObjects.o
