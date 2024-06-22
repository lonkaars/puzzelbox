all: doxygen

.PHONY: FORCE

doxygen: Doxyfile FORCE
	doxygen

FMT += $(shell git ls-files '*.h' '*.c' '*.cpp')
format: FORCE
	clang-format -i $(FMT)
	clang-tidy --fix-errors $(FMT)

