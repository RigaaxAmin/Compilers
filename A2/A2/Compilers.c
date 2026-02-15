/*
************************************************************
* COMPILERS COURSE - Algonquin College
* Code version: Winter, 2025
* Author: Suleiman Amin - 041 130 274 / Blessy Joy - 041 134 4029
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
# ECHO "    @@         R I G A A X         @@    ”
# ECHO "    @@                             @@    ”
# ECHO "    @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@    ”
# ECHO "                                         "
# ECHO "[COMPILER SCRIPT .......................]"
# ECHO "                                         "
*/

/*
************************************************************
* File name: compilers.c
* Compiler: MS Visual Studio 2022
* Course: CST 8152 – Compilers, Lab Section: [011, 012]
* Assignment: A12, A22, A32.
* Date: sEP 01 2025
* Professor: Paulo Sousa
* Purpose: This file is the main program of Compilers Project
* Function list: main().
************************************************************
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

/*
 * ............................................................................
 * ADVICE 1:
 * Please check the "TODO" labels to develop your activity.
 *
 * ADVICE 2: This file must be updated according to each assignment
 * (see the progression: reader > scanner > parser).
 * ............................................................................
 */

 /*
 ***********************************************************
 * Function name: main
 * Purpose: Main function
 * Author: Paulo Sousa
 * History/Versions: Ver F25
 * Called functions: mainReader(), mainScanner(), mainParser()
 * Parameters: Command line arguments - argc, argv
 * Return value: Status
 * Algorithm: -
 *************************************************************
 */

rigaax_intg main(int argc, char** argv) {

	rigaax_intg i;
	printLogo();
	if (DEBUG) {
		for (i = 0; i < argc; ++i)
			printf("argv[%d] = %s\n", i, argv[i]);
	}
	if (argc < 2) {
		printf("%s%c%s%c%s%c%s%c%s%c%s", "OPTIONS:\n* [",
			PGM_CDR, "] - Coder\n* [",
			PGM_RDR, "] - Reader\n* [",
			PGM_SCN, "] - Scanner\n* [",
			PGM_PSR, "] - Parser\n* [",
			PGM_WRT, "] - Writer\n");
		return EXIT_FAILURE;
	}
	rigaax_char option = argv[1][0];
	switch (option) {

	case PGM_CDR:
		printf("%s%c%s", "\n[Option '", PGM_CDR, "': Starting CODER .....]\n\n");
		main1Coder((rigaax_intg)argc, (rigaax_strg*)argv);
		break;

	case PGM_RDR:
		printf("%s%c%s", "\n[Option '", PGM_RDR, "': Starting READER .....]\n\n");
		main2Reader((rigaax_intg)argc, (rigaax_strg*)argv);
		break;

	default:
		printf("%s%c%s%c%s%c%s%c%s%c%s", "OPTIONS:\n* [",
			PGM_CDR, "] - Coder\n* [",
			PGM_RDR, "] - Reader\n* [",
			PGM_SCN, "] - Scanner\n* [",
			PGM_PSR, "] - Parser\n* [",
			PGM_WRT, "] - Writer\n");
		break;
	}
	return EXIT_SUCCESS;
}

/*
***********************************************************
* Function name: printLogo
* Purpose: Print Logo
* Author: Paulo Sousa
* History/Versions: Ver F25
* Called functions: -
* Parameters: -
* Return value: (Null)
* Algorithm: -
*************************************************************
*/

rigaax_void printLogo() {
	printf("%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s",
		"\t=---------------------------------------=\n",
		"\t|  COMPILERS - ALGONQUIN COLLEGE (W26)  |\n",
		"\t=---------------------------------------=\n",
		"\t    @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@    \n",
		"\t    @@                             @@    \n",
		"\t    @@           %&@@@@@@@@@@@     @@    \n",
		"\t    @@       @%% (@@@@@@@@@  @     @@    \n",
		"\t    @@      @& @   @ @       @     @@    \n",
		"\t    @@     @ @ %  / /   @@@@@@     @@    \n",
		"\t    @@      & @ @  @@              @@    \n",
		"\t    @@       @/ @*@ @ @   @        @@    \n",
		"\t    @@           @@@@  @@ @ @      @@    \n",
		"\t    @@            /@@    @@@ @     @@    \n",
		"\t    @@     @      / /     @@ @     @@    \n",
		"\t    @@     @ @@   /@/   @@@ @      @@    \n",
		"\t    @@     @@@@@@@@@@@@@@@         @@    \n",
		"\t    @@                             @@    \n",
		"\t    @@         R I G A A X         @@    \n",
		"\t    @@                             @@    \n",
		"\t    @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@    \n",
		"\t                                         \n",
		"\t[COMPILER SCRIPT .......................]\n",
		"\t                                         \n"
	);
}

/*
************************************************************
* Error printing function with variable number of arguments
* Params: Variable arguments, using formats from C language.
*	- Internal vars use list of arguments and types from stdarg.h
*   - NOTE: The format is using signature from C Language
************************************************************
*/

rigaax_void errorPrint(rigaax_strg fmt, ...) {
	/* Initialize variable list */
	va_list ap;
	va_start(ap, fmt);

	(rigaax_void)vfprintf(stderr, fmt, ap);
	va_end(ap);

	/* Move to new line */
	if (strchr(fmt, '\n') == NULL)
		fprintf(stderr, "\n");
}
