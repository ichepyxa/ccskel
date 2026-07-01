#include <stdio.h>
#include <getopt.h>
#include <sys/stat.h>
#include <sys/dir.h>
#include <stdlib.h>
#include <string.h>

#include "types/directories.h"
#include "types/config.h"
#include "templates/makefile.h"
#include "templates/cmake.h"
#include "templates/main-c.h"
#include "templates/main-cpp.h"

// Выводит помощь в консоль
void print_usage(char *program_name) {
  printf("Использование: %s [ОПЦИИ]\n", program_name);
  printf("Опции:\n");
  printf("  --help (-h)                Помощь\n");
  printf("  --dir (-d) <директория>    Указать директорию для создания проекта\n");
  printf("  --cmake                    Использует в проекте cmake вместо makefile\n");
  printf("  --cpp                      Создает C++ проект вместо C\n");
}

// Проверка на существование, в случае отсутствия создание директории по указанному пути
void ensure_directory(const char *path) {
  struct stat sb;
  if (stat(path, &sb) != 0) {
    printf("Создание директории: %s\n", path);
    mkdir(path, 0755);
  }
}

// Создание файла по указанному пути
void create_file(const char *path) {
  printf("Создание файла: %s\n", path);

  FILE *f = fopen(path, "w");
  if (f) fclose(f);
}

// Заполяем Makefile/CMake
void fill_makefile(const char *path, int is_cmake) {
  FILE *f = fopen(path, "wb");
  if (f == NULL) {
    perror("Ошибка открытия файла для записи");
    return;
  }

  if (is_cmake) {
    fwrite(templates_cmake_txt, 1, templates_cmake_txt_len, f);
  } else {
    fwrite(templates_makefile_txt, 1, templates_makefile_txt_len, f);
  }

  fclose(f);

  printf("Makefile успешно заполнен по шаблону\n");
}

// Заполяем main.c/main.cpp
void fill_main(const char *path, int is_cpp) {
  FILE *f = fopen(path, "wb");
  if (f == NULL) {
    perror("Ошибка открытия файла для записи");
    return;
  }

  if (is_cpp) {
    fwrite(templates_main_cpp_txt, 1, templates_main_cpp_txt_len, f);
  } else {
    fwrite(templates_main_c_txt, 1, templates_main_c_txt_len, f);
  }

  fclose(f);

  printf("main файл успешно заполнен по шаблону\n");
}

// Создание структуру проекта
void create_structure(const DirectoryItem *structure, const char *directory_path) {
  for (int i = 0; structure[i].name != NULL; i++) {
    char temp_path[1024];
    snprintf(temp_path, sizeof(temp_path), "%s%s", directory_path, structure[i].name);

    if (structure[i].type == TYPE_DIR) {
      // Создание директории
      ensure_directory(temp_path);
      
      if (structure[i].items != NULL) {
        create_structure(structure[i].items, temp_path);
      }
    } else {
      // Создание файла
      create_file(temp_path);

      // Заполняем файлы данными
      if (strcmp(structure[i].name, "/Makefile") == 0) {
        fill_makefile(temp_path, 0);
      } else if (strcmp(structure[i].name, "/main.c") == 0) {
        fill_main(temp_path, 0);
      } else if (strcmp(structure[i].name, "/CMakeList.txt") == 0) {
        fill_makefile(temp_path, 1);
      } else if (strcmp(structure[i].name, "/main.cpp") == 0) {
        fill_main(temp_path, 1);
      } 
    }
  }
}

// Печатаем арт при успешном создании проекта
void print_done_art() {
  printf("\t\t\033[0;31m  _____           _           _      _____ _____  ______       _______ ______ _____  \n");
  printf("\t\t\033[0;31m |  __ \\         (_)         | |    / ____|  __ \\|  ____|   /\\|__   __|  ____|  __ \\ \n");
  printf("\t\t\033[0;31m | |__) | __ ___  _  ___  ___| |_  | |    | |__) | |__     /  \\  | |  | |__  | |  | | \n");
  printf("\t\t\033[0;31m |  ___/ '__/ _ \\| |/ _ \\/ __| __| | |    |  _  /|  __|   / /\\ \\ | |  |  __| | |  | |\n");
  printf("\t\t\033[0;32m | |   | | | (_) | |  __/ (__| |_  | |____| | \\ \\| |____ / ____ \\| |  | |____| |__| |\n");
  printf("\t\t\033[0;32m |_|   |_|  \\___/| |\\___|\\___|\\__|  \\_____|_|  \\_\\______/_/    \\_\\_|  |______|_____/ \n");
  printf("\t\t\033[0;32m                _/ |                                                                 \n");
  printf("\t\t\033[0;32m               |__/                                                                  \n");
}

