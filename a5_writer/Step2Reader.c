/*
************************************************************
* COMPILERS COURSE - Algonquin College
* Code version: Spring, 2026
* Author: James
* Professors: Paulo Sousa
************************************************************
#
# ECHO "=---------------------------------------="
# ECHO "|  COMPILERS - ALGONQUIN COLLEGE (S26)  |"
# ECHO "=---------------------------------------="
# ECHO "     _           _          _      _   _    ____   "
# ECHO "    | |         | |        / \    | \ | |  / ___|  " 
# ECHO " _  | |  _____  | |       / _ \   |  \| | | |  _   "
# ECHO "| |_| | |_____| | |___   / ___ \  | |\  | | |_| |  "
# ECHO " \___/          |_____| /_/   \_\ |_| \_|  \____|  "
# ECHO "                                                   "
# ECHO "                                                   "
# ECHO "[CODER SCRIPT ....................................]"
# ECHO "                                                   "
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Compilers.h"
#include "Step1Coder.h"
#include "Step2Reader.h"

/* ==================== readerCreate ==================== */
BufferPointer readerCreate(jms_intg size, jms_real factor) {
    BufferPointer p = (BufferPointer)calloc(1, sizeof(Buffer));
    if (p == NULL) return NULL;

    if (size <= 0) {
        size = READER_DEFAULT_SIZE;
        }
        
    if (factor <= 0) {
        factor = READER_DEFAULT_FACTOR;
        }

    p->content = (jms_strg)malloc(size);
    if (p->content == NULL) {
        free(p);
        return NULL;
    }

    p->size = size;
    p->factor = factor;
    p->position.wrte = 0;
    p->position.read = 0;
    p->position.mark = 0;
    p->numReaderErrors = 0;
    p->checkSum = 0;

    p->flags.isEmpty = JAMES_TRUE;
    p->flags.isFull  = JAMES_FALSE;
    p->flags.isRead  = JAMES_FALSE;
    p->flags.isMoved = JAMES_FALSE;

    for (jms_intg i = 0; i < NCHAR; i++) p->histogram[i] = 0;

    return p;
}

/* ==================== readerAddChar ==================== */
BufferPointer readerAddChar(BufferPointer const p, jms_char ch) {
    if (p == NULL)
    {
        return NULL;
    }

    if (p->position.wrte >= p->size) {
        jms_intg newSize = (jms_intg)(p->size * (1.0 + p->factor));
        if (newSize > READER_MAX_SIZE || newSize <= 0) {
            p->numReaderErrors++;
            return NULL;
        }

        jms_strg temp = (jms_strg)realloc(p->content, newSize);
        if (temp == NULL) {
            p->numReaderErrors++;
            return NULL;
        }
        if (temp != p->content) p->flags.isMoved = JAMES_TRUE;
        p->content = temp;
        p->size = newSize;
    }

    p->content[p->position.wrte] = ch;
    p->position.wrte++;


    if ((unsigned char)ch < NCHAR) p->histogram[(unsigned char)ch]++;
    p->checkSum = (p->checkSum + (unsigned char)ch) & 0xFF;

    p->flags.isEmpty = JAMES_FALSE;
    if (p->position.wrte >= p->size) p->flags.isFull = JAMES_TRUE;

    return p;

}

/* ==================== readerLoad ==================== */
jms_intg readerLoad(BufferPointer const p, jms_strg fileName) {
    if (p == NULL || fileName == NULL) return READER_ERROR;

    jms_strg decrypted = vigenereMem(fileName, STR_LANGNAME, DECYPHER);
    if (decrypted == NULL) {
        p->numReaderErrors++;
        return READER_ERROR;
    }

	// load decrypted content into the buffer
    jms_intg count = 0;
    for (jms_intg i = 0; decrypted[i] != '\0'; i++) {
        if (readerAddChar(p, decrypted[i]) == NULL) {
            p->numReaderErrors++;
            free(decrypted);
            return READER_ERROR;
        }
        count++;
    }
    free(decrypted);
    p->flags.isRead = JAMES_FALSE;
    return count;
}

