CXX       = g++ --std=c++11
MKDIR     = mkdir -p
DEL       = rm -rf
LDD       = ld -r

OUTPUT    = ofled

SRC_DIR   = ./src/
RES_DIR   = ./resources/
OBJ_DIR   = ./obj/
OUT_DIR   = ./bin/Debug/

CXX_OBJ   = -c
CXX_FLAGS = -g -Wall
CXX_OUT   = -o
OF_FLAGS  = -DDEBUG_ENABLED
OF_DEPS   = -lSDL2 -lSDL2_image -lGL -lGLEW -lopenal -logg -lvorbis \
			-lvorbisfile -lphysfs
OF_LIBS   = -loficinad  $(OF_DEPS)

IMGUI_DIR  = $(SRC_DIR)imgui/
IMGUI_LIBS = $(OBJ_DIR)imgui.o $(OBJ_DIR)imgui_demo.o $(OBJ_DIR)imgui_draw.o $(OBJ_DIR)imgui_impl_sdl.o

PROJ_LIBS = $(IMGUI_LIBS) $(OBJ_DIR)EditorScreen.o $(OBJ_DIR)main.o

LIBS      = $(OF_LIBS)
FLAGS     = $(CXX_FLAGS) $(OF_FLAGS) -I$(SRC_DIR) -I$(IMGUI_DIR)

.PHONY: clean delete dirs assets




### Main target

all: debug

release: CXX_FLAGS = -O3
release: OF_FLAGS =
release: OF_LIBS   = -loficina $(OF_DEPS)
release: OUT_DIR   = ./bin/Release/
release: clean game

debug: game

game: dirs $(PROJ_LIBS)
	$(CXX) $(PROJ_LIBS) $(FLAGS) $(LIBS) $(CXX_OUT) $(OUT_DIR)$(OUTPUT)




### Phony Targets

assets:
	cd $(RES_DIR);  \
	make

clean:
	$(DEL) $(OBJ_DIR)*.o

delete:
	$(DEL) $(OUT_DIR)$(OUTPUT)

dirs:
	@$(MKDIR) $(OUT_DIR) $(OBJ_DIR)


### ImGui Targets

$(OBJ_DIR)imgui.o: $(SRC_DIR)imgui.cpp
	$(CXX) $(FLAGS) $(CXX_OBJ) $(SRC_DIR)imgui.cpp $(CXX_OUT) $(OBJ_DIR)imgui.o

$(OBJ_DIR)imgui_demo.o: $(SRC_DIR)imgui_demo.cpp
	$(CXX) $(FLAGS) $(CXX_OBJ) $(SRC_DIR)imgui_demo.cpp $(CXX_OUT) $(OBJ_DIR)imgui_demo.o

$(OBJ_DIR)imgui_draw.o: $(SRC_DIR)imgui_draw.cpp
	$(CXX) $(FLAGS) $(CXX_OBJ) $(SRC_DIR)imgui_draw.cpp $(CXX_OUT) $(OBJ_DIR)imgui_draw.o

$(OBJ_DIR)imgui_impl_sdl.o: $(SRC_DIR)imgui_impl_sdl.cpp
	$(CXX) $(FLAGS) $(CXX_OBJ) $(SRC_DIR)imgui_impl_sdl.cpp $(CXX_OUT) $(OBJ_DIR)imgui_impl_sdl.o


### Subtargets

$(OBJ_DIR)main.o: $(SRC_DIR)main.cpp $(SRC_DIR)EditorScreen.hpp
	$(CXX) $(FLAGS) $(CXX_OBJ) $(SRC_DIR)main.cpp $(CXX_OUT) $(OBJ_DIR)main.o

$(OBJ_DIR)EditorScreen.o: $(SRC_DIR)EditorScreen.cpp $(SRC_DIR)EditorScreen.hpp
	$(CXX) $(FLAGS) $(CXX_OBJ) $(SRC_DIR)EditorScreen.cpp $(CXX_OUT) $(OBJ_DIR)EditorScreen.o
