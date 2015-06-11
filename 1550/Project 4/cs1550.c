/*
        FUSE: Filesystem in Userspace
        Copyright (C) 2001-2007  Miklos Szeredi <miklos@szeredi.hu>

        This program can be distributed under the terms of the GNU GPL.
        See the file COPYING.

*/

//Adam Jaworski
//CS 1550
//Project 4
//December 6, 2014

//NOTES:
//-this tends to compile with warnings about my bitmap functions and unused variables; I've just been ignoring those
//-it gets a little confused when files don't have extensions
//-it seems to work fine when files are kept small, but it has trouble when files span multiple blocks
//--I imagine there's some bug in my implementation of read and/or write
//-file functions start on line 879

#define FUSE_USE_VERSION 26

#include <fuse.h>
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
	fclose(disk);	//close the file
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
	fclose(disk);	//close the file
	return 0;
}


int isBlockFree(int block)	//whether or not a block is free
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
		fclose(disk);	//close the file
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

int firstFreeBlock(void)	//determines the first free block
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

int findContiguousFree(int wanted)	//finds the first streak of free blocks of the specified size
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

/*
 * Called whenever the system wants to know the file attributes, including
 * simply whether the file exists or not.
 *
 * man -s 2 stat will show the fields of a stat structure
 */
static int cs1550_getattr(const char *path, struct stat *stbuf)
{
        int res = 0;
		char directory[MAX_FILENAME + 1], filename[MAX_FILENAME + 1], extension[MAX_EXTENSION + 1];
        memset(stbuf, 0, sizeof(struct stat));
		sscanf(path, "/%[^/]/%[^.].%s", directory, filename, extension);
        //is path the root dir?
        if (strcmp(path, "/") == 0) 
		{      
			stbuf->st_mode = S_IFDIR | 0755;
            stbuf->st_nlink = 2;
			return 0;
        } 
		else //if the path is not the root dir
		{
			FILE *dfile;
			dfile = fopen(".directories", "ab+");	//should create the file if it's not already there
			cs1550_directory_entry curr;
			int validDirectory = 0;
			if (dfile == NULL)	//probably shouldn't ever happen now
			{
				return -ENOENT;
			}
			while (fread (&curr, sizeof(cs1550_directory_entry), 1, dfile))	//should read one directory entry from .directories and load it into curr
			{
				if (strcmp(directory, curr.dname) == 0)	//if path's directory is the same as the directory entry's directory name
				{
					validDirectory = 1;
					break;		//leave curr as the appropriate directory entry
				}
			}
			if (validDirectory == 0)	//no directories matched the one in the path
			{
				fclose(dfile);
				res = -ENOENT;
				return res;	//if we've read all directories without a match, then the path must be invalid
			}
			if (strlen(path) == (strlen(directory) + 1))	//if there is no file name, then it's just a subdirectory
			{
				stbuf->st_mode = S_IFDIR | 0755;	//these values are appropriate for a subdirectory
                stbuf->st_nlink = 2;
                res = 0; //no error
				fclose(dfile);
				return res;
			}
			int i = 0;
			while (i < curr.nFiles)	//we'll look at all files in directory curr
			{
				if (strcmp(curr.files[i].fname, filename) == 0 && strcmp(curr.files[i].fext, extension) == 0)	//if the file names and extensions match
				{
					stbuf->st_mode = S_IFREG | 0666;	//these values are appropriate for a file
					stbuf->st_nlink = 1; //file links
					stbuf->st_size = curr.files[i].fsize; //file size
					res = 0; // no error
					fclose(dfile);
					return res;
				}
				i += 1;	//to consider the next file
			}

                //Else return that path doesn't exist
            res = -ENOENT;
			fclose(dfile);	//if no files in curr match the file specified in path
        }
        return res;
}

/*
 * Called whenever the contents of a directory are desired. Could be from an 'ls'
 * or could even be when a user hits TAB to do autocompletion
 */
