#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <time.h>

#include "ext2_fs.h"

unsigned long s_log_block_size = 0;
unsigned long size;
unsigned long size2;

int fd;

ssize_t ret;

void directory(unsigned int inodeNumber, unsigned int block2){
  char block[s_log_block_size];
  ret = pread(fd, block, s_log_block_size, block2 * s_log_block_size); 
  struct ext2_dir_entry* entry = (struct ext2_dir_entry *)block;          
  size = 0;
  //size2 = 0;
  while(size < s_log_block_size)  { //size is less than the total size of the inode
    
    char file_name[EXT2_NAME_LEN+1];
    memcpy(file_name, entry->name, entry->name_len);
    file_name[entry->name_len] = 0;/* append null char to the file name */
    if (entry->inode != 0){
      printf("DIRENT,%d,%ld,%d,%d,%d,'%s'\n", inodeNumber, size2, entry->inode, entry->rec_len, entry->name_len, file_name);
      fflush(stdout);
    }
    size += entry->rec_len;
    size2 += entry->rec_len;
    //unsigned int x = entry->rec_len;
    entry = (void*) entry + entry->rec_len;      /* move to the next entry */
    //size += entry->rec_len;
    //size2 += x;
  }

}

void indirect(char fileType, struct ext2_inode *inode, unsigned int inodeNumber, unsigned long block2, unsigned int level, unsigned int buff){
  unsigned int count = s_log_block_size/4;
  unsigned int block[count];
  ret = pread(fd, block, s_log_block_size, block2*s_log_block_size);
  //size2 = 0;
  if((unsigned)ret < sizeof(s_log_block_size)){
    fprintf(stderr, "Error in pread");
    exit(2);
    }
  if(ret < 0){
    fprintf(stderr, "Error in reading file");
    exit(1);
  }

  
  for(unsigned int i = 0; i < count; i++){
    //size2 = 0;
    if(block[i] != 0){
     printf("INDIRECT,%d,%d,%d,%ld,%d\n", inodeNumber, level, buff, block2, block[i]);
     fflush(stdout);
    if(level == 1 && fileType == 'd')
      directory(inodeNumber, block[i]);
        
    if(level == 3){
      indirect(fileType, inode, inodeNumber, block[i], level-1, buff);
      buff += 65536;
    }
    if(level == 2){
      indirect(fileType, inode, inodeNumber, block[i], level-1, buff);
      buff += 256;
      }

    }

    if(level == 1)
      buff++;

  }
}