// Основная функция создания проекта
void create_project(const char *directory_path, const DirectoryItem *project_structure) {
  ensure_directory(directory_path);

  create_structure(project_structure, directory_path);

  print_done_art();
}

// Создает C проект
void create_c_project(const char *directory_path, int has_cmake_flag) {
  const DirectoryItem external_directory_structure[3] = {
    {"/include", TYPE_DIR, NULL},
    {"/lib", TYPE_DIR, NULL},
    {NULL, 0, NULL}
  };

  const DirectoryItem src_directory_structure[2] = {
    {"/main.c", TYPE_FILE, NULL},
    {NULL, 0, NULL}
  };

  const DirectoryItem project_structure[6] = {
    {"/include", TYPE_DIR, NULL},
    {"/external", TYPE_DIR, external_directory_structure},
    {"/assets", TYPE_DIR, NULL},
    {"/src", TYPE_DIR, src_directory_structure},
    {has_cmake_flag ? "/CMakeList.txt" : "/Makefile", TYPE_FILE, NULL},
    {NULL, 0, NULL}
  };

  create_project(directory_path, project_structure);
}

// Создает C++ проект
void create_cpp_project(const char *directory_path, int has_cmake_flag) {
  const DirectoryItem external_directory_structure[3] = {
    {"/include", TYPE_DIR, NULL},
    {"/lib", TYPE_DIR, NULL},
    {NULL, 0, NULL}
  };

  const DirectoryItem src_directory_structure[2] = {
    {"/main.cpp", TYPE_FILE, NULL},
    {NULL, 0, NULL}
  };

  const DirectoryItem project_structure[6] = {
    {"/include", TYPE_DIR, NULL},
    {"/external", TYPE_DIR, external_directory_structure},
    {"/assets", TYPE_DIR, NULL},
    {"/src", TYPE_DIR, src_directory_structure},
    {has_cmake_flag ? "/CMakeList.txt" : "/Makefile", TYPE_FILE, NULL},
    {NULL, 0, NULL}
  };

  create_project(directory_path, project_structure);
}

// Начало выполнения программы
int main(int argc, char* argv[]) {
  if (argc == 1) {
    print_usage(argv[0]);
    return 0;
  }
  
  struct option long_options[] = {
    {"help", no_argument, 0, 'h'},
    {"dir", required_argument, 0, 'd'},
    {"cmake", no_argument, 0, 1001},
    {"cpp", no_argument, 0, 1002},
    {0, 0, 0, 0}
  };

  struct OptionList option_list = {
    .directory_option = {
      .has_dir_flag = FLAG_NO,
      .path = ""
    },
    .has_cpp_flag = FLAG_NO,
    .has_cmake_flag = FLAG_NO
  };
  
  int opt;
  while ((opt = getopt_long(argc, argv, "hd:m", long_options, NULL)) != -1) {
    switch (opt) {
      case 'h':
        print_usage(argv[0]);
        return 0;
      case 'd':
        option_list.directory_option.has_dir_flag = FLAG_YES;
        option_list.directory_option.path = optarg;
        continue;
      case 1001:
        option_list.has_cmake_flag = FLAG_YES;
        continue;
      case 1002:
        option_list.has_cpp_flag = FLAG_YES;
        continue;
      case '?':
        fprintf(stderr, "Используйте '%s -h' для справки.\n", argv[0]);
        return 1;
    }
  }

  if (option_list.directory_option.has_dir_flag && option_list.has_cpp_flag) {
    create_cpp_project(option_list.directory_option.path, option_list.has_cmake_flag);
  } else {
    create_c_project(option_list.directory_option.path, option_list.has_cmake_flag);
  }

  return 0;
}