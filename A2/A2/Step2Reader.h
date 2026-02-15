/*
************************************************************
* COMPILERS COURSE - Algonquin College
* Code version: Fall, 2025
* Author: TO_DO
* Professors: Paulo Sousa
************************************************************
#
# ECHO "=---------------------------------------="
# ECHO "|  COMPILERS - ALGONQUIN COLLEGE (F25)  |"
# ECHO "=---------------------------------------="
# ECHO "    @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@    ”
# ECHO "    @@                             @@    ”
# ECHO "    @@           %&@@@@@@@@@@@     @@    ”
# ECHO "    @@       @%% (@@@@@@@@@  @     @@    ”
# ECHO "    @@      @& @   @ @       @     @@    ”
# ECHO "    @@     @ @ %  / /   @@@@@@     @@    ”
# ECHO "    @@      & @ @  @@              @@    ”
# ECHO "    @@       @/ @*@ @ @   @        @@    ”
# ECHO "    @@           @@@@  @@ @ @      @@    ”
# ECHO "    @@            /@@    @@@ @     @@    ”
# ECHO "    @@     @      / /     @@ @     @@    ”
# ECHO "    @@     @ @@   /@/   @@@ @      @@    ”
# ECHO "    @@     @@@@@@@@@@@@@@@         @@    ”
# ECHO "    @@                             @@    ”
# ECHO "    @@         S O F I A           @@    ”
# ECHO "    @@                             @@    ”
# ECHO "    @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@    ”
# ECHO "                                         "
# ECHO "[READER SCRIPT .........................]"
# ECHO "                                         "
*/

/*
************************************************************
* File name: Reader.h
* Compiler: MS Visual Studio 2022
* Course: CST 8152 – Compilers, Lab Section: [011, 012]
* Assignment: A12.
* Date: Jan 01 2025
* Professor: Paulo Sousa
* Purpose: This file is the main header for Reader (.h)
************************************************************
*/

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef READER_H_
#define READER_H_

#include <stdio.h>
#include <malloc.h>
#include <limits.h>

/* GENERAL CONSTANTS */
#define READER_ERROR        (-1)
#define READER_TERMINATOR   '\0'

#define READER_MAX_SIZE         INT_MAX-1
#define READER_DEFAULT_SIZE     100
#define READER_DEFAULT_FACTOR   0.5f

#define NCHAR       128
#define CHARSEOF    (-1)

/* STRUCTURES */

/* Offset declaration */
typedef struct position {
    rigaax_intg wrte;
    rigaax_intg read;
    rigaax_intg mark;
} Position;

/* Flags declaration */
typedef struct flag {
    rigaax_boln isEmpty;
    rigaax_boln isFull;
    rigaax_boln isRead;
    rigaax_boln isMoved;
} Flag;

/* Buffer structure */
typedef struct bufferReader {
    rigaax_strg      content;
    rigaax_intg      size;
    rigaax_real      factor;
    Flag             flags;
    Position         position;
    rigaax_intg      histogram[NCHAR];
    rigaax_intg      numReaderErrors;
    rigaax_intg      checkSum;
} Buffer, * BufferPointer;

/* FUNCTIONS */

/* General Operations */
BufferPointer   readerCreate(rigaax_intg, rigaax_real);
BufferPointer   readerAddChar(BufferPointer const, rigaax_char);
rigaax_boln     readerClear(BufferPointer const);
rigaax_boln     readerFree(BufferPointer const);
rigaax_boln     readerIsFull(BufferPointer const);
rigaax_boln     readerIsEmpty(BufferPointer const);
rigaax_boln     readerSetMark(BufferPointer const, rigaax_intg);
rigaax_intg     readerPrint(BufferPointer const);
rigaax_intg     readerLoad(BufferPointer const, rigaax_strg);
rigaax_boln     readerRecover(BufferPointer const);
rigaax_boln     readerRetract(BufferPointer const);
rigaax_boln     readerRestore(BufferPointer const);
rigaax_intg     readerChecksum(BufferPointer const);

/* Getters */
rigaax_char     readerGetChar(BufferPointer const);
rigaax_strg     readerGetContent(BufferPointer const, rigaax_intg);
rigaax_intg     readerGetPosRead(BufferPointer const);
rigaax_intg     readerGetPosWrte(BufferPointer const);
rigaax_intg     readerGetPosMark(BufferPointer const);
rigaax_intg     readerGetSize(BufferPointer const);
rigaax_void     readerPrintFlags(BufferPointer const);
rigaax_void     readerPrintStat(BufferPointer const);
rigaax_intg     readerNumErrors(BufferPointer const);

#endif
