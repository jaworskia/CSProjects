/*
        FUSE: Filesystem in Userspace
        Copyright (C) 2001-2007  Miklos Szeredi <miklos@szeredi.hu>

        This program can be distributed under the terms of the GNU GPL.
        See the file COPYING.

*/

//Adam Jaworski
//CS 1550
//Project 4
//November 24, 2014

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>

//size of a disk block
#define BLOCK_SIZE 512

//we'll use 8.3 filenames
#define MAX_FILENAME 8
#define MAX_EXTENSION 3

//How many files can there be in one directory?
#define MAX_FILES_IN_DIR (BLOCK_SIZE - (MAX_FILENAME + 1) - sizeof(int)) / \
        ((MAX_FILENAME + 1) + (MAX_EXTENSION + 1) + sizeof(size_t) + sizeof(long))

//How much data can one block hold?
#define MAX_DATA_IN_BLOCK BLOCK_SIZE

//How many pointers in an inode?
#define NUM_POINTERS_IN_INODE ((BLOCK_SIZE - sizeof(unsigned int) - sizeof(unsigned long)) / sizeof(unsigned long))


struct cs1550_directory_entry
{
        char dname[MAX_FILENAME + 1];   //the directory name (plus space for a nul)
        int nFiles;                     //How many files are in this directory.
                                        //Needs to be less than MAX_FILES_IN_DIR
        struct cs1550_file_directory
        {
                char fname[MAX_FILENAME + 1];   //filename (plus space for nul)
                char fext[MAX_EXTENSION + 1];   //extension (plus space for nul)
                size_t fsize;                   //file size
                long nStartBlock;               //where the first block is on disk
        } files[MAX_FILES_IN_DIR];              //There is an array of these
};

typedef struct cs1550_directory_entry cs1550_directory_entry;

struct cs1550_disk_block
{
        //And all of the space in the block can be used for actual data
        //storage.
        char data[MAX_DATA_IN_BLOCK];
};

typedef struct cs1550_disk_block cs1550_disk_block;


