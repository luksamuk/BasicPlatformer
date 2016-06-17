CXX       = g++ --std=c++11
MKDIR     = mkdir -p
DEL       = rm -rf
LDD       = ld -r

OUTPUT    = SonicEngine

SRC_DIR   = ./
RES_DIR   = $(SRC_DIR)resources/
OBJ_DIR   = ../obj/
OUT_DIR   = ../bin/

CXX_OBJ   = -c
CXX_FLAGS = -g
CXX_OUT   = -o
OF_FLAGS  = -DDEBUG_ENABLED
OF_DEPS   = -lSDL2 -lSDL2_image -lGL -lGLEW -lopenal -logg -lvorbis \
            -lvorbisfile -lphysfs
OF_LIBS   = -loficinad  $(OF_DEPS)

PROJ_OBJS_SCREENS = $(OBJ_DIR)TitleScreen.o $(OBJ_DIR)PresentationScreen.o \
					$(OBJ_DIR)OptionsScreen.o $(OBJ_DIR)LoadingScreen.o \
					$(OBJ_DIR)LevelSelectScreen.o $(OBJ_DIR)LevelScreen.o

PROJ_OBJS_GAME    = $(OBJ_DIR)Effect.o $(OBJ_DIR)Player.o $(OBJ_DIR)Solid.o

PROJ_OBJS_SYS     = $(OBJ_DIR)EffectSpawner.o $(OBJ_DIR)Grid.o \
					$(OBJ_DIR)Parallax.o $(OBJ_DIR)Main.o


PROJ_LIBS = $(OBJ_DIR)Screens.o $(OBJ_DIR)GameObjects.o \
			$(OBJ_DIR)SystemObjects.o

LIBS      = $(OF_LIBS)
FLAGS     = $(CXX_FLAGS) $(OF_FLAGS) -I$(SRC_DIR)

.PHONY: clean delete dirs assets

### Main target

all: dirs $(PROJ_LIBS)
	$(CXX) $(PROJ_LIBS) $(FLAGS) $(LIBS) $(CXX_OUT) $(OUT_DIR)$(OUTPUT)

### Phony Targets
clean:
	$(DEL) $(OBJ_DIR)*.o

delete:
	$(DEL) $(OUT_DIR)$(OUTPUT)

dirs:
	@$(MKDIR) $(OUT_DIR) $(OBJ_DIR)

assets:
	cd $(RES_DIR);  \
	make


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

$(OBJ_DIR)LevelScreen.o: $(SRC_DIR)LevelScreen.cpp
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

$(OBJ_DIR)SystemObjects.o: $(PROJ_OBJS_SYS)
	$(LDD) $(PROJ_OBJS_SYS) $(CXX_OUT) $(OBJ_DIR)SystemObjects.o