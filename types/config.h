#define FLAG_NO 0
#define FLAG_YES 1

typedef struct OptionList {
  struct DirectoryOption {
    int has_dir_flag;
    char *path;
  } directory_option;

  int has_cmake_flag;
  int has_cpp_flag;
} OptionList;