static int cs1550_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
                         off_t offset, struct fuse_file_info *fi)
{
        //Since we're building with -Wall (all warnings reported) we need
        //to "use" every parameter, so let's just cast them to void to
        //satisfy the compiler
        (void) offset;
        (void) fi;
		int isRoot = 0;
		char directory[MAX_FILENAME + 1], filename[MAX_FILENAME + 1], extension[MAX_EXTENSION + 1];
		sscanf(path, "/%[^/]/%[^.].%s", directory, filename, extension);
		if (strcmp(path, "/") == 0)	//if it's the root directory
		{
			isRoot = 1;	//remember that
		}
		int validDirectory = 0;
		FILE *dfile;
		dfile = fopen(".directories", "ab+");	//should create the file if it's not already there
		cs1550_directory_entry curr;
		if (dfile == NULL && isRoot == 0)	//shouldn't ever happen now
		{
			return -ENOENT;
		}
		while (fread (&curr, sizeof(cs1550_directory_entry), 1, dfile))	//should read one directory entry from .directories and load it into curr
		{
			if (isRoot == 1)	//if root, we'll be adding the subdirectories
			{
				filler(buf, curr.dname, NULL, 0);	//add the dname
			}
			else	//otherwise, we'll be looking for a subdirectory, so we can add its files
			{
				if (strcmp(directory, curr.dname) == 0)	//if path's directory is the same as the directory entry's directory name
				{
					validDirectory = 1;
					break;		//leave curr as the appropriate directory entry
				}
			}
		}
		//safe to assume that there will be no file name?
		if (validDirectory == 1)	//if the path is to a valid subdirectory
		{
			int i = 0;
			while (i < curr.nFiles)	//we'll add the files in directory curr
			{
				char newpath[14];
				strcpy(newpath, curr.files[i].fname);	//copy file name into newpath
				if (strlen(curr.files[i].fext) > 0)
				{
					strcat(newpath, ".");
					strcat(newpath, curr.files[i].fext);	//append file extension to newpath
				}
				filler(buf, newpath, NULL, 0);		//add newpath to the list
				i += 1;	//to consider the next file
			}
		}


        //the filler function allows us to add entries to the listing
        //read the fuse.h file for a description (in the ../include dir)
        filler(buf, ".", NULL, 0);	//I think we just always add these
        filler(buf, "..", NULL, 0);

		fclose(dfile);
		if (isRoot == 0 && validDirectory == 0)	//if neither the root directory nor a subdirectory
		{
			return -ENOENT;	//return an error
		}
        return 0;	//otherwise return success
}

/*
 * Creates a directory. We can ignore mode since we're not dealing with
 * permissions, as long as getattr returns appropriate ones for us.
 */
static int cs1550_mkdir(const char *path, mode_t mode)
{
        (void) mode;
		char directory[MAX_FILENAME + 1], filename[MAX_FILENAME + 1], extension[MAX_EXTENSION + 1];
		sscanf(path, "/%[^/]/%[^.].%s", directory, filename, extension);	//assuming the path still has all these components
		if (strlen(directory) > MAX_FILENAME)	//if the directory name is longer than the maximum allowed name length
		{
			return -ENAMETOOLONG;	//return this aptly-named error
		}
		if (strlen(path) > (strlen(directory) + 1))	//if there's more to the path than the '/' + the directory name
		{
			return -EPERM;	//the directory is not under the root
		}
		int validDirectory = 0;
		FILE *dfile;
		dfile = fopen(".directories", "ab+");
		cs1550_directory_entry curr;
		if (dfile == NULL)	//should never happen now
		{
			//nothing
		}
		else	//if there is a directories file
		{
			while (fread (&curr, sizeof(cs1550_directory_entry), 1, dfile))	//should read one directory entry from .directories and load it into curr
			{

				if (strcmp(directory, curr.dname) == 0)	//if path's directory is the same as the directory entry's directory name
				{
					validDirectory = 1;
					break;		
				}
				
			}
			fclose(dfile);
		}
		if (validDirectory == 1)	//we found the directory already in there
		{
			return -EEXIST;
		}
		//if we've reached this point, we should be in the clear
		cs1550_directory_entry newdir;
		strcpy(newdir.dname, directory);	//presumably it should be named as the directory part of the path
		newdir.nFiles = 0;			//we're just creating it now, so it shouldn't have any files in it
		//and I suppose we can leave its 'files' array as all null
		FILE *dfile2;
		dfile2 = fopen(".directories", "ab");	//reopen the .directories file, this time for appending (should create if not already there)
		fwrite(&newdir, sizeof(cs1550_directory_entry), 1, dfile2);	//write the new directory entry to the .directories file
		fclose(dfile2);
        return 0;	//return success
}

/*
 * Removes a directory.
 */
static int cs1550_rmdir(const char *path)
{
        (void) path;
    return 0;
}

/*
 * Does the actual creation of a file. Mode and dev can be ignored.
 *
 */
