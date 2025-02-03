CC = clang

src = $(shell find src -type f -name "*.c")
lib = glfw3 dl GL pthread X11 m
lib_include = lib/glfw/lib lib/bgfx/.build/linux64_gcc/bin
include = lib/cglm/include lib/glfw/include lib/bgfx/include lib/bx/include lib/bimg/include
additional_flags =

all: shaders_create_directories shaders_compile textures_create_directories textures_compile
	@mkdir -p build
	@$(CC) $(addprefix -I,$(include)) $(addprefix -L,$(lib_include)) $(src) lib/bgfx/.build/linux64_gcc/bin/libbgfx-shared-libRelease.so $(addprefix -l,$(lib)) $(additional_flags) -o build/testing

debug:
	@$(CC) $(addprefix -I,$(include)) $(addprefix -L,$(lib_include)) $(src) $(addprefix -l,$(lib)) $(additional_flags) -o build/testing -pg

shaders_create_directories:
	@find res/shaders -mindepth 1 -type d -exec sh -c 'mkdir -p build/shaders/$$(echo {} | sed "s|^res/shaders/||")' \;

shaders_compile:
	@find res/shaders -type f -name "vs.sc" -exec sh -c 'output=$$(echo {} | sed "s|^res/shaders|build/shaders|" | sed "s/.sc/.bin/"); varying=$$(echo $$(dirname {})); varying+="/varying.def.sc"; lib/bgfx/.build/linux64_gcc/bin/shadercRelease -f {} -o $$output --platform linux --type vertex --varyingdef $$varying --profile 440 -i lib/bgfx/src;' \;
	@find res/shaders -type f -name "fs.sc" -exec sh -c 'output=$$(echo {} | sed "s|^res/shaders|build/shaders|" | sed "s/.sc/.bin/"); varying=$$(echo $$(dirname {})); varying+="/varying.def.sc"; lib/bgfx/.build/linux64_gcc/bin/shadercRelease -f {} -o $$output --platform linux --type fragment --varyingdef $$varying --profile 440 -i lib/bgfx/src;' \;

textures_create_directories:
	@find res/textures -mindepth 1 -type d -exec sh -c 'mkdir -p build/textures/$$(echo {} | sed "s|^res/textures/||")' \;

textures_compile:
	@find res/textures -mindepth 1 -type f -exec sh -c 'output=$$(echo {} | sed "s|^res/textures|build/textures|" | sed "s/\.[^.]*$$/.dds/"); lib/bgfx/.build/linux64_gcc/bin/texturecRelease -f {} -o $$output' \;
