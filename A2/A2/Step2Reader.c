/*
************************************************************
* COMPILERS COURSE - Algonquin College
* Code version: Summer, 2025
* Author: Suleiman Amin - 041 130 274 / Blessy Joy - 041 134 4029
* Professors: Paulo Sousa
************************************************************
#
# ECHO "=---------------------------------------="
# ECHO "|  COMPILERS - ALGONQUIN COLLEGE (S25)  |"
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
# ECHO "    @@         R I G A A X         @@    ”
# ECHO "    @@                             @@    ”
# ECHO "    @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@    ”
# ECHO "                                         "
# ECHO "[READER SCRIPT .........................]"
# ECHO "                                         "
*/

/*
***********************************************************
* File name: Reader.c
* Compiler: MS Visual Studio 2022
* Course: CST 8152 – Compilers, Lab Section: [011, 012, 013]
* Assignment: A12.
* Date: Jan 01 2025
* Professor: Paulo Sousa
* Purpose: This file is the main code for Buffer/Reader (A12)
************************************************************
*/

/*
 *.............................................................................
 * MAIN ADVICE:
 * - Please check the "TODO" labels to develop your activity.
 * - Review the functions to use "Defensive Programming".
 *.............................................................................
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef CODER_H_
#include "Step1Coder.h"
#endif

#ifndef READER_H_
#include "Step2Reader.h"
#endif

 /*
 ***********************************************************
 * Function name: readerCreate
 * Purpose: Creates the buffer reader according to capacity, increment
 *	 factor and operational mode ('f', 'a', 'm')
 * Author: Svillen Ranev / Paulo Sousa
 * History/Versions: S22
 * Called functions: calloc(), malloc()
 * Parameters:
 *   size = initial capacity
 *   increment = increment factor
 *   mode = operational mode
 * Return value: bPointer (pointer to reader)
 * Algorithm: Allocation of memory according to inicial (default) values.
 *************************************************************
 */

 /* =======================================================================
	REPLACEMENT FUNCTIONS (keep your rigaax_* names + your Buffer fields)

	Notes:
	- I kept YOUR variable/type names (rigaax_intg, rigaax_char, BufferPointer, etc.)
	- I kept YOUR function signatures (so it still matches your Step2Reader.h)
	- Behavior is aligned to the “other” style: strong defensive checks + grow-by-doubling
	======================================================================= */

	/*
	 ***********************************************************
	 * Function name: readerCreate
	 * Purpose: Creates the buffer reader according to capacity and factor.
	 *************************************************************
	 */
BufferPointer readerCreate(rigaax_intg size, rigaax_real factor) {
	BufferPointer readerPointer = NULL;

	/* Defensive programming: size */
	if (size <= 0) {
		errorPrint("Invalid buffer size\n");
		return NULL;
	}
	if (size > READER_MAX_SIZE) size = READER_MAX_SIZE;

	/* Defensive programming: factor (we keep it, but the growth policy below uses doubling) */
	if (factor < 0.0f) factor = 0.0f;

	/* Allocate buffer struct */
	readerPointer = (BufferPointer)calloc(1, sizeof(Buffer));
	if (!readerPointer) {
		errorPrint("Failed to allocate memory for readerPointer\n");
		return NULL;
	}

	/* Allocate content */
	readerPointer->content = (rigaax_strg)malloc((size_t)size);
	if (!readerPointer->content) {
		errorPrint("Failed to allocate memory for content\n");
		free(readerPointer);
		return NULL;
	}

	/* Initialize histogram */
	for (rigaax_intg i = 0; i < NCHAR; ++i)
		readerPointer->histogram[i] = 0;

	/* Initialize errors + checksum */
	readerPointer->numReaderErrors = 0;
	readerPointer->checkSum = 0;

	/* Update properties */
	readerPointer->size = size;
	readerPointer->factor = factor;

	/* Initialize positions */
	readerPointer->position.read = 0;
	readerPointer->position.wrte = 0;
	readerPointer->position.mark = 0;

	/* Initialize flags (created => EMP) */
	readerPointer->flags.isEmpty = RIGAAX_TRUE;
	readerPointer->flags.isFull = RIGAAX_FALSE;
	readerPointer->flags.isRead = RIGAAX_FALSE;
	readerPointer->flags.isMoved = RIGAAX_FALSE;

	/* Safety terminator */
	if (readerPointer->size > 0)
		readerPointer->content[0] = READER_TERMINATOR;

	return readerPointer;
}