static int cs1550_mknod(const char *path, mode_t mode, dev_t dev)
{
        (void) mode;
        (void) dev;	//consider them ignored
		//FILE *efile;
		//efile = fopen(".errors", "w");
		//fprintf(efile, "Just opened the error file.");
		//fclose(efile);
		char directory[MAX_FILENAME + 1], filename[MAX_FILENAME + 1], extension[MAX_EXTENSION + 1];
		sscanf(path, "/%[^/]/%[^.].%s", directory, filename, extension);
		if (strlen(filename) > MAX_FILENAME || strlen(extension) > MAX_EXTENSION)	//if either part of the name is beyond the limit
		{
			return -ENAMETOOLONG;	//return the only error that makes sense
		}
		if ((strlen(directory) + 1) == strlen(path))	//seems to be the case when path is "/file.txt", for instance
		{
			return -EPERM;	//trying to be created in the root dir
		}
		FILE *dfile;		//the directories file
		dfile = fopen(".directories", "ab+");	//should create the file if it's not already there
		cs1550_directory_entry curr;
		int found = 0;
		while (fread (&curr, sizeof(cs1550_directory_entry), 1, dfile))	//should read one directory entry from .directories and load it into curr
		{
			if (strcmp(directory, curr.dname) == 0)	//if path's directory is the same as the directory entry's directory name
			{
				found = 1;
				break;		//this is the directory entry we want
			}
		}
		int f = curr.nFiles;
		int i = 0;
		while (i < f)	//check all files in the directory
		{
			if (strcmp(curr.files[i].fname, filename) == 0 && strcmp(curr.files[i].fext, extension) == 0)	//if names and extensions match
			{
				fclose(dfile);
				return -EEXIST;		//the file already exists
			}
			i += 1;
		}
		fclose(dfile);
		//if we've reached this point, we should be in the clear
		int block = firstFreeBlock();	//get the first free block, according to the bitmap
		setBlockBit(block);		//set that bit in the bitmap, because we're putting the file there
		FILE *dfile2;
		dfile2 = fopen(".directories", "rb+");	//open directories, for reading and writing
		cs1550_directory_entry curr2;
		while (fread (&curr2, sizeof(cs1550_directory_entry), 1, dfile2))	//should read one directory entry from .directories and load it into curr
		{
			if (strcmp(directory, curr2.dname) == 0)	//if path's directory is the same as the directory entry's directory name
			{
				found = 1;
				break;		//this is the directory entry we want
			}
		}
		fseek(dfile2, sizeof(cs1550_directory_entry) * -1, SEEK_CUR);	//seek back to beginning of the entry, so we can write curr back over it later
		f = curr2.nFiles;	//in theory should be the same as before, but whatever
		//we'll just put it in the files array with the current number of files as the index
		strcpy(curr2.files[f].fname, filename);	//copy in the filename
		strcpy(curr2.files[f].fext, extension);	//copy in the extension
		curr2.files[f].fsize = 0;		//I guess it should be 0 to start
		curr2.files[f].nStartBlock = block;	//the block we found for it before
		curr2.nFiles += 1;			//actually increment the number of files in the directory
		fwrite(&curr2, sizeof(cs1550_directory_entry), 1, dfile2);	//write the entry back in
		fclose(dfile2);	//close the file
		//FILE *efile2;
		//efile2 = fopen(".errors2", "w");
		//fprintf(efile2, "Returning 0 for success");
		//fclose(efile2);
        return 0;		//return success
}

/*
 * Deletes a file
 */
static int cs1550_unlink(const char *path)
{
	char directory[MAX_FILENAME + 1], filename[MAX_FILENAME + 1], extension[MAX_EXTENSION + 1];
	sscanf(path, "/%[^/]/%[^.].%s", directory, filename, extension);
	if (strlen(path) == (strlen(directory) + 1))	//if the full path is only one more than the directory
	{
		return -EISDIR;		//the path is a directory
	}
	FILE *dfile;
	dfile = fopen(".directories", "rb+");
	cs1550_directory_entry curr;
	int found = 0;
	while (fread (&curr, sizeof(cs1550_directory_entry), 1, dfile))	//should read one directory entry from .directories and load it into curr
	{
		if (strcmp(directory, curr.dname) == 0)	//if path's directory is the same as the directory entry's directory name
		{
			found = 1;
			break;		//this is the directory entry we want
		}
	}
	fseek(dfile, sizeof(cs1550_directory_entry) * -1, SEEK_CUR);	//to rewrite over the directory entry later
	int f = curr.nFiles;
	int i = 0;
	int found2 = 0;
	while (i < f)	//go through the directory's files
	{
		if (strcmp(curr.files[i].fname, filename) == 0 && strcmp(curr.files[i].fext, extension) == 0)	//if names and extensions match
		{
			found2 = 1;
			break;	//we've found the file, and therefore its index
		}
		i += 1;
	}
	if (found == 0 || found2 == 0)	//if we couldn't find the directory and/or file
	{
		fclose(dfile);
		return -ENOENT;	//return that the file does not exist
	}
	int fileSize = curr.files[i].fsize;	//the size of the file we're deleting
	int startBlock = curr.files[i].nStartBlock;	//the block on disk where the file begins
	int numBlocks = ((fileSize - 1) / sizeof(cs1550_disk_block)) + 1;	//how many blocks the file occupies
	int clearCount = 0;
	while (clearCount < numBlocks)	//for each of the file's blocks
	{
		clearBlockBit(startBlock + clearCount);	//clear its corresponding bit in the bitmap
		clearCount += 1;
	}
	curr.nFiles -= 1;		//because we're deleting one of its files
	cs1550_directory_entry replacer;	//this is what we'll write over the original directory entry
	replacer.nFiles = curr.nFiles;	//same name and number of files
	strcpy(replacer.dname, curr.dname);
	int replacerIndex = 0;
	int originalIndex = 0;
	while (originalIndex < f)	//the files in the old entry
	{
		if (originalIndex != i)	//if it's not the index of the file we're deleting
		{
			replacer.files[replacerIndex].fsize = curr.files[originalIndex].fsize;	//copy the file's information
			replacer.files[replacerIndex].nStartBlock = curr.files[originalIndex].nStartBlock;
			strcpy(replacer.files[replacerIndex].fname, curr.files[originalIndex].fname);
			strcpy(replacer.files[replacerIndex].fext, curr.files[originalIndex].fext);
			replacerIndex += 1;	//increment replacer index every time except for the deleted file
		}
		originalIndex += 1;	//always increment the original index
	}
	fwrite(&replacer, sizeof(cs1550_directory_entry), 1, dfile);	//write in the new directory entry, without the deleted file
	fclose(dfile);	//close the directories file
    return 0;	//return 0 for success
}

