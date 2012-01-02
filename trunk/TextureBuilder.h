#ifndef TEXTURE_BUILDER
#define TEXTURE_BUILDER



#include <windows.h>
#include <tchar.h>
#include <string>
#include <stdio.h>
#include <glut.h>

GLuint LoadTexture( const char * filename, int width, int height, int wrap )
{
      GLuint texture;
    //int width, height;
    BYTE * data;
    FILE * file;

    // open texture data
    file = fopen( filename, "rb" );
    if ( file == NULL ) return 0;

    // allocate buffer
    //width = 256;
   // height = 256;
    data = (BYTE *) malloc( width * height * 3 );

    // read texture data
    fread( data, width * height * 3, 1, file );
    fclose( file );

    glGenTextures( 1, &texture );
    glBindTexture( GL_TEXTURE_2D, texture );

    glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );

    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

    // texture wraps over at the edges
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, 1 );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, 1);

    // build texture
    gluBuild2DMipmaps( GL_TEXTURE_2D, 3, width, height,
                       GL_RGB, GL_UNSIGNED_BYTE, data );

    // free buffer
    free( data );

    return texture;

}

#endif