/*
***********************************************************
* Function name: readerAddChar
* Purpose: Adds a char to buffer reader (grows by doubling).
*************************************************************
*/
BufferPointer readerAddChar(BufferPointer const readerPointer, rigaax_char ch) {
	rigaax_strg tempReader = NULL;
	rigaax_intg newSize = 0;

	/* Defensive programming */
	if (!readerPointer || !readerPointer->content) {
		errorPrint("Invalid reader pointer or content\n");
		return NULL;
	}

	/* Test inclusion of chars (ASCII only) */
	if ((unsigned char)ch >= (unsigned char)NCHAR) {
		readerPointer->numReaderErrors++;
		return readerPointer; /* same pattern as your original */
	}

	/* Need room for ch + terminator => wrte must be <= size-2 before write */
	if (readerPointer->position.wrte >= readerPointer->size - 1) {
		/* Buffer full -> grow (doubling, like the other version) */
		readerPointer->flags.isFull = RIGAAX_TRUE;

		newSize = readerPointer->size * 2;
		if (newSize <= readerPointer->size) newSize = readerPointer->size + 1;
		if (newSize > READER_MAX_SIZE) newSize = READER_MAX_SIZE;

		if (newSize <= readerPointer->size) {
			/* cannot grow */
			return NULL;
		}

		tempReader = (rigaax_strg)realloc(readerPointer->content, (size_t)newSize);
		if (!tempReader) {
			errorPrint("Failed to reallocate memory for content\n");
			return NULL;
		}

		readerPointer->flags.isMoved = (tempReader != readerPointer->content) ? RIGAAX_TRUE : RIGAAX_FALSE;
		readerPointer->content = tempReader;
		readerPointer->size = newSize;

		readerPointer->flags.isFull = RIGAAX_FALSE;
	}

	/* Add the char + keep terminator */
	readerPointer->content[readerPointer->position.wrte++] = ch;
	readerPointer->content[readerPointer->position.wrte] = READER_TERMINATOR;

	/* Update flags */
	readerPointer->flags.isEmpty = (readerPointer->position.wrte == 0) ? RIGAAX_TRUE : RIGAAX_FALSE;
	readerPointer->flags.isRead = RIGAAX_FALSE;
	readerPointer->flags.isFull = (readerPointer->position.wrte >= readerPointer->size - 1) ? RIGAAX_TRUE : RIGAAX_FALSE;

	/* Update histogram */
	readerPointer->histogram[(unsigned char)ch]++;

	return readerPointer;
}


/*
***********************************************************
* Function name: readerClear
* Purpose: Clears the buffer reader (keeps allocation).
*************************************************************
*/
rigaax_boln readerClear(BufferPointer const readerPointer) {
	if (!readerPointer || !readerPointer->content) {
		errorPrint("Invalid reader pointer or content\n");
		return RIGAAX_FALSE;
	}

	readerPointer->position.read = 0;
	readerPointer->position.wrte = 0;
	readerPointer->position.mark = 0;

	readerPointer->content[0] = READER_TERMINATOR;

	for (rigaax_intg i = 0; i < NCHAR; ++i)
		readerPointer->histogram[i] = 0;

	readerPointer->numReaderErrors = 0;
	readerPointer->checkSum = 0;

	readerPointer->flags.isEmpty = RIGAAX_TRUE;
	readerPointer->flags.isFull = RIGAAX_FALSE;
	readerPointer->flags.isRead = RIGAAX_FALSE;
	readerPointer->flags.isMoved = RIGAAX_FALSE;

	return RIGAAX_TRUE;
}


/*
***********************************************************
* Function name: readerFree
* Purpose: Releases the buffer address.
*************************************************************
*/
rigaax_boln readerFree(BufferPointer const readerPointer) {
	if (!readerPointer) {
		errorPrint("Invalid reader pointer\n");
		return RIGAAX_FALSE;
	}

	if (readerPointer->content)
		free(readerPointer->content);

	free((void*)readerPointer);
	return RIGAAX_TRUE;
}


/*
***********************************************************
* Function name: readerIsFull
*************************************************************
*/
rigaax_boln readerIsFull(BufferPointer const readerPointer) {
	if (!readerPointer) {
		errorPrint("Invalid reader pointer\n");
		return RIGAAX_FALSE;
	}
	return readerPointer->flags.isFull;
}