/*
 * Read size bytes from file into buf starting from offset
 *
 */
static int cs1550_read(const char *path, char *buf, size_t size, off_t offset,
                          struct fuse_file_info *fi)
{
        (void) fi;

        //check to make sure path exists
		char directory[MAX_FILENAME + 1], filename[MAX_FILENAME + 1], extension[MAX_EXTENSION + 1];
		sscanf(path, "/%[^/]/%[^.].%s", directory, filename, extension);
		if (strlen(path) == (strlen(directory) + 1))	//if the full path is only one more than the directory
		{
			return -EISDIR;		//the path is a directory
		}
		//check that size is > 0
		if (size == 0)			//if it wants us to read 0 bytes
			return size;		//then I suppose we've successfully read 0 bytes
		FILE *dfile;
		dfile = fopen(".directories", "rb+");	//open the files
		FILE *disk;
		disk = fopen(".disk", "rb+");
		cs1550_directory_entry curr;
		int found = 0;
		while (fread (&curr, sizeof(cs1550_directory_entry), 1, dfile))	//should read one directory entry from .directories and load it into curr
		{
			if (strcmp(directory, curr.dname) == 0)	//if path's directory is the same as the directory entry's directory name
			{
				found = 1;
				break;		//this is the directory entry we want
			}
		}
		int f = curr.nFiles;
		int i = 0;
		while (i < f)	//go through the directory's files
		{
			if (strcmp(curr.files[i].fname, filename) == 0 && strcmp(curr.files[i].fext, extension) == 0)	//if names and extensions match
			{
				break;	//we've found the file, and therefore its index
			}
			i += 1;
		}
		int fileSize = curr.files[i].fsize;
		int startBlock = curr.files[i].nStartBlock;
		if (offset > fileSize)	//if the offset is beyond the file size
		{
			fclose(dfile);
			fclose(disk);
			return -EFBIG;	//return this error
		}
		int fileBlocks = ((fileSize - 1)/sizeof(cs1550_disk_block)) + 1;	//how many blocks the file is occupying
		fseek(disk, sizeof(cs1550_disk_block) * startBlock, SEEK_SET);	//seek to the file's start block on disk
		cs1550_disk_block readBlock;
		if (fileBlocks == 1)	//if there is just one file block
		{
			fread(&readBlock, sizeof(cs1550_disk_block), 1, disk);	//read the block
			memcpy(buf, readBlock.data + offset, size);		//copy in the data
		}
		else	//file spans multiple blocks
		{
			int offsetBlock = (offset - 1) / sizeof(cs1550_disk_block);	//which of the file's blocks contains the offset
			int count = 0;
			int remaining = size;
			while (count < offsetBlock)	//while the block is not the one with the offset
			{
				fread(&readBlock, sizeof(cs1550_disk_block), 1, disk);	//read through the block
				count += 1;
			}
			fread(&readBlock, sizeof(cs1550_disk_block), 1, disk);	//read the offset block
			memcpy(buf, readBlock.data + offset, sizeof(cs1550_disk_block) - offset);	//read the block from offset to the end of the block
			count += 1;
			while (count < fileBlocks)
			{
				fread(&readBlock, sizeof(cs1550_disk_block), 1, disk);
				strcat(buf, readBlock.data);	//append the block's data onto buf
				count += 1;
			}
			
		}
		fclose(dfile);	//close the files
		fclose(disk);
        return size;	//return how much was read
}

/*
 * Write size bytes from buf into file starting from offset
 *
 */
