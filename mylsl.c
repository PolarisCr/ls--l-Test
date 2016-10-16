#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <dirent.h>
#include <string.h>


#define MAX_BUF 256

main()
{
	// Directory Pointer
	DIR *dp;

	// Directory Size
	int size = 0;

	// File stat Buffer
	struct stat statbuf;

	// File direct Struct
	struct dirent *dep;

	//  Original Path buf of Symbolic Link
	char ori_name[MAX_BUF];

	// User, Group Info
	struct passwd *user_info;
	struct group *group_info;

	// Modified Time String Buf
	char mod_time[30];

	// dp open
	if ((dp = opendir(".")) == NULL)
	{
		perror("opendir");
		exit(0);
	}

	// Calculate Total Size
	while (dep = readdir(dp))
	{
		// Pass ., .. Directory 
		if ((strcmp(dep->d_name, ".") == 0) || (strcmp(dep->d_name, "..") == 0))
			 continue;

		// File open
		lstat(dep->d_name, &statbuf);

		// File's 512-byte Blocks
		size += statbuf.st_blocks;
	}

	// Total Size
	printf("total %d\n", size / 2);

	// Directory Pointer Rewind
	rewinddir (dp);

	// readdir dp
	while (dep = readdir(dp))
	{
		// Pass ., .. Directory
		if ((strcmp(dep->d_name, ".") == 0) || (strcmp(dep->d_name, "..") == 0))
			continue;

		// File stat load
		lstat(dep->d_name, &statbuf);

		// Print File Type 
		if (S_ISREG(statbuf.st_mode))		// Regular File
			printf("-");
		else if (S_ISDIR(statbuf.st_mode))	// Directory File
			printf("d");
		else if (S_ISCHR(statbuf.st_mode))	// Character Special File (Device File)
			printf("c");
		else if (S_ISBLK(statbuf.st_mode))	// Block Special File
			printf("b");
		else if (S_ISLNK(statbuf.st_mode))	// Symbolic Link File
			printf("l");
		else if (S_ISFIFO(statbuf.st_mode))	// FIFO (pipe) File
			printf("p");
		else if (S_ISSOCK(statbuf.st_mode))	// Socket File
			printf("s");

		// Print File Permission
		// User Read Permission
		if (statbuf.st_mode & S_IRUSR)
			printf("r");
		else	printf("-");

		// User Write Permission
		if (statbuf.st_mode & S_IWUSR)
			printf("w");
		else	printf("-");

		// User Execute Permission
		if (statbuf.st_mode & S_IXUSR)
			printf("x");
		else	printf("-");

		// Group Read Permission
		if (statbuf.st_mode & S_IRGRP)
			printf("r");
		else	printf("-");

		// Group Write Permission
		if (statbuf.st_mode & S_IWGRP)
			printf("w");
		else	printf("-");

		// Group Execute Permission
		if (statbuf.st_mode & S_IXGRP)
			printf("x");
		else	printf("-");

		// Other Read Permission
		if (statbuf.st_mode & S_IROTH)
			printf("r");
		else	printf("-");

		// Other Write Permission
		if (statbuf.st_mode & S_IWOTH)
			printf("w");
		else	printf("-");

		// Other Execute Permission
		if (statbuf.st_mode & S_IXOTH)
			printf("x");
		else	printf("-");

		// Number of Links
		printf(" %d", (int)(statbuf.st_nlink));

		// Print User Name, Group Name
		user_info = getpwuid(statbuf.st_uid);
		group_info = getgrgid(statbuf.st_gid);
		printf(" %s", user_info->pw_name);
		printf(" %s", group_info->gr_name);

		// Print File Size
		printf(" %8ld", (long int)statbuf.st_size);

		// Print Last Modified time 
		strcpy(mod_time, ctime(&statbuf.st_mtime));	
		strtok(mod_time, " ");
		printf(" %s", strtok(NULL, " "));
		printf(" %2s", strtok(NULL, " "));
		printf(" %2s", strtok(NULL, ":"));
		printf(":%2s", strtok(NULL, ":"));

		// Print File name
		printf(" %s", dep->d_name);

		// If Symbolic Link, print original file name
		if (S_ISLNK(statbuf.st_mode))
		{
			readlink(dep->d_name, ori_name, MAX_BUF);	
			printf(" -> %s", ori_name);
		}

		// Next Line
		printf("\n");
	}

	// Directory Pointer Close
	closedir (dp);
}
