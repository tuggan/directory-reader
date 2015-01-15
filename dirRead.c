/*
 *    Copyright 2015 Dennis Vesterlund
 *
 *   Licensed under the Apache License, Version 2.0 (the "License");
 *   you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 */

#include "dirRead.h"

dynArray directoryList(dynArray list, char *path) {
    if(list == NULL)
        return list;
    DIR *dp;
    struct dirent *ep;     
    dp = opendir (path);

    if (dp != NULL)
    {
        while ((ep = readdir (dp)))
            dynA_append(list,(void *)createEntry(path, ep));

        (void) closedir (dp);
    }
    else
        perror ("Couldn't open the directory");

    return list;

}

struct entry *createEntry(char *path, struct dirent *de) {
    
    struct entry *ent = calloc(sizeof(struct entry), 1);

    ent->nlength = 0;
    memset(ent->name, 0, 256);
    ent->type = d_unknown;
    
    if(de == NULL)
        return ent;

    ent->nlength = de->d_reclen;
    strncpy(ent->name, de->d_name, de->d_reclen);

    ent->nlength = de->d_reclen;

    size_t length = ( strlen(path) + de->d_reclen + 2 );
    
    char *fullpath = calloc(sizeof(char), length);

    strcpy(fullpath, path);
    strcat(fullpath, "/");
    strcat(fullpath, de->d_name);
    
    struct stat st;
    
    lstat(fullpath, &st);

    if(S_ISDIR(st.st_mode)) {
        ent->type = d_directory;
    } else if(S_ISLNK(st.st_mode)) {
        ent->type = d_link;
    } else if(S_ISSOCK(st.st_mode)) {
        ent->type = d_socket;
    } else if(S_ISFIFO(st.st_mode)) {
        ent->type = d_fifo;
    } else {
        ent->type = d_file;
    }

    ent->pusr |= (st.st_mode & S_IRUSR)? (1<<0) : 0;
    ent->pusr |= (st.st_mode & S_IWUSR)? (1<<1) : 0;
    ent->pusr |= (st.st_mode & S_IXUSR)? (1<<2) : 0;
    ent->pgrp |= (st.st_mode & S_IRGRP)? (1<<0) : 0;
    ent->pgrp |= (st.st_mode & S_IWGRP)? (1<<1) : 0;
    ent->pgrp |= (st.st_mode & S_IXGRP)? (1<<2) : 0;
    ent->poth |= (st.st_mode & S_IROTH)? (1<<0) : 0;
    ent->poth |= (st.st_mode & S_IWOTH)? (1<<1) : 0;
    ent->poth |= (st.st_mode & S_IXOTH)? (1<<2) : 0;

    ent->fsize = st.st_size;

    return ent;
}



const char *stringFromType(enum dr_type t) {
    static const char *strings[] = {
        "unknown", "directory", "file", "link", "socket", "fifo"
    };

    return strings[t];
}



void sortDirList(dynArray arr) {
    qsort(dynA_getCArray(arr), dynA_size(arr), sizeof(void *), compareNames);
}

int compareNames(const void *a, const void *b) {
    struct entry **ap = (struct entry **)a;
    struct entry **bp = (struct entry **)b;
    
    return strcmp((*ap)->name, (*bp)->name);
}

void printList(dynArray list) {
    printf("%-25s %-12s %-12s %-13s\n","Name", "Type", "Permissions", "Size");
    struct entry *e;
    unsigned long i;
    for(i = 0; i < dynA_size(list); i++) {
        e = (struct entry *)dynA_get(list, i);
        printf("%-25.20s %-12.10s %u%u%-7u %s\n",
               e->name,
               stringFromType(e->type),
               e->pusr,
               e->pgrp,
               e->poth,
               sizeToHuman(e->fsize));
          
    }
}

char *sizeToHuman(off_t size) {

    const unsigned char h[] = "KMGTPEZY";

    
    off_t curr = size;
    unsigned char index = 0;
    
    while(curr > 1000) {
        index++;
        curr /= 1000;
    }

    char *buffer = malloc(50*sizeof(char));
    if(index < 1) {
        sprintf(buffer, "%7ld", size);
    } else {
        float f = size / 1000*index;
        sprintf(buffer, "%6.1f%c", f, h[--index]);
    }
           
    return buffer;
}



