static int cs1550_write(const char *path, const char *buf, size_t size,
                          off_t offset, struct fuse_file_info *fi)
{
        (void) fi;
        //check to make sure path exists
		char directory[MAX_FILENAME + 1], filename[MAX_FILENAME + 1], extension[MAX_EXTENSION + 1];
		sscanf(path, "/%[^/]/%[^.].%s", directory, filename, extension);
		FILE *dfile;		//the directories file
		dfile = fopen(".directories", "rb+");	
		cs1550_directory_entry curr;
		int found = 0;
		int found2 = 0;
		int entryNumber = 0;
		while (fread (&curr, sizeof(cs1550_directory_entry), 1, dfile))	//should read one directory entry from .directories and load it into curr
		{
			if (strcmp(directory, curr.dname) == 0)	//if path's directory is the same as the directory entry's directory name
			{
				found = 1;
				break;		//this is the directory entry we want
			}
			entryNumber += 1;	//so we can seek to this entry again later
		}
		fseek(dfile, sizeof(cs1550_directory_entry) * -1, SEEK_CUR);	//seek back to beginning of the entry, so we can write curr back over it later
		int f = curr.nFiles;
		int i = 0;
		while (i < f)	//check all files in the directory
		{
			if (strcmp(curr.files[i].fname, filename) == 0 && strcmp(curr.files[i].fext, extension) == 0)	//if names and extensions match
			{
				found2 = 1;
				break;	//break, so i is the index of the file we're writing to
			}
			i += 1;
		}
        //check that size is > 0
		if (!(size > 0))	//if we're not actually writing anything
		{
			fclose(dfile);
			return 0;		//I guess just return 0
		}
        //check that offset is <= to the file size
		int theFileSize = curr.files[i].fsize;
		if (offset > theFileSize)	//if the offset is beyond the size of the file
		{
			fclose(dfile);
			return -EFBIG;		//return this error
		}
        //write data
        //set size (should be same as input) and return, or error
		int startBlock = curr.files[i].nStartBlock;	//the file's start block
		FILE *disk;		//the disk file
		disk = fopen(".disk", "rb+");
		int location = startBlock * sizeof(cs1550_disk_block);	//the index of the block on disk (512 bytes per block)
		fseek(disk, location, SEEK_SET);	//seek there (from beginning)
		cs1550_disk_block writeBlock;
		int numBlocks = ((theFileSize - 1) / sizeof(cs1550_disk_block)) + 1;	//how many blocks the file is currently occupying
		int bytesIntoLastBlock = theFileSize % sizeof(cs1550_disk_block);	//how many bytes of the last block are being used
		int availableInLastBlock = sizeof(cs1550_disk_block) - bytesIntoLastBlock;	//how many bytes of the last block are NOT being used
		int mustAppend = 0;
		int newSize = theFileSize;
		if ((offset + size) > theFileSize)	//if the file will be larger afterward
			newSize = offset + size;
		int difference = newSize - theFileSize;
		int finalBlocksNeeded = ((newSize - 1) / sizeof(cs1550_disk_block)) + 1;	//how many blocks the file will be when done writing
		int needMoreBlocks = 0;
		if (difference > 0)	//if the file will be larger afterward
			mustAppend = 1;		//it's an append
		if (difference > availableInLastBlock)	//if the free bytes in the current last block do not cover the difference
			needMoreBlocks = 1;		//we'll need at least one more block
		int deficit = difference - availableInLastBlock;	//how many more bytes are needed from new blocks
		int moreBlocksNeeded = (deficit / sizeof(cs1550_disk_block)) + 1;	//how many more blocks will be needed

		if (needMoreBlocks == 0)	//let's start with the much simpler case of not needing a new block
		{
			fread(&writeBlock, sizeof(cs1550_disk_block), 1, disk);	//read in the block
			fseek(disk, sizeof(cs1550_disk_block) * -1, SEEK_CUR);	//to write back over it later
			memcpy(writeBlock.data + offset, buf, size);	//copy size bytes from buf into block's data at offset
			fwrite(&writeBlock, sizeof(cs1550_disk_block), 1, disk);	//write it back over
			curr.files[i].fsize += difference;	//because we'll have written 'difference' more bytes
			fwrite(&curr, sizeof(cs1550_directory_entry), 1, dfile);	//write the directory entry back in
			fclose(dfile);
			fclose(disk);
			return size;	//the number of bytes we just wrote
		}
		
		else	//more blocks were needed
		{
			int clearC = 0;
			while (clearC < numBlocks)	//for all blocks in the old file
			{
				clearBlockBit(startBlock + clearC);	//clear their corresponding bit in the bitmap
				clearC += 1;	//so that they can be considered by the find contiguous function
			}
			int spot = findContiguousFree(finalBlocksNeeded);	//finds the first contiguous streak of the necessary number of free blocks
			int moving = 0;
			if (spot != startBlock)	//if the current spot won't work
				moving = 1;		//we'll have to move something
				
			if (moving == 0)	//start with the simpler case of no moving
			{
				int setCount = 0;
				while (setCount < finalBlocksNeeded)	//for all blocks in the final file
				{
					setBlockBit(startBlock + setCount);	//set their corresponding bit in the bitmap
					setCount += 1;
				}
				int offsetBlock = (offset - 1) / 512;		//which of the file's blocks contains the offset
				int count = 0;	//how many blocks in we are
				while (count < offsetBlock)	//read through blocks until we've reached the offset's block
				{
					fread(&writeBlock, sizeof(cs1550_disk_block), 1, disk);	//because the file's blocks will be contiguous
					count += 1;
				}
				fread(&writeBlock, sizeof(cs1550_disk_block), 1, disk);	//read in the block containing the offset
				fseek(disk, sizeof(cs1550_disk_block) * -1, SEEK_CUR);	//to write back over it later
				//figure out which block contains the offset
				//seek there
				//only write from there (leave everything before offset untouched)
				memcpy(writeBlock.data + offset, buf, sizeof(cs1550_disk_block) - offset);	//fill in the rest of the offset block from buf
				fwrite(&writeBlock, sizeof(cs1550_disk_block), 1, disk);	//rewrite the offset block
				count += 1;	//just wrote the offset block
				int count2 = 0;	//number of blocks after the offset block we've written
				while (count < finalBlocksNeeded)	//writing whole blocks
				{
					fread(&writeBlock, sizeof(cs1550_disk_block), 1, disk);	//read in the block
					fseek(disk, sizeof(cs1550_disk_block) * -1, SEEK_CUR);	//to write back over it later
					memcpy(writeBlock.data, buf + offset + (sizeof(cs1550_disk_block)*count2), sizeof(cs1550_disk_block));	//write a block's worth of data
																															//from where we left off in buf
					fwrite(&writeBlock, sizeof(cs1550_disk_block), 1, disk);	//rewrite the block
					count2 += 1;
					count += 1;
				}
				fread(&writeBlock, sizeof(cs1550_disk_block), 1, disk);	//read in the last block of the file
				fseek(disk, sizeof(cs1550_disk_block) * -1, SEEK_CUR);	//to write back over it later
				memcpy(writeBlock.data, buf + offset + (sizeof(cs1550_disk_block)*count2), bytesIntoLastBlock);	//write in what was left in buf
				fwrite(&writeBlock, sizeof(cs1550_disk_block), 1, disk);	//write it back in
				curr.files[i].fsize += difference;	//because we'll have written 'difference' more bytes
				fwrite(&curr, sizeof(cs1550_directory_entry), 1, dfile);	//write the directory entry back in
				fclose(dfile);
				fclose(disk);
				return size;
			}
			
			else	//we have to move one of the files
			{
				fseek(dfile, 0, SEEK_SET);	//return to beginning of directory file
				int bStartBlock = startBlock + numBlocks;	//the start block of the file in the way
				cs1550_directory_entry bDirectory;
				int bEntryNumber = 0;
				int bFileIndex = -1;
				while (fread (&bDirectory, sizeof(cs1550_directory_entry), 1, dfile))	//read through directory entries
				{
					int currNum = bDirectory.nFiles;
					int currIndex = 0;
					while (currIndex < currNum)	//for all files in the directory
					{
						if (bStartBlock == bDirectory.files[currIndex].nStartBlock)	//if this is the file that's in our way
						{
							bFileIndex = currIndex;	//the index of the file in the directory entry's 'files' array
							break;
						}
						currIndex += 1;
					}
					if (bFileIndex != -1)	//means the file was in this directory
						break;	//so end the loop
					bEntryNumber += 1;
				}
				int bSize = bDirectory.files[bFileIndex].fsize;	//how big the in the way file is
				
				if (newSize >= bSize)	//move the file we're writing to if it's bigger
				{
					curr.files[i].fsize += difference;	//because we'll have written 'difference' more bytes
					curr.files[i].nStartBlock = spot;	//update its start block to where we're moving it
					fseek(dfile, sizeof(cs1550_directory_entry) * entryNumber, SEEK_SET);	//seek back to its directory entry
					fwrite(&curr, sizeof(cs1550_directory_entry), 1, dfile);	//write the directory entry back in
					fseek(disk, sizeof(cs1550_disk_block) * spot, SEEK_SET);	//seek to where we're moving the file
					int setCount = 0;
					while (setCount < finalBlocksNeeded)	//for all blocks in the final file
					{
						setBlockBit(spot + setCount);	//set their corresponding bit in the bitmap
						setCount += 1;
					}
					int clearCount = 0;
					while (clearCount < numBlocks)	//for all blocks in the old file
					{
						clearBlockBit(startBlock + clearCount);	//clear their corresponding bit in the bitmap
						clearCount += 1;
					}
					int offsetBlock = (offset - 1) / 512;		//which of the file's blocks contains the offset
					int count = 0;	//how many blocks in we are
					while (count < offsetBlock)	//read through blocks until we've reached the offset's block
					{
						fread(&writeBlock, sizeof(cs1550_disk_block), 1, disk);	//because the file's blocks will be contiguous
						count += 1;
					}
					fread(&writeBlock, sizeof(cs1550_disk_block), 1, disk);	//read in the block containing the offset
					fseek(disk, sizeof(cs1550_disk_block) * -1, SEEK_CUR);	//to write back over it later
					//figure out which block contains the offset
					//seek there
					//only write from there (leave everything before offset untouched)
					memcpy(writeBlock.data + offset, buf, sizeof(cs1550_disk_block) - offset);	//fill in the rest of the offset block from buf
					fwrite(&writeBlock, sizeof(cs1550_disk_block), 1, disk);	//rewrite the offset block
					count += 1;	//just wrote the offset block
					int count2 = 0;	//number of blocks after the offset block we've written
					while (count < finalBlocksNeeded)	//writing whole blocks
					{
						fread(&writeBlock, sizeof(cs1550_disk_block), 1, disk);	//read in the block
						fseek(disk, sizeof(cs1550_disk_block) * -1, SEEK_CUR);	//to write back over it later
						memcpy(writeBlock.data, buf + offset + (sizeof(cs1550_disk_block)*count2), sizeof(cs1550_disk_block));	//write a block's worth of data
																															//from where we left off in buf
						fwrite(&writeBlock, sizeof(cs1550_disk_block), 1, disk);	//rewrite the block
						count2 += 1;
						count += 1;
					}
					fread(&writeBlock, sizeof(cs1550_disk_block), 1, disk);	//read in the last block of the file
					fseek(disk, sizeof(cs1550_disk_block) * -1, SEEK_CUR);	//to write back over it later
					memcpy(writeBlock.data, buf + offset + (sizeof(cs1550_disk_block)*count2), bytesIntoLastBlock);	//write in what was left in buf
					fwrite(&writeBlock, sizeof(cs1550_disk_block), 1, disk);	//write it back in
					fclose(dfile);
					fclose(disk);
					return size;
				}
				
				else	//move the file in the way
				{
					int setC = 0;
					while (setC < numBlocks)	//for all blocks in the old file
					{
						setBlockBit(startBlock + setC);	//set their corresponding bit in the bitmap
						setC += 1;	//so that they can't be considered by the find contiguous function
					}
					int blocksForB = ((bSize - 1) / sizeof(cs1550_disk_block)) + 1;	//the number of blocks file B is occupying
					int spot2 = findContiguousFree(blocksForB);	//find a space to move the file to
					fseek(disk, sizeof(cs1550_disk_block) * spot2, SEEK_SET);	//seek to that block on the disk
					int setCount = 0;
					while (setCount < blocksForB)	//for all blocks in the b file
					{
						setBlockBit(spot2 + setCount);	//set their corresponding bit in the bitmap
						setCount += 1;
					}
					int clearCount = 0;
					while (clearCount < blocksForB)	//for all blocks in the b file
					{
						clearBlockBit(bStartBlock + clearCount);	//set their corresponding bit in the bitmap
						clearCount += 1;
					}
					fseek(dfile, sizeof(cs1550_directory_entry) * bEntryNumber, SEEK_SET);	//seek to file b's directory entry
					bDirectory.files[bFileIndex].nStartBlock = spot2;	//update its start block
					fwrite(&bDirectory, sizeof(cs1550_directory_entry), 1, dfile);	//overwrite its directory entry
					fseek(dfile, sizeof(cs1550_directory_entry) * entryNumber, SEEK_SET);	//seek to file a's directory entry
					int bMoveCount = 0;
					while (bMoveCount < blocksForB)	//for all of B's blocks
					{
						fseek(disk, sizeof(cs1550_disk_block) * (bStartBlock + bMoveCount), SEEK_SET);	//seek to one of B's old blocks
						fread(&writeBlock, sizeof(cs1550_disk_block), 1, disk);	//read it in
						fseek(disk, sizeof(cs1550_disk_block) * (spot2 + bMoveCount), SEEK_SET);	//seek to B's corresponding new block
						fwrite(&writeBlock, sizeof(cs1550_disk_block), 1, disk);	//write it in its new location
						bMoveCount += 1;
					}
					
					fseek(disk, sizeof(cs1550_disk_block) * startBlock, SEEK_SET);	//seek back to the start of file A
					curr.files[i].fsize += difference;	//update the file's size
					fwrite(&curr, sizeof(cs1550_directory_entry), 1, dfile);	//write the directory entry back in
					int offsetBlock = (offset - 1) / 512;		//which of the file's blocks contains the offset
					int count = 0;	//how many blocks in we are
					while (count < offsetBlock)	//read through blocks until we've reached the offset's block
					{
						fread(&writeBlock, sizeof(cs1550_disk_block), 1, disk);	//because the file's blocks will be contiguous
						count += 1;
					}
					fread(&writeBlock, sizeof(cs1550_disk_block), 1, disk);	//read in the block containing the offset
					fseek(disk, sizeof(cs1550_disk_block) * -1, SEEK_CUR);	//to write back over it later
					//figure out which block contains the offset
					//seek there
					//only write from there (leave everything before offset untouched)
					memcpy(writeBlock.data + offset, buf, sizeof(cs1550_disk_block) - offset);	//fill in the rest of the offset block from buf
					fwrite(&writeBlock, sizeof(cs1550_disk_block), 1, disk);	//rewrite the offset block
					count += 1;	//just wrote the offset block
					int count2 = 0;	//number of blocks after the offset block we've written
					while (count < finalBlocksNeeded)	//writing whole blocks
					{
						fread(&writeBlock, sizeof(cs1550_disk_block), 1, disk);	//read in the block
						fseek(disk, sizeof(cs1550_disk_block) * -1, SEEK_CUR);	//to write back over it later
						memcpy(writeBlock.data, buf + offset + (sizeof(cs1550_disk_block)*count2), sizeof(cs1550_disk_block));	//write a block's worth of data
																															//from where we left off in buf
						fwrite(&writeBlock, sizeof(cs1550_disk_block), 1, disk);	//rewrite the block
						count2 += 1;
						count += 1;
					}
					fread(&writeBlock, sizeof(cs1550_disk_block), 1, disk);	//read in the last block of the file
					fseek(disk, sizeof(cs1550_disk_block) * -1, SEEK_CUR);	//to write back over it later
					memcpy(writeBlock.data, buf + offset + (sizeof(cs1550_disk_block)*count2), bytesIntoLastBlock);	//write in what was left in buf
					fwrite(&writeBlock, sizeof(cs1550_disk_block), 1, disk);	//write it back in
					setCount = 0;
					while (setCount < finalBlocksNeeded)	//for all blocks in the final file
					{
						setBlockBit(startBlock + setCount);	//set their corresponding bit in the bitmap
						setCount += 1;
					}
					fclose(dfile);
					fclose(disk);
					return size;
				}
			}
		}
		
		curr.files[i].fsize += difference;	//because we'll have written 'difference' more bytes
		fwrite(&curr, sizeof(cs1550_directory_entry), 1, dfile);	//write the directory entry back in
		fclose(dfile);
		fclose(disk);
        return size;
}

