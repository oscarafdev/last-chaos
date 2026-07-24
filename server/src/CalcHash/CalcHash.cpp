/* Copyright (c) 2024 Alexander Pavlov <t.x00100x.t@yandex.ru>

This program is free software; you can redistribute it and/or modify
it under the terms of version 2 of the GNU General Public License as published by
the Free Software Foundation


This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program; if not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA. */

// CalcHash - Calculate hash for db_auth/bg_user

#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "il_crypt.h"
#include "il_system.h"
#include "LCSha256.h"

#define BUZZ_SIZE 1024

int main( int argc, char *argv[])
{
  printf("\nCalcHash - Calculate hash for db_auth/bg_user\n");
  printf(  "           (C)2024  Alexander Pavlov <t.x00100x.t@yandex.ru>\n\n");

  // 2 parameters are allowed as input
  if((argc<3) || (argc>3))
  {
    printf( " USAGE: CalcHash <username> <password>\n");
    printf( "\n");
    printf( "\n");
    printf( " This utility is designed to work based on the\n");
    printf( " Server 2018 codebase. The utility generates a\n");
    printf( " hash of the SHA256 based on the server salt key,\n");
    printf( " username and password of the user. Place the\n");
    printf( " generated hash in the pass field of the db_auth\n");
    printf( " database user of the bg_user table.\n");
    printf( "\n");
    exit(EXIT_FAILURE);
  }

  char * user = (char*) malloc(BUZZ_SIZE);  
  char * pass = (char*) malloc(BUZZ_SIZE); 
  char * salt = (char*) malloc(BUZZ_SIZE); 
  char * hash = (char*) malloc(BUZZ_SIZE); 

  memset(user, 0x00, BUZZ_SIZE);
  memset(pass, 0x00, BUZZ_SIZE);
  memset(salt, 0x00, BUZZ_SIZE);
  memset(hash, 0x00, BUZZ_SIZE);

  memcpy(user, argv[1], strlen(argv[1]));
  memcpy(pass, argv[1], strlen(argv[2]));

  FILE *file;

  // Read salt
  printf("\nRead salt.dat: \n");
  file = fopen("salt.dat", "r");
  if(file == NULL) {
    printf("\nCannot open 'salt.dat' file.\n");
    exit(EXIT_FAILURE);
  }
  fgets(salt, BUZZ_SIZE, file);
  printf("Salt: %s\n", (const char*)salt);
  fclose(file);

  // Calculate hash
  bool ret = ConverToHash256((const char*)user, (const char*)salt, (const char*)pass, hash, BUZZ_SIZE);
  if(!ret) {
    printf( "CalcHash: Error Conver To Hash.\nTry again.\n");
    free(hash);
    free(salt);
    free(pass);
    free(user);
    exit(EXIT_FAILURE);
  }

  // Write hash
  file = fopen("hash.txt", "wb");
  fwrite((void *)hash, sizeof(char), strlen(hash), file);
  fclose(file);

  printf("Hash: %s\n", (const char*)hash);
  printf("Writen to hash.txt\n");

  // Free
  free(hash);
  free(salt);
  free(pass);
  free(user);
  return 0;
}