/*
***********************************************************
* Function name: readerIsEmpty
*************************************************************
*/
rigaax_boln readerIsEmpty(BufferPointer const readerPointer) {
	if (!readerPointer) {
		errorPrint("Invalid reader pointer\n");
		return RIGAAX_FALSE;
	}
	return readerPointer->flags.isEmpty;
}


/*
***********************************************************
* Function name: readerSetMark
*************************************************************
*/
rigaax_boln readerSetMark(BufferPointer const readerPointer, rigaax_intg mark) {
	if (!readerPointer) {
		errorPrint("Invalid reader pointer\n");
		return RIGAAX_FALSE;
	}

	/* boundary: mark must be within [0, wrte] */
	if (mark < 0 || mark > readerPointer->position.wrte) {
		errorPrint("Mark out of bounds\n");
		return RIGAAX_FALSE;
	}

	readerPointer->position.mark = mark;
	return RIGAAX_TRUE;
}


/*
***********************************************************
* Function name: readerPrint
* Purpose: Prints the buffer content (does not decode; prints what is inside).
*************************************************************
*/
rigaax_intg readerPrint(BufferPointer const readerPointer) {
	if (!readerPointer || !readerPointer->content) {
		errorPrint("Invalid reader pointer or content\n");
		return 0;
	}

	rigaax_intg printed = 0;
	for (rigaax_intg i = 0; i < readerPointer->position.wrte; ++i) {
		rigaax_char c = readerPointer->content[i];
		if (c == READER_TERMINATOR) break;

		if ((unsigned char)c < (unsigned char)NCHAR) {
			putchar(c);
			printed++;
		}
		else {
			((BufferPointer)readerPointer)->numReaderErrors++;
		}
	}

	return printed;
}


/*
***********************************************************
* Function name: readerLoad
* Purpose: Loads the string in the buffer with decoded file content
*          using vigenereMem(fileName, STR_LANGNAME, DECYPHER).
*************************************************************
*/
rigaax_intg readerLoad(BufferPointer const readerPointer, rigaax_strg fileName) {
    if (!readerPointer || !readerPointer->content || !fileName) {
        errorPrint("Invalid reader pointer/content or fileName\n");
        return READER_ERROR;
    }

    /* Decrypt whole file into memory */
    rigaax_strg decoded = vigenereMem(fileName, STR_LANGNAME, DECYPHER);
    if (!decoded) {
        errorPrint("vigenereMem failed (check key/mode/file)\n");
        return READER_ERROR;
    }

    /* Clear buffer first */
    readerClear(readerPointer);

    /* Load decrypted string into buffer */
    rigaax_intg loaded = 0;
    for (rigaax_intg i = 0; decoded[i] != '\0'; ++i) {
        if (!readerAddChar(readerPointer, decoded[i])) {
            free(decoded);
            errorPrint("Failed to add char while loading\n");
            return READER_ERROR;
        }
        loaded++;
    }

    free(decoded);
    return loaded;
}


/*
***********************************************************
* Function name: readerRecover
*************************************************************
*/
rigaax_boln readerRecover(BufferPointer const readerPointer) {
	if (!readerPointer) {
		errorPrint("Invalid reader pointer\n");
		return RIGAAX_FALSE;
	}

	readerPointer->position.read = 0;
	readerPointer->position.mark = 0;
	readerPointer->flags.isRead = RIGAAX_FALSE;

	return RIGAAX_TRUE;
}


/*
***********************************************************
* Function name: readerRetract
*************************************************************
*/
rigaax_boln readerRetract(BufferPointer const readerPointer) {
	if (!readerPointer) {
		errorPrint("Invalid reader pointer\n");
		return RIGAAX_FALSE;
	}

	if (readerPointer->position.read <= 0)
		return RIGAAX_FALSE;

	readerPointer->position.read--;
	readerPointer->flags.isRead = RIGAAX_FALSE;
	return RIGAAX_TRUE;
}


/*
***********************************************************
* Function name: readerRestore
*************************************************************
*/
rigaax_boln readerRestore(BufferPointer const readerPointer) {
	if (!readerPointer) {
		errorPrint("Invalid reader pointer\n");
		return RIGAAX_FALSE;
	}

	readerPointer->position.read = readerPointer->position.mark;
	readerPointer->flags.isRead = RIGAAX_FALSE;
	return RIGAAX_TRUE;
}


