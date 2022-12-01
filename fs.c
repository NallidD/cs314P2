#include "fs.h"

unsigned char inode_bitmap[4096];
unsigned char data_bitmap[4096];
unsigned char* fs;

void init_blocks(super_block * sb, inode * node, free_list * fl) {

  memset(inode_bitmap, 0, BLOCK_SIZE);
  memset(data_bitmap, 0, BLOCK_SIZE);

  memset(sb->fs_type, 1, 8);
  sb->block_total = 4;
  sb->root_index = BLOCK_SIZE * 5;
  sb->data_index = BLOCK_SIZE * 5;
  sb->data_block_total = 1;

  memset(node->direct, 0, 100);
  node->valid = 1;
  node->size = 0;
  node->type = 1;

  for(int i = 0; i < 8; i++) {

    fl->list[i] = make_bitmap(8);
    memset(fl->list + i, 0, 8);

  }

}

void print_blocks(super_block * sb, inode * node, free_list * fl) {

  printf("Struct SB: %ld\tStruct IN: %ld\tStruct FBL: %ld\n", sizeof(struct SB), sizeof(struct IN), sizeof(struct FBL));
  printf("Name-value pairings of struct files.\n");
  printf("{ SB->fs_type[0...7], ");

  for(int i = 0; i < 8; i++) {

    printf("%d", sb->fs_type[i]);

  }

  printf(" }\t{ SB->block_total, %d }\t{ SB->root_index, %d }\t{ SB->data_index, %d }\t{ SB->data_block_total, %d }\t\n",
  sb->block_total, sb->root_index, sb->data_index, sb->data_block_total);
  printf("{ IN->valid, %d }\t{ IN->size, %d }\t{IN->direct[0...100], ", node->valid, node->size);

  for(int i = 0; i < 100; i++) {

    printf("%d", node->direct[i]);

  }

  printf(" }\nFBL bitmap table\n\t");

  for(int i = 0; i < 8; i++) {

    printf("%d", get_bit(fl->list[0], (i % 8) + 1));

  }

  printf("\n\t");

  for(int i = 0; i < 8; i++) {

    printf("%d", get_bit(fl->list[1], (i % 8) + 1));

  }

  printf("\n");

}

void print_buffer() {

  long long sb = sizeof(struct SB);
  long long node = sizeof(struct IN);
  long long fl = sizeof(struct FBL);

  printf("Super Block: \n");

  for(int i = 0; i < sb; i++) {

    printf("%d", fs[i]);

    if(i % 128 == 0 && i > 0) {

      printf("\n");

    }

  }

  printf("\n");

  printf("INode blocks (4): \n");

  for(int i = sb; i < sb + (node * 4); i++) {

    printf("%d", fs[i]);

    if(i % 128 == 0 && i > 0) {

      printf("\n");

    }

  }

  printf("\nFree list: \n");

  for(int i = sb + (node * 4); i < sb + (node * 4) + fl; i++) {

    printf("%d", fs[i]);

    if(i % 128 == 0 && i > 0) {

      printf("\n");

    }

  }

  printf("\n");

}

void write_to_buffer(super_block * sb, inode * node, free_list * fl) {

  for(int i = 0; i < 8; i++) {

    fs[i] = sb->fs_type[i];

  }

}

unsigned int get_next_data_block() {



}

void mapfs(int fd){
  if ((fs = mmap(NULL, FSSIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0)) == NULL) {
      perror("mmap failed");
      exit(EXIT_FAILURE);
  }
}


void unmapfs(){
  munmap(fs, FSSIZE);
}


void formatfs() {

  struct SB * sb;
  struct IN * node;
  struct FBL * fl;

  sb = (struct SB *)malloc(sizeof(struct SB));
  node = (struct IN *)malloc(sizeof(struct IN));
  fl = (struct FBL *)malloc(sizeof(struct FBL));
  
  init_blocks(sb, node, fl);

  write_to_buffer(sb, node, fl);

  print_blocks(sb, node, fl);
  print_buffer();

  free(sb);
  free(node);
  free(fl);

}

inode * make_inode(int size, char type) {

  inode * node = (inode *)malloc(sizeof(inode));
  node->size = size;
  node->type = type;
  node->num_blocks = size / BLOCK_SIZE + ((size % BLOCK_SIZE) != 0);
  node->valid = 0;

  return node;

}


void loadfs(){



}


void lsfs(){
  
}

void addfilefs(char* fname){

  int fd = open(fname, O_WRONLY, 0700); //open file to add

  if(fd == -1) {

    perror("File not found.\n");
    exit(EXIT_FAILURE);

  }

  int pos = lseek(fd, 0, SEEK_CUR); //get cur pos in file

  lseek(fd, 0, SEEK_END); //go to end of file

  int size = lseek(fd, 0, SEEK_CUR); //get cur size of file

  lseek(fd, pos, SEEK_SET); //reset position to beginning of file

  if(read(fd, fs, size) != size) {

    perror("Could not read file to buffer.\n");
    exit(EXIT_FAILURE);

  }

}


void removefilefs(char* fname){

  int fd = open(fname, O_RDWR);

  if(fd == -1) {

    perror("File not found.\n");
    exit(EXIT_FAILURE);

  }

  FILE * file = fdopen(fd, "wr");

  int pos = ftell(file);

  fseek(file, 0, SEEK_END);

  int size = ftell(file);

  char c = fgetc(file);

  while(c != EOF) {

    printf("%c", c);
    c = fgetc(file);

  }

  fseek(file, 0, SEEK_SET);

  for(int i = pos; i < size; i++) {

    fs[i] = 0;

  }

}


void extractfilefs(char* fname){

}