int setBlockBit(int block)		//sets the bit in the bitmap corresponding to the block
{
	int mapBlock = block/4096;	//block will be 0-10239... will be in one of three blocks
	int newBlock = block%4096;	//which bit of the appropriate block we want
	int character = newBlock/8;	//which character of the block the bit is in
	int bit = newBlock%8;		//which bit of the character is the one we want
	FILE *disk;					//the disk file
	disk = fopen(".disk", "rb+");	//open for reading and writing, must exist
	cs1550_disk_block curr;
	unsigned char other;
	if (mapBlock == 0)	//we want to look in the first map block
	{
		fseek(disk, 0, SEEK_SET);	//seek to beginning of first disk block
		fread(&curr, sizeof(cs1550_disk_block), 1, disk);	//read in the disk block
		if (bit == 0)
		{
			printf("Setting: %d\n", block);
			printf("Block: %d, Character: %d, Bit: %d\n", mapBlock, character, bit);
			printf("Initial value: %d\n", curr.data[character]);
			other = 128;
			curr.data[character] = curr.data[character] | other;	//OR itself with 128, should guarantee first bit is set
			printf("After value: %d\n\n", curr.data[character]);
		}
		else if (bit == 1)
		{
			printf("Setting: %d\n", block);
			printf("Block: %d, Character: %d, Bit: %d\n", mapBlock, character, bit);
			printf("Initial value: %d\n", curr.data[character]);
			other = 64;
			curr.data[character] = curr.data[character] | other;	//should guarantee second bit is set
			printf("After value: %d\n\n", curr.data[character]);
		}
		else if (bit == 2)
		{
			printf("Setting: %d\n", block);
			printf("Block: %d, Character: %d, Bit: %d\n", mapBlock, character, bit);
			printf("Initial value: %d\n", curr.data[character]);
			other = 32;
			curr.data[character] = curr.data[character] | other;
			printf("After value: %d\n\n", curr.data[character]);
		}
		else if (bit == 3)
		{
			printf("Setting: %d\n", block);
			printf("Block: %d, Character: %d, Bit: %d\n", mapBlock, character, bit);
			printf("Initial value: %d\n", curr.data[character]);
			other = 16;
			curr.data[character] = curr.data[character] | other;
			printf("After value: %d\n\n", curr.data[character]);
		}
		else if (bit == 4)
		{
			printf("Setting: %d\n", block);
			printf("Block: %d, Character: %d, Bit: %d\n", mapBlock, character, bit);
			printf("Initial value: %d\n", curr.data[character]);
			other = 8;
			curr.data[character] = curr.data[character] | other;
			printf("After value: %d\n\n", curr.data[character]);
		}
		else if (bit == 5)
		{
			printf("Setting: %d\n", block);
			printf("Block: %d, Character: %d, Bit: %d\n", mapBlock, character, bit);
			printf("Initial value: %d\n", curr.data[character]);
			other = 4;
			curr.data[character] = curr.data[character] | other;
			printf("After value: %d\n\n", curr.data[character]);
		}
		else if (bit == 6)
		{
			printf("Setting: %d\n", block);
			printf("Block: %d, Character: %d, Bit: %d\n", mapBlock, character, bit);
			printf("Initial value: %d\n", curr.data[character]);
			other = 2;
			curr.data[character] = curr.data[character] | other;
			printf("After value: %d\n\n", curr.data[character]);
		}
		else if (bit == 7)
		{
			printf("Setting: %d\n", block);
			printf("Block: %d, Character: %d, Bit: %d\n", mapBlock, character, bit);
			printf("Initial value: %d\n", curr.data[character]);
			other = 1;
			curr.data[character] = curr.data[character] | other;
			printf("After value: %d\n\n", curr.data[character]);
		}
		fseek(disk, 0, SEEK_SET);	//seek back to beginning of the block
		fwrite(&curr, sizeof(cs1550_disk_block), 1, disk);	//write the block back in
	}
	else if (mapBlock == 1)	//look in the second map block
	{
		fseek(disk, 512, SEEK_SET);	//seek to beginning of second disk block
		fread(&curr, sizeof(cs1550_disk_block), 1, disk);	//read in the disk block
		if (bit == 0)
		{
			other = 128;
			curr.data[character] = curr.data[character] | other;	//OR itself with 128, should guarantee first bit is set
		}
		else if (bit == 1)
		{
			other = 64;
			curr.data[character] = curr.data[character] | other;	//should guarantee second bit is set
		}
		else if (bit == 2)
		{
			other = 32;
			curr.data[character] = curr.data[character] | other;
		}
		else if (bit == 3)
		{
			other = 16;
			curr.data[character] = curr.data[character] | other;
		}
		else if (bit == 4)
		{
			other = 8;
			curr.data[character] = curr.data[character] | other;
		}
		else if (bit == 5)
		{
			other = 4;
			curr.data[character] = curr.data[character] | other;
		}
		else if (bit == 6)
		{
			other = 2;
			curr.data[character] = curr.data[character] | other;
		}
		else if (bit == 7)
		{
			other = 1;
			curr.data[character] = curr.data[character] | other;
		}
		fseek(disk, 512, SEEK_SET);	//seek back to beginning of the block
		fwrite(&curr, sizeof(cs1550_disk_block), 1, disk);	//write the block back in
	}
	else	//look in the third and final map block
	{
		fseek(disk, 1024, SEEK_SET);	//seek to beginning of third disk block
		fread(&curr, sizeof(cs1550_disk_block), 1, disk);	//read in the disk block
		if (bit == 0)
		{
			other = 128;
			curr.data[character] = curr.data[character] | other;	//OR itself with 128, should guarantee first bit is set
		}
		else if (bit == 1)
		{
			other = 64;
			curr.data[character] = curr.data[character] | other;	//should guarantee second bit is set
		}
		else if (bit == 2)
		{
			other = 32;
			curr.data[character] = curr.data[character] | other;
		}
		else if (bit == 3)
		{
			other = 16;
			curr.data[character] = curr.data[character] | other;
		}
		else if (bit == 4)
		{
			other = 8;
			curr.data[character] = curr.data[character] | other;
		}
		else if (bit == 5)
		{
			other = 4;
			curr.data[character] = curr.data[character] | other;
		}
		else if (bit == 6)
		{
			other = 2;
			curr.data[character] = curr.data[character] | other;
		}
		else if (bit == 7)
		{
			other = 1;
			curr.data[character] = curr.data[character] | other;
		}
		fseek(disk, 1024, SEEK_SET);	//seek back to beginning of the block
		fwrite(&curr, sizeof(cs1550_disk_block), 1, disk);	//write the block back in
	}
	fclose(disk);
	return 0;
}