/*
***********************************************************
* Function name: readerGetChar
*************************************************************
*/
rigaax_char readerGetChar(BufferPointer const readerPointer) {
	if (!readerPointer || !readerPointer->content) {
		errorPrint("Invalid reader pointer or content\n");
		return READER_TERMINATOR;
	}

	if (readerPointer->position.read >= readerPointer->position.wrte) {
		readerPointer->flags.isRead = RIGAAX_TRUE; /* END */
		return READER_TERMINATOR;
	}

	readerPointer->flags.isRead = RIGAAX_FALSE;
	return readerPointer->content[readerPointer->position.read++];
}


/*
***********************************************************
* Function name: readerGetContent
*************************************************************
*/
rigaax_strg readerGetContent(BufferPointer const readerPointer, rigaax_intg pos) {
	if (!readerPointer || !readerPointer->content) {
		errorPrint("Invalid reader pointer or content\n");
		return NULL;
	}

	if (pos < 0 || pos >= readerPointer->position.wrte) {
		errorPrint("Requested position is out of bounds\n");
		return NULL;
	}

	return &(readerPointer->content[pos]);
}


/*
***********************************************************
* Function name: readerGetPosRead
*************************************************************
*/
rigaax_intg readerGetPosRead(BufferPointer const readerPointer) {
	if (!readerPointer) {
		errorPrint("Invalid reader pointer\n");
		return 0;
	}
	return readerPointer->position.read;
}


/*
***********************************************************
* Function name: readerGetPosWrte
*************************************************************
*/
rigaax_intg readerGetPosWrte(BufferPointer const readerPointer) {
	if (!readerPointer) {
		errorPrint("Invalid reader pointer\n");
		return 0;
	}
	return readerPointer->position.wrte;
}


/*
***********************************************************
* Function name: readerGetPosMark
*************************************************************
*/
rigaax_intg readerGetPosMark(BufferPointer const readerPointer) {
	if (!readerPointer) {
		errorPrint("Invalid reader pointer\n");
		return 0;
	}
	return readerPointer->position.mark;
}


/*
***********************************************************
* Function name: readerGetSize
*************************************************************
*/
rigaax_intg readerGetSize(BufferPointer const readerPointer) {
	if (!readerPointer) {
		errorPrint("Invalid reader pointer\n");
		return 0;
	}
	return readerPointer->size;
}


/*
***********************************************************
* Function name: readerPrintFlags
* Purpose: Prints flags (EMP, FUL, END, REL)
*************************************************************
*/
rigaax_void readerPrintFlags(BufferPointer const readerPointer) {
	if (!readerPointer) return;

	printf("EMP: %d  ", readerPointer->flags.isEmpty ? 1 : 0);
	printf("FUL: %d  ", readerPointer->flags.isFull ? 1 : 0);
	printf("END: %d  ", readerPointer->flags.isRead ? 1 : 0);
	printf("REL: %d\n", readerPointer->flags.isMoved ? 1 : 0);
}


/*
***********************************************************
* Function name: readerPrintStat
*************************************************************
*/
rigaax_void readerPrintStat(BufferPointer const readerPointer) {
	if (!readerPointer) {
		errorPrint("Invalid reader pointer\n");
		return;
	}

	for (rigaax_intg i = 0; i < NCHAR; ++i) {
		if (readerPointer->histogram[i] > 0) {
			if (isprint(i))
				printf("  B['%c'] = %d\n", (char)i, readerPointer->histogram[i]);
			else
				printf("  B[0x%02X] = %d\n", (unsigned)i, readerPointer->histogram[i]);
		}
	}
}


/*
***********************************************************
* Function name: readerNumErrors
*************************************************************
*/
rigaax_intg readerNumErrors(BufferPointer const readerPointer) {
	if (!readerPointer) {
		errorPrint("Invalid reader pointer\n");
		return 0;
	}
	return readerPointer->numReaderErrors;
}


/*
***********************************************************
* Function name: readerChecksum
* Purpose: Sets/returns the checksum (8 bits) from content.
*************************************************************
*/
rigaax_intg readerChecksum(BufferPointer readerPointer) {
	if (!readerPointer || !readerPointer->content) {
		errorPrint("Invalid reader pointer or content\n");
		return 0;
	}

	unsigned int sum = 0;
	for (rigaax_intg i = 0; i < readerPointer->position.wrte; ++i)
		sum += (unsigned char)readerPointer->content[i];

	readerPointer->checkSum = (rigaax_intg)(sum & 0xFF);
	return readerPointer->checkSum;
}
