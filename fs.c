#include "fs.h"

unsigned char* fs;

void init_blocks(super_block * sb, inode * node, free_list * fl) {

  for(int i = 0; i < 8; i++) {

      sb->fs_type[i] = (char)256;

  }

  sb->block_total = 5; // 1 super block, 4 inodes
  sb->root_index = sizeof(struct SB) + (sizeof(struct IN) * 4);
  sb->data_index = sizeof(struct SB) + (sizeof(struct IN) * 4);
  sb->data_block_total = 1;
  sb->num_blocks = 6;

  for(int i = 0; i < 100; i++) {

    node->direct[i] = 0;

  }

  fl->list[0] = make_bitmap(8);
  fl->list[1] = make_bitmap(8);

  for(int i = 0; i < 8; i++) {

    set_bit(fl->list[0], i + 1);

  }

  for(int i = 0; i < 8; i++) {

    set_bit(fl->list[1], i + 1);

  }

  node->valid = 0;
  node->size = 0;

}

void print_blocks(super_block * sb, inode * node, free_list * fl) {

  printf("Struct SB: %ld\tStruct IN: %ld\tStruct FBL: %ld\n", sizeof(struct SB), sizeof(struct IN), sizeof(struct FBL));
  printf("Name-value pairings of struct files.\n");
  printf("{ SB->fs_type[0...7], ");

  for(int i = 0; i < 8; i++) {

    printf("%d", sb->fs_type[i]);

  }

  printf(" }\t{ SB->block_total, %d }\t{ SB->root_index, %d }\t{ SB->data_index, %d }\t{ SB->data_block_total, %d }\t{ SB->num_blocks, %d }\n",
  sb->block_total, sb->root_index, sb->data_index, sb->data_block_total, sb->num_blocks);
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

void mapfs(int fd){
  if ((fs = mmap(NULL, FSSIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0)) == NULL){
      perror("mmap failed");
      exit(EXIT_FAILURE);
  }
}


void unmapfs(){
  munmap(fs, FSSIZE);
}


void formatfs(){

  struct SB * sb;
  struct IN * node;
  struct FBL * fl;
  int offset = SBBEGIN;

  sb = (struct SB *)malloc(sizeof(struct SB));
  node = (struct IN *)malloc(sizeof(struct IN));
  fl = (struct FBL *)malloc(sizeof(struct FBL));
  
  init_blocks(sb, node, fl);
  
  memmove(fs, sb, sizeof(struct SB));
  memmove(fs + sizeof(struct SB), node, sizeof(struct IN));
  memmove(fs + sizeof(struct SB) + sizeof(struct IN), fl, sizeof(struct FBL));

  print_blocks(sb, node, fl);
  print_buffer();

}


void loadfs(){

}


void lsfs(){
  
}

void addfilefs(char* fname){

}


void removefilefs(char* fname){

}


void extractfilefs(char* fname){

}