int clearBlockBit(int block)
{
	int mapBlock = block/4096;	//block will be 0-10239... will be in one of three blocks
	int newBlock = block%4096;	//which bit of the appropriate block we want
	int character = newBlock/8;	//which character of the block the bit is in
	int bit = newBlock%8;		//which bit of the character is the one we want
	FILE *disk;					//the disk file
	disk = fopen(".disk", "rb+");	//open for reading and writing, must exist
	cs1550_disk_block curr;
	unsigned char other;
	if (mapBlock == 0)	//we want to look in the first map block
	{
		fseek(disk, 0, SEEK_SET);	//seek to beginning of first disk block
		fread(&curr, sizeof(cs1550_disk_block), 1, disk);	//read in the disk block
		if (bit == 0)
		{
			other = 127;
			curr.data[character] = curr.data[character] & other;	//AND itself with 127, should guarantee first bit is clear
		}
		else if (bit == 1)
		{
			other = 191;
			curr.data[character] = curr.data[character] & other;	//should guarantee second bit is clear
		}
		else if (bit == 2)
		{
			other = 223;
			curr.data[character] = curr.data[character] & other;
		}
		else if (bit == 3)
		{
			other = 239;
			curr.data[character] = curr.data[character] & other;
		}
		else if (bit == 4)
		{
			other = 247;
			curr.data[character] = curr.data[character] & other;
		}
		else if (bit == 5)
		{
			other = 251;
			curr.data[character] = curr.data[character] & other;
		}
		else if (bit == 6)
		{
			other = 253;
			curr.data[character] = curr.data[character] & other;
		}
		else if (bit == 7)
		{
			other = 254;
			curr.data[character] = curr.data[character] & other;
		}
		fseek(disk, 0, SEEK_SET);	//seek back to beginning of the block
		fwrite(&curr, sizeof(cs1550_disk_block), 1, disk);	//write the block back in
	}
	else if (mapBlock == 1)	//look in the second map block
	{
		fseek(disk, 512, SEEK_SET);	//seek to beginning of second disk block
		fread(&curr, sizeof(cs1550_disk_block), 1, disk);	//read in the disk block
		if (bit == 0)
		{
			other = 127;
			curr.data[character] = curr.data[character] & other;	//AND itself with 127, should guarantee first bit is clear
		}
		else if (bit == 1)
		{
			other = 191;
			curr.data[character] = curr.data[character] & other;	//should guarantee second bit is clear
		}
		else if (bit == 2)
		{
			other = 223;
			curr.data[character] = curr.data[character] & other;
		}
		else if (bit == 3)
		{
			other = 239;
			curr.data[character] = curr.data[character] & other;
		}
		else if (bit == 4)
		{
			other = 247;
			curr.data[character] = curr.data[character] & other;
		}
		else if (bit == 5)
		{
			other = 251;
			curr.data[character] = curr.data[character] & other;
		}
		else if (bit == 6)
		{
			other = 253;
			curr.data[character] = curr.data[character] & other;
		}
		else if (bit == 7)
		{
			other = 254;
			curr.data[character] = curr.data[character] & other;
		}
		fseek(disk, 512, SEEK_SET);	//seek back to beginning of the block
		fwrite(&curr, sizeof(cs1550_disk_block), 1, disk);	//write the block back in
	}
	else	//look in the third and final map block
	{
		fseek(disk, 1024, SEEK_SET);	//seek to beginning of third disk block
		fread(&curr, sizeof(cs1550_disk_block), 1, disk);	//read in the disk block
		if (bit == 0)
		{
			other = 127;
			curr.data[character] = curr.data[character] & other;	//AND itself with 127, should guarantee first bit is clear
		}
		else if (bit == 1)
		{
			other = 191;
			curr.data[character] = curr.data[character] & other;	//should guarantee second bit is clear
		}
		else if (bit == 2)
		{
			other = 223;
			curr.data[character] = curr.data[character] & other;
		}
		else if (bit == 3)
		{
			other = 239;
			curr.data[character] = curr.data[character] & other;
		}
		else if (bit == 4)
		{
			other = 247;
			curr.data[character] = curr.data[character] & other;
		}
		else if (bit == 5)
		{
			other = 251;
			curr.data[character] = curr.data[character] & other;
		}
		else if (bit == 6)
		{
			other = 253;
			curr.data[character] = curr.data[character] & other;
		}
		else if (bit == 7)
		{
			other = 254;
			curr.data[character] = curr.data[character] & other;
		}
		fseek(disk, 1024, SEEK_SET);	//seek back to beginning of the block
		fwrite(&curr, sizeof(cs1550_disk_block), 1, disk);	//write the block back in
	}
	fclose(disk);
	return 0;
}


