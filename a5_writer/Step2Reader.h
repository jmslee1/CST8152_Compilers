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

#ifndef READER_H_
#define READER_H_

#include "Compilers.h"
#include "Step1Coder.h"   // for vigenereMem

/* Constants */
#define READER_ERROR      (-1)
#define READER_TERMINATOR '\0'

/* Function prototypes */
BufferPointer   readerCreate   (jms_intg size, jms_real factor);
BufferPointer   readerAddChar  (BufferPointer const readerPointer, jms_char ch);
jms_boln        readerClear    (BufferPointer const readerPointer);
jms_boln        readerFree     (BufferPointer const readerPointer);
jms_boln        readerIsFull   (BufferPointer const readerPointer);
jms_boln        readerIsEmpty  (BufferPointer const readerPointer);
jms_boln        readerSetMark  (BufferPointer const readerPointer, jms_intg mark);
jms_intg        readerPrint    (BufferPointer const readerPointer);
jms_intg        readerLoad     (BufferPointer const readerPointer, jms_strg fileName);
jms_boln        readerRecover  (BufferPointer const readerPointer);
jms_boln        readerRetract  (BufferPointer const readerPointer);
jms_boln        readerRestore  (BufferPointer const readerPointer);
jms_intg        readerChecksum (BufferPointer const readerPointer);

/* Getters */
jms_char        readerGetChar      (BufferPointer const readerPointer);
jms_strg        readerGetContent   (BufferPointer const readerPointer, jms_intg pos);
jms_intg        readerGetPosRead   (BufferPointer const readerPointer);
jms_intg        readerGetPosWrte   (BufferPointer const readerPointer);
jms_intg        readerGetPosMark   (BufferPointer const readerPointer);
jms_intg        readerGetSize      (BufferPointer const readerPointer);
jms_intg        readerNumErrors    (BufferPointer const readerPointer);
jms_void        readerPrintFlags   (BufferPointer const readerPointer);
jms_void        readerPrintStat    (BufferPointer const readerPointer);

#endif
