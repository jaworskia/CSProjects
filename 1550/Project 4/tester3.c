/*
    FUSE: Filesystem in Userspace
    Copyright (C) 2001-2007  Miklos Szeredi <miklos@szeredi.hu>

    This program can be distributed under the terms of the GNU GPL.
    See the file COPYING.

    gcc -Wall `pkg-config fuse --cflags --libs` hello.c -o hello
*/

#define FUSE_USE_VERSION 26

#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>

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

static const char *hello_str = "Hello World!\n";
static const char *hello_path = "/hello";

static int hello_getattr(const char *path, struct stat *stbuf)
{
    int res = 0;	//what we return... short for response?
	
	//Pro tip: apparently a 'struct stat' is a structure for 'file status'
	char directory[MAX_FILENAME + 1], filename[MAX_FILENAME + 1], extension[MAX_EXTENSION + 1];
	sscanf(path, "/%[^/]/%[^.].%s", directory, filename, extension);
    memset(stbuf, 0, sizeof(struct stat));	//all bytes of stbuf are intitialized to be 0, so I guess all zeroes
	
	FILE *dfile;
	dfile = fopen(".directories", "rb");	
	
    if (strcmp(path, "/") == 0) 	//if the specified path is the root directory
	{
        stbuf->st_mode = S_IFDIR | 0755;	//'protection'
        stbuf->st_nlink = 2;				//'number of hard links'
		return 0;
    } 
	else if (strcmp(path, hello_path) == 0) //if the specified path is the path to the 'hello' file
	{
		cs1550_directory_entry curr;
        stbuf->st_mode = S_IFREG | 0444;	//'protection'
        stbuf->st_nlink = 1;				//'number of hard links'
        stbuf->st_size = strlen(hello_str);	//'total size, in bytes'
		return 0;
    }
	else if (strcmp(path, "/herpderp.txt") == 0)
	{
		stbuf->st_mode = S_IFREG | 0444;	//'protection'
        stbuf->st_nlink = 1;				//'number of hard links'
        stbuf->st_size = 9;	//'total size, in bytes'
		return 0;
	}
	else if (strcmp(path, "/null") == 0 && dfile == NULL)
	{
		stbuf->st_mode = S_IFREG | 0444;	//'protection'
        stbuf->st_nlink = 1;				//'number of hard links'
        stbuf->st_size = 9;	//'total size, in bytes'
		return 0;
	}
	else
        return -ENOENT;		//ERROR NO ENTRY

    return 0;	//either 0 (success) or '-ENOENT'
}
static int hello_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
                         off_t offset, struct fuse_file_info *fi)	//lists the contents of a directory, loads them in buffer
{
    (void) offset;
    (void) fi;
	char directory[MAX_FILENAME + 1], filename[MAX_FILENAME + 1], extension[MAX_EXTENSION + 1];
	sscanf(path, "/%[^/]/%[^.].%s", directory, filename, extension);
	
	FILE *dfile;
	dfile = fopen(".directories", "rb");
	
	if (dfile == NULL)
	{
		filler(buf, "null", NULL, 0);
	}
	
	int isRoot = 0;
	if (strcmp(path, "/") == 0)	//if it's the root directory
	{
		isRoot = 1;	//remember that
	}
    if (isRoot == 0)	//if not the root directory (testmount, in our example)
        return -ENOENT;	//ERROR NO ENTRY

    filler(buf, ".", NULL, 0);	//I guess adds the current directory as a thing
    filler(buf, "..", NULL, 0);	//the directory 'above', I think for us will always be the root
    filler(buf, hello_path + 1, NULL, 0);	//'hello' (the path without the slash at the beginning)
	filler(buf, "herpderp.txt", NULL, 0);
	//in the actual thing, will we be reading from the 'directories' file?
	//to generate the list, I mean

    return 0;	//return 0 for success
}

static int hello_open(const char *path, struct fuse_file_info *fi)	//open the file at path, with I assume corresponding 'fuse file info'
{
    if (strcmp(path, hello_path) != 0)	//if the path specified is not the path to hello
        return -ENOENT;

    if ((fi->flags & 3) != O_RDONLY)	//something to do with permissions, which I don't think we have to worry about
        return -EACCES;

    return 0;	//otherwise return 0, which is taken to mean success
}

static int hello_read(const char *path, char *buf, size_t size, off_t offset,
                      struct fuse_file_info *fi)	//read the file at path from offset for size bytes, putting what we read in buf, and there's 'fuse file info'
{
    size_t len;
    (void) fi;
    if(strcmp(path, hello_path) != 0)		//if the given path and the path to the 'hello' file are not the same
        return -ENOENT;

    len = strlen(hello_str);	//len is the length of the hello string "Hello World!\n"
    if (offset < len) 	//if the offset (where we start reading) is not beyond the length of the file
	{
        if (offset + size > len)	//if you'd be trying to copy beyond the length of the file
            size = len - offset;	//ensures we only copy from offset to the end
        memcpy(buf, hello_str + offset, size);	//copies 'size' bytes from 'offset' indexes into the hello string into buf
    } else
        size = 0;

    return size;	//I suppose returns how many bytes we copied, and also now 'buf' is storing a copy of what was read
}

static struct fuse_operations hello_oper = 
{
    .getattr    = hello_getattr,			//maps calls to the above implementations
    .readdir    = hello_readdir,
    .open       = hello_open,
    .read       = hello_read,
};

int main(int argc, char *argv[])	//I think just creates a FUSE 'thing', with mount point the directory specified as a command line argument, and these implementations mapped to the stuff
{
    return fuse_main(argc, argv, &hello_oper, NULL);	
}