int isBlockFree(int block)
{
	int mapBlock = block/4096;	//block will be 0-10239... will be in one of three blocks
	int newBlock = block%4096;	//which bit of the appropriate block we want
	int character = newBlock/8;	//which character of the block the bit is in
	int bit = newBlock%8;		//which bit of the character is the one we want
	FILE *disk;					//the disk file
	disk = fopen(".disk", "rb+");	//open for reading and writing, must exist
	cs1550_disk_block curr;
	unsigned char other;
	unsigned char value;
	if (mapBlock == 0)	//we want to look in the first map block
	{
		fseek(disk, 0, SEEK_SET);	//seek to beginning of first disk block
		fread(&curr, sizeof(cs1550_disk_block), 1, disk);	//read in the disk block
		fclose(disk);
		if (bit == 0)	//if we're checking its first bit
		{
			other = 128;
			value = curr.data[character] & other;	//should wipe out everything but the first bit
			if (value > 0)		//first bit must have been a 1
				return 0;
			else		//must have been a 0
				return 1;
		}
		else if (bit == 1)
		{
			other = 64;
			value = curr.data[character] & other;	
			if (value > 0)
				return 0;
			else
				return 1;	
		}
		else if (bit == 2)
		{
			other = 32;
			value = curr.data[character] & other;	
			if (value > 0)
				return 0;
			else
				return 1;
		}
		else if (bit == 3)
		{
			other = 16;
			value = curr.data[character] & other;	
			if (value > 0)
				return 0;
			else
				return 1;
		}
		else if (bit == 4)
		{
			other = 8;
			value = curr.data[character] & other;	
			if (value > 0)
				return 0;
			else
				return 1;
		}
		else if (bit == 5)
		{
			other = 4;
			value = curr.data[character] & other;	
			if (value > 0)
				return 0;
			else
				return 1;
		}
		else if (bit == 6)
		{
			other = 2;
			value = curr.data[character] & other;	
			if (value > 0)
				return 0;
			else
				return 1;
		}
		else if (bit == 7)
		{
			other = 1;
			value = curr.data[character] & other;	
			if (value > 0)
				return 0;
			else
				return 1;
		}
	}
	else if (mapBlock == 1)	//look in the second map block
	{
		fseek(disk, 512, SEEK_SET);	//seek to beginning of second disk block
		fread(&curr, sizeof(cs1550_disk_block), 1, disk);	//read in the disk block
		fclose(disk);
		if (bit == 0)
		{
			other = 128;
			value = curr.data[character] & other;	
			if (value > 0)
				return 0;
			else
				return 1;
		}
		else if (bit == 1)
		{
			other = 64;
			value = curr.data[character] & other;	
			if (value > 0)
				return 0;
			else
				return 1;
		}
		else if (bit == 2)
		{
			other = 32;
			value = curr.data[character] & other;	
			if (value > 0)
				return 0;
			else
				return 1;
		}
		else if (bit == 3)
		{
			other = 16;
			value = curr.data[character] & other;	
			if (value > 0)
				return 0;
			else
				return 1;
		}
		else if (bit == 4)
		{
			other = 8;
			value = curr.data[character] & other;	
			if (value > 0)
				return 0;
			else
				return 1;
		}
		else if (bit == 5)
		{
			other = 4;
			value = curr.data[character] & other;	
			if (value > 0)
				return 0;
			else
				return 1;
		}
		else if (bit == 6)
		{
			other = 2;
			value = curr.data[character] & other;	
			if (value > 0)
				return 0;
			else
				return 1;
		}
		else if (bit == 7)
		{
			other = 1;
			value = curr.data[character] & other;	
			if (value > 0)
				return 0;
			else
				return 1;
		}
	}
	else	//look in the third and final map block
	{
		fseek(disk, 1024, SEEK_SET);	//seek to beginning of third disk block
		fread(&curr, sizeof(cs1550_disk_block), 1, disk);	//read in the disk block
		fclose(disk);
		if (bit == 0)
		{
			other = 128;
			value = curr.data[character] & other;	
			if (value > 0)
				return 0;
			else
				return 1;
		}
		else if (bit == 1)
		{
			other = 64;
			value = curr.data[character] & other;	
			if (value > 0)
				return 0;
			else
				return 1;
		}
		else if (bit == 2)
		{
			other = 32;
			value = curr.data[character] & other;	
			if (value > 0)
				return 0;
			else
				return 1;
		}
		else if (bit == 3)
		{
			other = 16;
			value = curr.data[character] & other;	
			if (value > 0)
				return 0;
			else
				return 1;
		}
		else if (bit == 4)
		{
			other = 8;
			value = curr.data[character] & other;	
			if (value > 0)
				return 0;
			else
				return 1;
		}
		else if (bit == 5)
		{
			other = 4;
			value = curr.data[character] & other;	
			if (value > 0)
				return 0;
			else
				return 1;
		}
		else if (bit == 6)
		{
			other = 2;
			value = curr.data[character] & other;	
			if (value > 0)
				return 0;
			else
				return 1;
		}
		else if (bit == 7)
		{
			other = 1;
			value = curr.data[character] & other;	
			if (value > 0)
				return 0;
			else
				return 1;
		}
	}
	return 0;
}

