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

static const char *hello_str = "Hello World!\n";
static const char *hello_path = "/hello";

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
			dfile = fopen(".directories", "rb");
			cs1550_directory_entry curr;
			int validDirectory = 0;
			if (dfile == NULL)	//if the directories file doesn't exist yet, then obviously the specified directory doesn't exist
			{
				return -ENOENT;
				//fprintf(stderr, "\nError opening .directories\n\n");
				//exit(1);
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
				stbuf->st_mode = S_IFDIR | 0755;	//supposedly these values are appropriate for a subdirectory
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
					stbuf->st_mode = S_IFREG | 0666;	//supposedly these values are appropriate for a file
					stbuf->st_nlink = 1; //file links
					stbuf->st_size = curr.files[i].fsize; //file size - make sure you replace with real size!
					res = 0; // no error
					fclose(dfile);
					return res;
				}
				i += 1;	//to consider the next file
			}
        //Check if name is subdirectory
        /*
                //Might want to return a structure with these fields
                stbuf->st_mode = S_IFDIR | 0755;
                stbuf->st_nlink = 2;
                res = 0; //no error
        */

        //Check if name is a regular file
        /*
                //regular file, probably want to be read and write
                stbuf->st_mode = S_IFREG | 0666;
                stbuf->st_nlink = 1; //file links
                stbuf->st_size = 0; //file size - make sure you replace with real size!
                res = 0; // no error
        */

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
		dfile = fopen(".directories", "rb");
		cs1550_directory_entry curr;
		if (dfile == NULL && isRoot == 0)	//if the directories file doesn't exist yet, then obviously the specified directory doesn't exist
		{
			char derp[30];
			strcpy(derp, "HEY, WE'RE HERE\n");
			filler(buf, derp, NULL, 0);
			return -ENOENT;
			//fprintf(stderr, "\nError opening .directories\n\n");
			//exit(1);
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
				strcat(newpath, curr.files[i].fext);	//append file extension to newpath
				filler(buf, newpath + 1, NULL, 0);		//add newpath to the list
				i += 1;	//to consider the next file
			}
		}
        //This line assumes we have no subdirectories, need to change
        //if (strcmp(path, "/") != 0)
        //return -ENOENT;

        //the filler function allows us to add entries to the listing
        //read the fuse.h file for a description (in the ../include dir)
        filler(buf, ".", NULL, 0);	//I think we just always add these
        filler(buf, "..", NULL, 0);

        /*
        //add the user stuff (subdirs or files)
        //the +1 skips the leading '/' on the filenames
        filler(buf, newpath + 1, NULL, 0);
        */
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
        //(void) path;
        (void) mode;
		char directory[MAX_FILENAME + 1], filename[MAX_FILENAME + 1], extension[MAX_EXTENSION + 1];
		sscanf(path, "/%[^/]/%[^.].%s", directory, filename, extension);	//assuming the path still has all these components
		//really there should just be a directory
		if (strlen(directory) > MAX_FILENAME)	//if the directory name is longer than the maximum allowed name length
		{
			return -ENAMETOOLONG;	//return this aptly-named error
		}
		if (strlen(path) > (strlen(directory) + 1))	//if there's more to the path than the '/' + the directory name
		{
			return -EPERM;	//the directory is not under the root
			//though I'm not sure how the path gets split
			//if the directory is the entire directory path, this won't always work
			//like for this case: /a/b/dir (strlen(directory) = 7, strlen(path) = 8) checks out, but it shouldn't
			//but that's only if it's the entire directory path
			//okay apparently the way it works is the directory is the FIRST thing after the slash
			//then the filename is EVERYTHING after (aside from the extension)
			//so yeah I THINK this is actually valid
		}
		int validDirectory = 0;
		FILE *dfile;
		dfile = fopen(".directories", "rb");
		cs1550_directory_entry curr;
		if (dfile == NULL)	//if there is no directories file, do nothing for now
		{
			//fprintf(stderr, "\nError opening .directories\n\n");
			//exit(1);
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
		if (dfile2 == NULL)	//shouldn't ever be the case, as it will either already be there or be created, but whatever
		{
			//fprintf(stderr, "\nError opening .directories\n\n");
			//exit(1);
		}
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
        (void) dev;
        (void) path;
        return 0;
}

/*
 * Deletes a file
 */
static int cs1550_unlink(const char *path)
{
    (void) path;

    return 0;
}

/*
 * Read size bytes from file into buf starting from offset
 *
 */
static int cs1550_read(const char *path, char *buf, size_t size, off_t offset,
                          struct fuse_file_info *fi)
{
        (void) buf;
        (void) offset;
        (void) fi;
        (void) path;

        //check to make sure path exists
        //check that size is > 0
        //check that offset is <= to the file size
        //read in data
        //set size and return, or error

        size = 0;

        return size;
}

/*
 * Write size bytes from buf into file starting from offset
 *
 */
static int cs1550_write(const char *path, const char *buf, size_t size,
                          off_t offset, struct fuse_file_info *fi)
{
        (void) buf;
        (void) offset;
        (void) fi;
        (void) path;

        //check to make sure path exists
        //check that size is > 0
        //check that offset is <= to the file size
        //write data
        //set size (should be same as input) and return, or error

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
