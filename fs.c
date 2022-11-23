#include "fs.h"

unsigned char* fs;

void init_blocks(super_block * sb, inode * node, free_list * fl) {

  for(int i = 0; i < 4096; i++) {

    


  }

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
  struct FBL * fl;
  struct IN * node;
  int offset = SBBEGIN;

  sb = (struct SB *)malloc(sizeof(struct SB));
  fl = (struct FBL *)malloc(sizeof(struct FBL));
  node = (struct IN *)malloc(sizeof(struct IN));

  sb->block;
  fl->list[0] = 0;
  fl->list[1] = 0;
  node->valid = 0;
  node->size = 0;
  
  for(int i = 0; i < 100; i++) {

    node->direct[i] = 0;

  }


  memccpy(fs, &sb->block, 0, sizeof(sb->block));
  memccpy(fs + 16, &fl->list, 0, sizeof(fl->list));
  memccpy(fs + 17, &node->valid, 0, sizeof(node->valid));
  memccpy(fs + 18, &node->size, 0, sizeof(node->size));
  memccpy(fs + 19, &node->direct, 0, sizeof(node->direct));

  printf("super block size: %ld, free list size: %ld, node valid size: %ld, node size size: %ld, node direct size: %ld\n, total: %ld",
        sizeof(struct SB), sizeof(struct FBL), sizeof(node->valid), sizeof(node->size), sizeof(node->direct),
        sizeof(sb) + sizeof(fl) + sizeof(node->valid) + sizeof(node->size) + sizeof(node->direct));

  // printf("super block: %d - %d, free list: %d - %d, node valid: %d - %d, node size: %d - %d, node direct: %d \n", 
  //         fs[SBBEGIN], fs[SBEND], fs[FLBEGIN], fs[FLEND], fs[INBEGIN], fs[INBEGIN + 1], fs[INBEGIN + 2], fs[INBEGIN + 3], fs[INEND]);


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
