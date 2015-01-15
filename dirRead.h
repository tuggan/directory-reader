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

#ifndef DIRREAD_H
#define DIRREAD_H

#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>

#include "dynamicArray.h"

typedef enum dr_type {
    d_unknown,
    d_directory,
    d_file,
    d_link,
    d_socket,
    d_fifo
} dr_type;

#define DR_TYPE_STR(type)                                \
    (d_unknown   == type ? "unknown"   :                 \
    (d_directry  == type ? "directory" :                 \
    (d_file      == type ? "file"      :                 \
    (d_link      == type ? "link"      :                 \
    (d_socket    == type ? "socket"    :                 \
    (d_fifo      == type ? "fifo"      : "unknown")))))) 

struct entry {
    unsigned short nlength;
    char name[256];
    enum dr_type type;
    off_t fsize;
    uid_t uid;
    gid_t gid;
    unsigned char pusr;
    unsigned char pgrp;
    unsigned char poth;
};


dynArray directoryList(dynArray list, char *path);

struct entry *createEntry(char *path, struct dirent *de);

const char *stringFromType(enum dr_type t);

void sortDirList(dynArray arr);

int compareNames(const void *a, const void *b);

void printList(dynArray list);

char *sizeToHuman(off_t size);

#endif


