int main(int argc, char* argv[]){
  //int fd = 0;
  if(argc != 2){
    fprintf(stderr, "Unrecognized argument");
    exit(1);
  }
  
  struct ext2_super_block super;

  //SUPERBLOCK STRUCTURE VARIABLES
  unsigned int s_blocks_count = 0, s_inodes_count = 0,
    s_blocks_per_group = 0, s_inodes_per_group = 0,
    first_nonreserved_inode = 0;

    
  fd = open(argv[1], O_RDONLY);

  if(fd < 0){
    fprintf(stderr, "Error opening file");
    exit(1);
  }

  ret = pread(fd, &super, sizeof(super), 1024);
  if((unsigned)ret < sizeof(super)){
    fprintf(stderr, "Error in pread");
    exit(2);
    }
  if(ret < 0){
    fprintf(stderr, "Error in reading file");
    exit(1);
  }

  if (super.s_magic != EXT2_SUPER_MAGIC){
    fprintf(stderr, "Bad File System");
    exit(1); /* bad file system */
  }
  
  s_blocks_count = super.s_blocks_count;
  s_inodes_count = super.s_inodes_count;
  s_blocks_per_group = super.s_blocks_per_group;
  s_inodes_per_group = super.s_inodes_per_group;
  first_nonreserved_inode = super.s_first_ino;

  s_log_block_size = 1024 << super.s_log_block_size;
  //unsigned int s_log_inode_size = 1024 << super.s_inode_size;
  unsigned long s_inode_size = super.s_inode_size;
  printf("SUPERBLOCK,%d,%d,%ld,%d,%d,%d,%d\n", s_blocks_count, s_inodes_count, s_log_block_size, super.s_inode_size, s_blocks_per_group, s_inodes_per_group,
	 first_nonreserved_inode);
  fflush(stdout);

  //BLOCK GROUP
  unsigned int bg_blocks_count = 0, 
    bg_inodes_count = 0, bg_free_blocks_count = 0, bg_free_inodes_count = 0, 
    bg_block_bitmap = 0, bg_inode_bitmap = 0, bg_inode_table = 0;

  struct ext2_group_desc group_descr;
  
  //POSSIBLE ERROR OFFSET
  if(s_log_block_size > 1024)
    ret = pread(fd, &group_descr, sizeof(group_descr), 1024);
  else
    ret = pread(fd, &group_descr, sizeof(group_descr), 1024+s_log_block_size);
  if((unsigned)ret < sizeof(group_descr)){
    fprintf(stderr, "Error in pread");
    exit(2);
    }
  if(ret < 0){
    fprintf(stderr, "Error in reading file");
    exit(1);
  }

  //close(fd);

  //group number will always be 0
  bg_blocks_count = s_blocks_count; //should it be total blocks or blocks/group
  bg_inodes_count = s_inodes_count;
  bg_free_blocks_count = group_descr.bg_free_blocks_count;
  bg_free_inodes_count = group_descr.bg_free_inodes_count;
  bg_block_bitmap = group_descr.bg_block_bitmap;
  bg_inode_bitmap = group_descr.bg_inode_bitmap;
  bg_inode_table = group_descr.bg_inode_table;

  printf("GROUP,0,%d,%d,%d,%d,%d,%d,%d\n", bg_blocks_count, bg_inodes_count, bg_free_blocks_count, bg_free_inodes_count, bg_block_bitmap,
	 bg_inode_bitmap, bg_inode_table);
  fflush(stdout);

  //BFREE and IFREE
  
  //char *bitmap = (char *)malloc(s_log_block_size);
  //pread(fd, &bitmap, s_log_block_size, ((bg_block_bitmap)*s_log_block_size));
  for(unsigned long i = 0; i < s_log_block_size; i++){
    char b;
    ret = pread(fd, &b, s_log_block_size, ((bg_block_bitmap)*s_log_block_size) + i);
    if((unsigned)ret < s_log_block_size){
      fprintf(stderr, "Error in pread");
      exit(2);
    }
    if(ret < 0){
      fprintf(stderr, "Error in reading file");
      exit(1);
    }

    for(unsigned int j = 0; j < 8; j++){
      if((b & 1) == 0){
	unsigned int blocknum = (i*8) + j;
	blocknum++;
	printf("BFREE,%d\n", blocknum);
      }
      b >>= 1;
    }
  }
  fflush(stdout);

  //free(bitmap);

  //char *bitmap2 = (char *)malloc(s_log_block_size);
  //pread(fd, &bitmap2, s_log_block_size, ((bg_inode_bitmap)*s_log_block_size));
  for(unsigned int i = 0; i < s_log_block_size; i++){
    char b2;
    ret = pread(fd, &b2, s_log_block_size, ((bg_inode_bitmap)*s_log_block_size)+i);
    if((unsigned)ret < s_log_block_size){
      fprintf(stderr, "Error in pread");
      exit(2);
    }
    if(ret < 0){
      fprintf(stderr, "Error in reading file");
      exit(1);
    }

    for(unsigned int j = 0; j < 8; j++){
      if((b2 & 1) == 0){
        unsigned int inodenum = (i*8) + j;
        inodenum++;
        printf("IFREE,%d\n", inodenum);
      }
      b2 >>= 1;
    }
  }
  fflush(stdout);

 
  //INODE
  struct ext2_inode inode;
  
  for(unsigned long i = 0; i < s_inodes_count; i++){
    pread(fd, &inode, sizeof(inode), (bg_inode_table * s_log_block_size) + (i*s_inode_size));
        
    if(inode.i_mode != 0 && inode.i_links_count != 0){

    char fileType = '?';
    if(S_ISDIR(inode.i_mode))
      fileType = 'd';
    if(S_ISREG(inode.i_mode))
      fileType = 'f';
    if(S_ISLNK(inode.i_mode))
      fileType = 's';
      

    time_t c_time = (time_t)inode.i_ctime;
    time_t m_time = (time_t)inode.i_mtime;
    time_t a_time = (time_t)inode.i_atime;
    struct tm ctime;
    struct tm mtime;
    struct tm atime;
    gmtime_r(&c_time, &ctime);
    gmtime_r(&m_time, &mtime);
    gmtime_r(&a_time, &atime);

    char str1[18];
    char str2[18];
    char str3[18];

    strftime(str1, 18, "%D %H:%M:%S", &ctime);
    strftime(str2, 18, "%D %H:%M:%S", &mtime);
    strftime(str3, 18, "%D %H:%M:%S", &atime);
    
    printf("INODE,%ld,%c,%o,%d,%d,%d,%s,%s,%s,%d,%d", (i+1), fileType, (inode.i_mode & 0xfff), inode.i_uid, inode.i_gid, inode.i_links_count, str1, str2, str3, inode.i_size, inode.i_blocks);
    fflush(stdout);

    if(fileType == 's' && inode.i_size < 60)
      ;
    
    else{
      for(unsigned long j = 0; j < 15; j++){
	printf(",%d", inode.i_block[j]);
	fflush(stdout);
      }
    }

    printf("\n");
    fflush(stdout);

    //DIRENT
    if(fileType == 'd'){
      size2 = 0;
      for(int k = 0; k < 12; k++){
	if(inode.i_block[k] != 0)
	  directory(i+1, inode.i_block[k]);
      }
    }
    //INDIRECT
    //size2 = 0;
    if(inode.i_block[12] != 0){
      indirect(fileType, &inode, i+1, inode.i_block[12], 1, 12);
    }

    if(inode.i_block[13] != 0){
      indirect(fileType, &inode, i+1, inode.i_block[13], 2, 12+(16*16));
    }

    if(inode.i_block[14] != 0){
      indirect(fileType, &inode, i+1, inode.i_block[14], 3, 12+(16*16)+(256*256));
    }
    
        
    }
  
  }

  exit(0);
}
