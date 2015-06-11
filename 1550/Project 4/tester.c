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

int readDir(char* path)
{
	printf("READ DIR\n");
	FILE *dfile;
	dfile = fopen(".derp", "rb");
	char directory[MAX_FILENAME + 1], filename[MAX_FILENAME + 1], extension[MAX_EXTENSION + 1];
	sscanf(path, "/%[^/]/%[^.].%s", directory, filename, extension);
	if (dfile == NULL)
	{
		printf("Derp file not found, returning -ENOENT\n");
		//DON'T CLOSE THE FILE IF IT WASN'T SUCCESSFULLY OPENED
		//fclose(dfile);
		return -ENOENT;
	}
	if (strcmp(path, "/") == 0) 
	{      
		printf("Path is root, returning success\n");
		fclose(dfile);
		return 0;
    } 
	cs1550_directory_entry curr;
	int validDirectory = 0;
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
		printf("No match found, returning -ENOENT\n");
		fclose(dfile);
		return -ENOENT;	//if we've read all directories without a match, then the path must be invalid
	}
	if (strlen(path) == (strlen(directory) + 1))	//if there is no file name, then it's just a subdirectory
	{
		printf("No file name, so just the directory, returning success\n");
		fclose(dfile);
		return 0;
	}
	int i = 0;
	while (i < curr.nFiles)	//we'll look at all files in directory curr
	{
		if (strcmp(curr.files[i].fname, filename) == 0 && strcmp(curr.files[i].fext, extension) == 0)	//if the file names and extensions match
		{
			printf("File match found, returning success\n");
			fclose(dfile);
			return 0;
		}
		i += 1;	//to consider the next file
	}
	printf("Made it all the way to the end somehow, so returning -ENOENT\n");
	fclose(dfile);
	return -ENOENT;
}

int createDir(char* path)
{
	printf("CREATE DIR\n");
	char directory[MAX_FILENAME + 1], filename[MAX_FILENAME + 1], extension[MAX_EXTENSION + 1];
	sscanf(path, "/%[^/]/%[^.].%s", directory, filename, extension);	//assuming the path still has all these components
	//really there should just be a directory
	if (strlen(directory) > MAX_FILENAME)	//if the directory name is longer than the maximum allowed name length
	{
		printf("Name too long, returning -ENAMETOOLONG\n");
		return -ENAMETOOLONG;	//return this aptly-named error
	}
	if (strlen(path) > (strlen(directory) + 1))	//if there's more to the path than the '/' + the directory name
	{
		printf("Directory not under root, returning -EPERM\n");
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
	dfile = fopen(".derp", "rb");
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
		printf("The directory already exists, returning -EEXIST\n");
		return -EEXIST;
	}
	cs1550_directory_entry newdir;
	strcpy(newdir.dname, directory);	//presumably it should be named as the directory part of the path
	newdir.nFiles = 0;			//we're just creating it now, so it shouldn't have any files in it
	//and I suppose we can leave its 'files' array as all null
	FILE *dfile2;
	dfile2 = fopen(".derp", "ab");	//reopen the .directories file, this time for appending (should create if not already there)
	if (dfile2 == NULL)	//shouldn't ever be the case, as it will either already be there or be created, but whatever
	{
		printf("Opening to append didn't work somehow\n");
		//fprintf(stderr, "\nError opening .directories\n\n");
		//exit(1);
	}
	fwrite(&newdir, sizeof(cs1550_directory_entry), 1, dfile2);	//write the new directory entry to the .directories file
	fclose(dfile2);
	return 0;
}

int readDirectories(void)
{
	FILE *dfile;
	dfile = fopen(".directories", "rb");
	cs1550_directory_entry curr;
	while (fread (&curr, sizeof(cs1550_directory_entry), 1, dfile))
	{
		printf("Directory: %s\n", curr.dname);
		printf("Number of files: %d\n", curr.nFiles);
		int numFiles = curr.nFiles;
		int i = 0;
		while (i < numFiles)
		{
			printf("\tFile: %s\n", curr.files[i].fname);
			printf("\t\tSize: %d\n", curr.files[i].fsize);
			printf("\t\tStart: %d\n", curr.files[i].nStartBlock);
			printf("\t\tExtension: %s\n", curr.files[i].fext);
			i += 1;
		}
	}
	return 0;
}

int main(int argc, char *argv[])
{
	readDirectories();
	/*
	char directory[MAX_FILENAME + 1], filename[MAX_FILENAME + 1], extension[MAX_EXTENSION + 1];
	char* path;
	int theSize = sizeof(cs1550_disk_block);
	printf("The size of a disk block is: %d\n", theSize);
	path = "/Marsha.txt";
	sscanf(path, "/%[^/]/%[^.].%s", directory, filename, extension);
	printf("Directory: %s\n", directory);
	printf("Filename: %s\n", filename);
	printf("Extension: %s\n", extension);
	printf("Path length: %d\n", strlen(path));
	printf("Directory length: %d\n", strlen(directory));
	printf("File length: %d\n", strlen(filename));
	printf("Extension length: %d\n", strlen(extension));
	*/
	//readDir(path);
	//createDir(path);
	//readDir(path);
    return 0;
}