/* ==================== Other functions ==================== */
jms_boln readerClear(BufferPointer const p) {
    if (p == NULL) return JAMES_FALSE;
    p->position.wrte = p->position.read = p->position.mark = 0;
    p->flags.isEmpty = JAMES_TRUE;
    p->flags.isFull = p->flags.isRead = JAMES_FALSE;
    return JAMES_TRUE;
}

jms_boln readerFree(BufferPointer const p) {
    if (p == NULL) return JAMES_FALSE;
    if (p->content) free(p->content);
    free(p);
    return JAMES_TRUE;
}

jms_char readerGetChar(BufferPointer const p) {
    if (p == NULL || p->position.read >= p->position.wrte) {
        if (p) p->flags.isRead = JAMES_TRUE;
        return READER_TERMINATOR;
    }
    return p->content[p->position.read++];
}

jms_intg readerGetSize(BufferPointer const p)      { return (p) ? p->size : 0; }
jms_intg readerGetPosWrte(BufferPointer const p)   { return (p) ? p->position.wrte : 0; }
jms_intg readerGetPosRead(BufferPointer const p)   { return (p) ? p->position.read : 0; }
jms_intg readerGetPosMark(BufferPointer const p)   { return (p) ? p->position.mark : 0; }
jms_strg readerGetContent(BufferPointer const p, jms_intg pos) {
    return (p && pos >= 0 && pos < p->position.wrte) ? &p->content[pos] : NULL;
}

jms_boln readerIsEmpty(BufferPointer const p) { return (p) ? p->flags.isEmpty : JAMES_TRUE; }
jms_boln readerIsFull (BufferPointer const p) { return (p) ? p->flags.isFull  : JAMES_FALSE; }

jms_boln readerSetMark(BufferPointer const p, jms_intg mark) {
    if (p == NULL || mark < 0 || mark > p->position.wrte) return JAMES_FALSE;
    p->position.mark = mark;
    return JAMES_TRUE;
}

jms_boln readerRecover(BufferPointer const p) {
    if (p == NULL) return JAMES_FALSE;
    p->position.read = p->position.mark = 0;
    p->flags.isRead = JAMES_FALSE;
    return JAMES_TRUE;
}

jms_boln readerRetract(BufferPointer const p) {
    if (p == NULL || p->position.read <= 0) return JAMES_FALSE;
    p->position.read--;
    p->flags.isRead = JAMES_FALSE;
    return JAMES_TRUE;
}

jms_boln readerRestore(BufferPointer const p) {
    if (p == NULL) return JAMES_FALSE;
    p->position.read = p->position.mark;
    p->flags.isRead = JAMES_FALSE;
    return JAMES_TRUE;
}

jms_void readerPrintFlags(BufferPointer const p) {
    if (!p) return;
    printf("Flag.isEmpty = %d\nFlag.isFull = %d\nFlag.isMoved = %d\nFlag.isRead = %d\n",
           p->flags.isEmpty, p->flags.isFull, p->flags.isMoved, p->flags.isRead);
}

jms_void readerPrintStat(BufferPointer const p) {
    if (!p) return;
    for (jms_intg i = 0; i < NCHAR; i++) {
        if (p->histogram[i] > 0)
            printf("B[%c]=%d, ", i, p->histogram[i]);
    }
    printf("\n");
}

jms_intg readerNumErrors(BufferPointer const p) { return (p) ? p->numReaderErrors : 0; }

jms_intg readerChecksum(BufferPointer const p) {
    if (!p) return 0;
    p->checkSum = 0;
    for (jms_intg i = 0; i < p->position.wrte; i++) {
        p->checkSum = (p->checkSum + (unsigned char)p->content[i]) & 0xFF;
    }
    return p->checkSum;
}

jms_intg readerPrint(BufferPointer const p) {
    if (!p || p->position.wrte == 0) return 0;
    jms_intg count = 0;
    readerRecover(p);
    jms_char c;
    while ((c = readerGetChar(p)) != READER_TERMINATOR) {
        putchar(c);
        count++;
    }
    return count;
}
