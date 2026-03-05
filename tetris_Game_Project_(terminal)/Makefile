# This is the "Play Button" command
run: build
	./tetris

# This is the "running" command
build:
	g++ src/tetris.cpp src/state.cpp src/physics.cpp src/renderer.cpp src/engine.cpp src/input.cpp -Iinclude -o tetris

# This is the "Cleaning" command
clean:
	rm tetris