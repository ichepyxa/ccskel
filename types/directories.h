#define TYPE_FILE 0
#define TYPE_DIR 1

typedef struct DirectoryItem {
  char *name;
  int type; // TYPE_DIR или TYPE_FILE
  const struct DirectoryItem *items;
} DirectoryItem;