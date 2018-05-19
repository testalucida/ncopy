/*
 * Copyright (c) 2009-2010, Oracle and/or its affiliates. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice,
 *   this list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * * Neither the name of Oracle nor the names of its contributors
 *   may be used to endorse or promote products derived from this software without
 *   specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/**
 * 
 * @param argc
 * @param argv:
 *      1. argument: name of destination file
 *      2. argument: name of source file
 *      3. argument: number of bytes to copy. Entering a raw number will be 
 *                   taken as number of bytes. Entering a number followed
 *                   by a 'k' or a 'm' will be taken as kilobyte or 
 *                   megabyte. 
 * @return 0 on successful execution, a positive number elsewise.
 */

const char* WRONG_ARGUMENTS = "Wrong number of arguments";
const char* USAGE = "usage: ncopy <pathnfile of dest file> \
<pathnfile of src file> " \
"<start copying with byte n>" \
"<number of bytes to copy. Either raw number or followed by 'k' or 'm'";
const char* OPEN_SRC_ERROR = "could not open source file";
const char* OPEN_DEST_ERROR = "could not create destination file";
const char* POSITION_ERROR = "could not go to desired position";

void exit_on_failure( int rc, const char* message ) {
    fprintf( stderr, "%s\n", message );
    fprintf( stderr, "exiting program with rc = %d\n", rc );
    exit( rc );
}

void read_write( const char* dest, const char* src, int startpos, int n ) {
    FILE *in = fopen( src, "rb" );
    if( in == NULL ) exit_on_failure( 1, OPEN_SRC_ERROR );
    
    
    FILE *out = fopen( dest, "wb" );
    if( out == NULL ) exit_on_failure( 1, OPEN_DEST_ERROR );
    
    if( fseek( in, startpos, SEEK_SET ) ) exit_on_failure( 1, POSITION_ERROR );
    
    int c = 0;
    for( c = fgetc( in ); c != EOF && n > 0; n-- ) {
        fputc( c, out );
        c = fgetc( in );
    }
    
    fclose( in );
    fclose( out );
}

int main( int argc, char**argv ) {
    if( argc != 5 ) {
        fprintf( stderr, "%s\n", USAGE );
        exit_on_failure( 1, WRONG_ARGUMENTS );
    }
    int i;
    
    // Prints arguments
    printf("Arguments:\n");
    for (i = 0; i < argc; i++) {
        printf("%i: %s\n", i, argv[i]);
    }
    
    //check if fifth argument is given as kilo or mega
    const char* arg4 = argv[4];
    int multiplier[] = { 1024, 1024, 1048576, 1048576, 1 };
    const char check[] = {'k', 'K', 'm', 'M', 0x00 };
    
    int k = 0;
    int kmax = strlen( check );
    char* pos = NULL;
    for( ; k < kmax; k++ ) {
        pos = strchr( arg4, check[k] );
        if( pos ) {
            *pos = 0x00;
            break;
        }
    }
    
    int nbytes = ( atoi( arg4 ) * multiplier[k] );
    
    read_write( argv[1], argv[2], atoi( argv[3] ), nbytes );

    return 0;
}
