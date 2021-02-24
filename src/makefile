name = "Tris"
files = main.c gameplay.c gui.c shapes.c client.c
src_dir = 
build_dir = ../
flags = -lraylib
flags_w64 = -L lib/ $(flags) -lopengl32 -lwinmm -lgdi32 -static -lwinpthread -Wl,--subsystem,windows -lwsock32

all: linux windows server

linux:
	gcc -O3 -W -Wall -Wextra $(flags) $(src_dir)$(files) -o $(build_dir)$(name)
	@echo Compilation Successfull!

windows:
	x86_64-w64-mingw32-gcc -O3 $(src_dir)$(files) -o $(build_dir)$(name).exe $(flags_w64)

server:
	gcc -O3 -W -Wall -Wextra $(src_dir)server.c $(src_dir)server_gameplay.c $(flags) -o $(build_dir)server
