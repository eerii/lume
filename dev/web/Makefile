CC = em++
TARGET = lume

SRC_DIRS = ../lume/fresa ../lume/game ../lume/deps/yaml-cpp/src
SINGLE_SRC_DIRS = ../lume/deps/glm ../lume/deps/yaml-cpp/include ../lume/deps/extra ../lume
EXCLUDE_DIRS = ../lume/game/gui/widgets

PRE_INCLUDE_DIRS = $(foreach DIR, $(SRC_DIRS), ${shell find $(DIR) -type d -print}) ${SINGLE_SRC_DIRS}
INCLUDE_DIRS = $(foreach E, $(EXCLUDE_DIRS), $(subst ${E},,$(PRE_INCLUDE_DIRS)))
INCLUDE_PARAMS = $(foreach DIR, $(INCLUDE_DIRS), -I${DIR})

SRCS = $(wildcard $(foreach DIR, $(INCLUDE_DIRS), ${DIR}/*.cpp))
OBJS = $(patsubst %.cpp,%.o,$(SRCS))

BUILD_DIR = build
RES_DIR = ../lume/res

all: post-build

pre-build:
	@echo "lume ✨"
	@echo "copying resources"
	${shell cp -r $(RES_DIR) ./}

main-build: pre-build
	@$(MAKE) --no-print-directory $(TARGET)

post-build: main-build
	@echo "done! :D"

$(TARGET): $(OBJS)
	@echo "linking everything"
	@${CC} --preload-file res -s USE_SDL=2 -s MAX_WEBGL_VERSION=2 -s LLD_REPORT_UNDEFINED -s NO_DISABLE_EXCEPTION_CATCHING $(foreach O, ${OBJS}, files/$(notdir ${O})) -o ${BUILD_DIR}/lume.html

%.o: %.cpp
	@echo "compiling $<"
	@$(CC) $(INCLUDE_PARAMS) -std=c++20 -c $< -o files/$(notdir $@)

.PHONY: all