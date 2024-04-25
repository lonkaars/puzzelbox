# this file is for lazy people (loek)

.PHONY: FORCE

all: FORCE build/main.uf2

build/build.ninja: CMakeLists.txt
	mkdir -p build
	cmake -B build -G Ninja --fresh --log-level WARNING

build/main.uf2: build/build.ninja FORCE
	ninja -C build
# ninja automatically builds in parallel, so is preferred

flash: build/main.uf2 FORCE
	picotool load -fx $<
# -f forces a reboot of the pico before flashing
# -x resets the pico after flashing

clean: FORCE
	$(RM) -r build

