CC = gcc
CFLAGS = -Wall -O

C_FILES = main.c

BUILD_DIR = build
TEMPLATES_DIR = templates

.PHONY: build
build: clean convert-templates
	@test -d $(BUILD_DIR) || mkdir $(BUILD_DIR)
	@echo "\n[ Сборка проекта ]"
	$(CC) $(CFLAGS) $(C_FILES) -o $(BUILD_DIR)/ccskel -cpp my_cpp_app


.PHONY: build-and-run
build-and-run: build start

.PHONY: start
start:
	@echo "\n[ Старт проекта ]"
	@build/main.o

.PHONY: convert-templates
convert-templates:
	@echo "\n[ Конвертация шаблонов файлов ]"
	xxd -i $(TEMPLATES_DIR)/makefile.txt > $(TEMPLATES_DIR)/makefile.h
	xxd -i $(TEMPLATES_DIR)/cmake.txt > $(TEMPLATES_DIR)/cmake.h
	xxd -i $(TEMPLATES_DIR)/main-c.txt > $(TEMPLATES_DIR)/main-c.h
	xxd -i $(TEMPLATES_DIR)/main-cpp.txt > $(TEMPLATES_DIR)/main-cpp.h

.PHONY: clean
clean:
	@echo "\n[ Удаление скомпилированного проекта ]"
	rm -rf build