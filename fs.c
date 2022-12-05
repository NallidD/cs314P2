#include "fs.h"

unsigned char inode_bitmap[4096];
unsigned char data_bitmap[4096];
unsigned char* fs;
inode * inodes;
data_block * data;
int data_index = 0;
int inode_index = 0;
int root_inode = 2;

void init_blocks(super_block * sb, inode * node, free_list * fl) {

  memset(inode_bitmap, 0, BLOCK_SIZE);
  memset(data_bitmap, 0, BLOCK_SIZE);

  memset(sb->fs_type, 1, 8);
  sb->block_total = 4;
  sb->root_index = BLOCK_SIZE * 5;
  sb->data_index = BLOCK_SIZE * 5;
  sb->data_block_total = 1;

  node->ptr_to_block = 0;
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

  printf("pointer to block: %p \n", &node->ptr_to_block);

  printf(" }\nFBL bitmap table\n\t");

  for(int i = 0; i < 8; i++) {

    printf("%d", get_bit(fl->list[0], (i % 8) + 1)); //something weird is happening, either uninitialized memory or index out of range

  }

  printf("\n\t");

  for(int i = 0; i < 8; i++) {

    printf("%d", get_bit(fl->list[1], (i % 8) + 1)); //something weird is happening, either uninitialized memory or index out of range

  }

  printf("\n");

}

void print_buffer() {

  for(int i = 0; i < FSSIZE; i++) {

    printf("%d", fs[i]);

    if(i % 128 == 0 && i > 0) {

      printf("\n");

    }

  }

  printf("\n");

}

void print_block(int bd) {

    int index = bd * BLOCK_SIZE;

    for(int i = index; index < BLOCK_SIZE * index; i++) {

      printf("%d", fs[index]);

      if(index % 4096 == 0 && index > 0) {

        puts("\n");

      }

    }

    puts("\n");

}

void rprint_block(int from, int to) {

  if(from  > to) {

    perror("Function call rprint_block: from is greater than to\n");
    exit(EXIT_FAILURE);

  }

  if(to > FSSIZE) {

    perror("Function call rprint_block: to is greater than FSSIZE\n");
    exit(EXIT_FAILURE);

  }

  if(from < 0 || to < 0) {

    perror("Function call rprint_block: from or to cannot be less than 0\n");
    exit(EXIT_FAILURE);

  }

  for(int i = from; i < to; i++) {

    printf("%d", fs[i]);

    if(i % 128 == 0 && i > 0) {

      printf("\n");

    }

  }

  printf("\n");

}

unsigned int get_next_data_block(unsigned int curindex) {

  return DATA_START + ((curindex + 1) * 512);

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

  struct SB * sb = (struct SB *)malloc(sizeof(struct SB));
  struct FBL * fl;

  memset(fs, 0, BLOCK_SIZE);
  memset(inode_bitmap, 0, BLOCK_SIZE);
  memset(data_bitmap, 0, BLOCK_SIZE);
  memcpy(fs + NODE_TABLE_OFFSET, inode_bitmap, BLOCK_SIZE);
  memcpy(fs + DATA_TABLE_OFFSET, data_bitmap, BLOCK_SIZE);

  //inodes need 63 4K blocks
  //data needs 2,494 4K blocks

  inodes = (inode *)malloc(100 * sizeof(struct IN));
  data = (data_block *)malloc(2494 * sizeof(struct DB));
  memcpy(fs + INODE_START, inodes, sizeof(struct IN) * 100);
  memcpy(fs + (INODE_START * 63), data, sizeof(struct DB) * 2494);

  inode_bitmap[2] = '1';

  sb->inode_index = 0;
  sb->data_index = sb->root_index = INODE_START * 63;
  sb->data_block_total = 1;
  sb->block_total = 10; //1 for sb, and 2 bit maps, then 7 for the inodes.
  
  for(int i = 0; i < 8; i++) {

    sb->fs_type[i] = 1;

  }

  dir * root_dir = make_dir();
  inode * root = make_inode('d', sizeof(root_dir));
  
  sb->inode_index++;

  memcpy(fs, sb, sizeof(struct SB));
  
}

inode * make_inode(int size, char type) {

  inode * node = (inode *)malloc(sizeof(inode));
  node->size = size;
  node->type = type;
  node->num_blocks = size / BLOCK_SIZE + ((size % BLOCK_SIZE) != 0);
  node->valid = 0;

  return node;

}

void write_inode(inode * iptr, int inum) {

  char * t = (char *)malloc(sizeof(char) * 256);
  t = (char *)iptr;
  int i = 12288 + (inum * 256);

  if(i != 0)
    return;

  iptr->valid = 1;
  
  memset(fs, *t, 256);

}

dir * make_dir() {

  dir * d = (dir *)malloc(sizeof(dir));
  d->num_files = 0;
  return d;

}


void loadfs(){



}


void lsfs(){

  //iterate through all the inodes
  //directory or file will be in the inodes
  /*
    root
      |
      |---dir1
      |    |--file.txt
      |    |---hellodir
      |           |
      |           |
      |---dir2
      |     |--img.jpg
      |     |--file2.txt
      |     |--file.txt
  */
  
}

void addfilefs(char* fname){

  int fd = open(fname, O_RDWR, 0700); //open file to add

  struct SB * sb;
  memcpy(sb, fs, BLOCK_SIZE);

  if(fd == -1) {

    perror("File not found.\n");
    exit(EXIT_FAILURE);

  }

  int pos = lseek(fd, 0, SEEK_CUR); //get cur pos in file

  lseek(fd, 0, SEEK_END); //go to end of file

  int size = lseek(fd, 0, SEEK_CUR); //get cur size of file

  lseek(fd, pos, SEEK_SET); //reset position to beginning of file

  if(size > 512) {

    perror("File is larger than 512 bytes.\n");
    exit(EXIT_FAILURE);

  }

  *(inodes + sb->inode_index) = *make_inode(size, FSFILE);
  sb->inode_index++;

  int read_test = read(fd, fs + DATA_START, size);

  if(read_test != size) {

    perror("Could not read file to buffer.\n");
    printf("Errno: %s\n", strerror(errno));
    exit(EXIT_FAILURE);

  }

  #ifdef D

  if(D) {

    print_buffer();

  }

  #endif

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

  //reads all the file contents into a redirect file.
  //./filefs -e a/b/c -f fs > foobar

  /*
  * 
  * Super block
  * char fs_type[8];
  * short block_total;
  * short root_index; //data root index
  * short data_index; //data start index
  * char data_block_total;
  * 
  * 
  *   ./filefs -e a/b/c -f fs > outputfile
  *   Puts file contents 'c' into outputfile
  *   Read super block's root index to get to the first data section
  *   Check if 'a' exists by checking if the root's "next" field points to that directory
  *   Descend until we hit file 'c'
  *   Read c from the buffer and printf it
  * 
  */

}