/******************************************************************************
 *
 *  DO NOT MODIFY ANYTHING BELOW THIS LINE
 *
 *****************************************************************************/

/*
 * truncate is called when a new file is created (with a 0 size) or when an
 * existing file is made shorter. We're not handling deleting files or
 * truncating existing ones, so all we need to do here is to initialize
 * the appropriate directory entry.
 *
 */
static int cs1550_truncate(const char *path, off_t size)
{
        (void) path;
        (void) size;

    return 0;
}


/*
 * Called when we open a file
 *
 */
static int cs1550_open(const char *path, struct fuse_file_info *fi)
{
        (void) path;
        (void) fi;
    /*
        //if we can't find the desired file, return an error
        return -ENOENT;
    */

    //It's not really necessary for this project to anything in open

    /* We're not going to worry about permissions for this project, but
           if we were and we don't have them to the file we should return an error

        return -EACCES;
    */

    return 0; //success!
}

/*
 * Called when close is called on a file descriptor, but because it might
 * have been dup'ed, this isn't a guarantee we won't ever need the file
 * again. For us, return success simply to avoid the unimplemented error
 * in the debug log.
 */
static int cs1550_flush (const char *path , struct fuse_file_info *fi)
{
        (void) path;
        (void) fi;

        return 0; //success!
}


//register our new functions as the implementations of the syscalls
static struct fuse_operations hello_oper = {
    .getattr    = cs1550_getattr,
    .readdir    = cs1550_readdir,
    .mkdir      = cs1550_mkdir,
        .rmdir = cs1550_rmdir,
    .read       = cs1550_read,
    .write      = cs1550_write,
        .mknod  = cs1550_mknod,
        .unlink = cs1550_unlink,
        .truncate = cs1550_truncate,
        .flush = cs1550_flush,
        .open   = cs1550_open,
};

//Don't change this.
int main(int argc, char *argv[])
{
        return fuse_main(argc, argv, &hello_oper, NULL);
}