int firstFreeBlock()
{
	int i = 3;	//start after bitmap blocks
	while (i < 10240)	//check all blocks, in order
	{
		if (isBlockFree(i) == 1)	//if the block is free
		{
			return i;			//return that block's number
		}
		i += 1;
	}
	return -1;	//no blocks were free
}

int findContiguousFree(int wanted)
{
	int i = 3;		//start after bitmap blocks
	int streak = 0;
	while (i < 10240)	//check each block, in order
	{
		if (isBlockFree(i) == 1)	//if it's free
			streak += 1;			//increment the streak
		else
			streak = 0;	//end the streak
		if (streak == wanted)	//if the streak is sufficient
			return (i - wanted + 1);	//return the streak's start block
		i += 1;
	}
	return -1;	//no such streak could be found
}

int printMap()
{
	int i = 3;
	while (i < 20)
	{
		if (isBlockFree(i))
			printf("Block %d is free\n", i);
		i += 1;
	}
	printf("\n\n");
	return 0;
}



//Don't change this.
int main(int argc, char *argv[])
{
		/*
        setBlockBit(3);
		setBlockBit(4);
		setBlockBit(5);
		setBlockBit(87);
		clearBlockBit(4);
		clearBlockBit(3);
		clearBlockBit(5);
		clearBlockBit(87);
		if (isBlockFree(3))
			printf("Block 3 is free\n");
		if (isBlockFree(4))
			printf("Block 4 is free\n");
		if (isBlockFree(5))
			printf("Block 5 is free\n");
		int first = firstFreeBlock();
		printf("The first free block is: %d\n", first);
		int three = findContiguousFree(3);
		printf("The first three contiguous start at: %d\n\n", three);
		printf("Blocks 3-112:\n");
		*/
		printMap();
		
		/*
		unsigned char first = 128;
		unsigned char second = 64;
		unsigned char third = 32;
		unsigned char fourth = 16;
		unsigned char fifth = 8;
		unsigned char sixth = 4;
		unsigned char seventh = 2;
		unsigned char eighth = 1;
		char derp = 255;
		unsigned char result1 = first | derp;
		unsigned char result2 = second | derp;
		unsigned char result3 = third | derp;
		unsigned char result4 = fourth | derp;
		unsigned char result5 = fifth | derp;
		unsigned char result6 = sixth | derp;
		unsigned char result7 = seventh | derp;
		unsigned char result8 = eighth | derp;
		printf("Result 1: %d\n", result1);
		printf("Result 2: %d\n", result2);
		printf("Result 3: %d\n", result3);
		printf("Result 4: %d\n", result4);
		printf("Result 5: %d\n", result5);
		printf("Result 6: %d\n", result6);
		printf("Result 7: %d\n", result7);
		printf("Result 8: %d\n", result8);
		int test = 1 + derp;
		printf("Test: %d\n", test);
		*/
}
