/*
************************************************************
* File name: Step3Scanner.c
* Course: CST 8152 – Compilers, Lab Section: [301]
* Assignment: A3
* Date: Jul 14 2026
* Purpose: Scanner implementation for Language "J"
************************************************************
*/

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <assert.h>

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef BUFFER_H_
#include "Step2Reader.h"
#endif

#ifndef SCANNER_H_
#include "Step3Scanner.h"
#endif

ScannerData scData;

/* --- Global objects --- */
extern BufferPointer stringLiteralTable;
jms_intg line;
extern jms_intg errorNumber;

extern jms_intg stateType[NUM_STATES];
extern jms_strg keywordTable[KWT_SIZE];
extern PTR_ACCFUN finalStateTable[NUM_STATES];
extern jms_intg transitionTable[NUM_STATES][CHAR_CLASSES];

static BufferPointer lexemeBuffer;
static BufferPointer sourceBuffer;

jms_intg scannerErrorCount = 0;

/* --- startScanner --- */
jms_intg startScanner(BufferPointer psc_buf) {
    scannerErrorCount = 0;
    for (jms_intg i = 0; i < NUM_TOKENS; i++)
        scData.scanHistogram[i] = 0;

    readerRecover(psc_buf);
    readerClear(stringLiteralTable);
    line = 1;
    sourceBuffer = psc_buf;
    return EXIT_SUCCESS;
}

/* --- tokenizer --- */
Token tokenizer(jms_void) {
    Token currentToken = { 0 };
    int c;
    jms_intg state = 0;
    jms_intg lexStart;
    jms_intg lexEnd;
    jms_intg lexLength;
    jms_intg i;
    jms_strg lexeme;

    lexeme = (jms_strg)malloc(VID_LEN * sizeof(jms_char));
    if (!lexeme)
        return currentToken;
    lexeme[0] = EOS_CHR;

    while (1) {
        c = readerGetChar(sourceBuffer);

        if (c < 0 || c >= NCHAR)
            return currentToken;

        state = 0;

    switch (c) {
        
        case HST_CHR:
            while ((c = readerGetChar(sourceBuffer)) != NWL_CHR &&
                c != EOS_CHR && c != EOF_CHR) {
            }
            if (c == NWL_CHR) {
                line++;
            }
            currentToken.code = CMT_T;
            scData.scanHistogram[currentToken.code]++;
            return currentToken;
        case SPC_CHR:
        case TAB_CHR:
            break;

        case NWL_CHR:
            line++;
            break;

        case SCL_CHR:
            currentToken.code = EOS_T;
            scData.scanHistogram[currentToken.code]++;
            return currentToken;

        case LPR_CHR:
            currentToken.code = LPR_T;
            scData.scanHistogram[currentToken.code]++;
            return currentToken;

        case RPR_CHR:
            currentToken.code = RPR_T;
            scData.scanHistogram[currentToken.code]++;
            return currentToken;

        case LBR_CHR:
            currentToken.code = LBR_T;
            scData.scanHistogram[currentToken.code]++;
            return currentToken;

        case RBR_CHR:
            currentToken.code = RBR_T;
            scData.scanHistogram[currentToken.code]++;
            return currentToken;

        case EOS_CHR:
            currentToken.code = SEOF_T;
            scData.scanHistogram[currentToken.code]++;
            currentToken.attribute.seofType = 0;
            return currentToken;

        case (unsigned char)EOF_CHR:
            currentToken.code = SEOF_T;
            scData.scanHistogram[currentToken.code]++;
            currentToken.attribute.seofType = 255;
            return currentToken;

/* Transition Table driven tokens (identifiers, numbers, strings) */
        default:
            state = nextState(state, c);
            lexStart = readerGetPosRead(sourceBuffer) - 1;
            readerSetMark(sourceBuffer, lexStart);

            while (stateType[state] == NOFS) {
                c = readerGetChar(sourceBuffer);
                state = nextState(state, c);
            }

            if (stateType[state] == FSWR)
                readerRetract(sourceBuffer);

            lexEnd = readerGetPosRead(sourceBuffer);
            lexLength = lexEnd - lexStart;

            lexemeBuffer = readerCreate(lexLength + 2, READER_DEFAULT_FACTOR);
            if (!lexemeBuffer) {
                fprintf(stderr, "Scanner error: Cannot create buffer\n");
                exit(1);
            }

            readerRestore(sourceBuffer);
            for (i = 0; i < lexLength; i++)
                readerAddChar(lexemeBuffer, readerGetChar(sourceBuffer));
            readerAddChar(lexemeBuffer, READER_TERMINATOR);

            lexeme = readerGetContent(lexemeBuffer, 0);
            if (!lexeme)
                return currentToken;

            currentToken = (*finalStateTable[state])(lexeme);
            readerRestore(lexemeBuffer);
            return currentToken;
        }
    }
}

/* --- nextState --- */
jms_intg nextState(jms_intg state, jms_char c) {
    jms_intg col = nextClass(c);
    jms_intg next = transitionTable[state][col];

    if (DEBUG)
        printf("Input: %c | State: %d | Col: %d | Next: %d\n", c, state, col, next);

    if (next == FS) {
        printf("Scanner Error: Illegal state transition\n");
        exit(1);
    }
    return next;
}

/* --- nextClass --- */
jms_intg nextClass(jms_char c) {
    if (c == EOS_CHR || c == EOF_CHR)
        return E;

    if (isalpha(c) || c == UND_CHR)
        return L;

    if (isdigit(c))
        return D;

    if (c == '"')
        return Q;

    return O;
}

/* --- funcINT --- */
Token funcINT(jms_strg lexeme) {
    Token currentToken = { 0 };
    jms_long tlong;

    if (strlen(lexeme) > NUM_LEN) {
        currentToken = (*finalStateTable[ESNR])(lexeme);
    }
    else {
        tlong = atol(lexeme);
        if (tlong >= 0 && tlong <= SHRT_MAX) {
            currentToken.code = INT_T;
            scData.scanHistogram[currentToken.code]++;
            currentToken.attribute.intValue = (jms_intg)tlong;
        }
        else {
            currentToken = (*finalStateTable[ESNR])(lexeme);
        }
    }
    return currentToken;
}

/* --- funcID --- */
Token funcID(jms_strg lexeme) {
    Token currentToken = { 0 };
    currentToken.code = MNID_T;
    scData.scanHistogram[currentToken.code]++;
    strncpy(currentToken.attribute.idLexeme, lexeme, VID_LEN);
    currentToken.attribute.idLexeme[VID_LEN] = EOS_CHR;
    return currentToken;
}

/* --- funcSL --- */
Token funcSL(jms_strg lexeme) {
    Token currentToken = { 0 };
    jms_intg i, len = (jms_intg)strlen(lexeme);

    currentToken.attribute.contentString = readerGetPosWrte(stringLiteralTable);

    for (i = 1; i < len - 1; i++) {
        if (lexeme[i] == NWL_CHR) line++;
        readerAddChar(stringLiteralTable, lexeme[i]);
    }
    readerAddChar(stringLiteralTable, EOS_CHR);

    currentToken.code = STR_T;
    scData.scanHistogram[currentToken.code]++;
    return currentToken;
}

/* --- funcCMT --- */
Token funcCMT(jms_strg lexeme) {
    Token currentToken = { 0 };
    jms_intg i, len = (jms_intg)strlen(lexeme);

    for (i = 1; i < len; i++) {
        if (lexeme[i] == NWL_CHR) line++;
    }
    currentToken.code = CMT_T;
    scData.scanHistogram[currentToken.code]++;
    return currentToken;
}

/* --- funcKEY --- */
Token funcKEY(jms_strg lexeme) {
    Token currentToken = { 0 };
    jms_intg kwindex = -1;

    // Clean the lexeme
    jms_char cleanLex[VID_LEN + 1];
    strncpy(cleanLex, lexeme, VID_LEN);
    cleanLex[VID_LEN] = EOS_CHR;

    // Remove trailing non-letter characters
    for (jms_intg k = strlen(cleanLex) - 1; k >= 0; k--) {
        if (!isalpha(cleanLex[k]) && cleanLex[k] != '_') {
            cleanLex[k] = EOS_CHR;
        }
        else {
            break;
        }
    }

    for (jms_intg j = 0; j < KWT_SIZE; j++) {
        if (!strcmp(cleanLex, keywordTable[j])) {
            kwindex = j;
            break;
        }
    }

    if (kwindex != -1) {
        currentToken.code = KW_T;
        scData.scanHistogram[currentToken.code]++;
        currentToken.attribute.codeType = kwindex;
    }
    else {
        currentToken = funcID(lexeme);
    }
    return currentToken;
}

/* --- funcErr --- */
Token funcErr(jms_strg lexeme) {
    scannerErrorCount++;          // Count errors

    Token currentToken = { 0 };
    jms_intg len = (jms_intg)strlen(lexeme);

    if (len > 0 && (isalpha(lexeme[0]) || lexeme[0] == '_')) {
        return funcKEY(lexeme);   // Fallback
    }

    if (len > ERR_LEN) {
        strncpy(currentToken.attribute.errLexeme, lexeme, ERR_LEN - 3);
        currentToken.attribute.errLexeme[ERR_LEN - 3] = EOS_CHR;
        strcat(currentToken.attribute.errLexeme, "...");
    }
    else {
        strcpy(currentToken.attribute.errLexeme, lexeme);
    }

    for (jms_intg i = 0; i < len; i++) {
        if (lexeme[i] == NWL_CHR) line++;
    }

    currentToken.code = ERR_T;
    scData.scanHistogram[currentToken.code]++;
    return currentToken;
}

/* --- printToken --- */
jms_void printToken(Token t) {
    extern jms_strg keywordTable[];

    switch (t.code) {
    case RTE_T:
        printf("RTE_T\t\t%s\n", t.attribute.errLexeme);
        break;
    case ERR_T:
        printf("ERR_T\t\t%s\n", t.attribute.errLexeme);
        break;
    case SEOF_T:
        printf("SEOF_T\t\t%d\n", t.attribute.seofType);
        break;
    case MNID_T:
        printf("MNID_T\t\t%s\n", t.attribute.idLexeme);
        break;
    case INT_T:
        printf("INT_T\t\t%d\n", t.attribute.intValue);
        break;
    case STR_T:
        printf("STR_T\t\t%s\n", readerGetContent(stringLiteralTable, t.attribute.contentString));
        break;
    case LPR_T:  printf("LPR_T\n"); break;
    case RPR_T:  printf("RPR_T\n"); break;
    case LBR_T:  printf("LBR_T\n"); break;
    case RBR_T:  printf("RBR_T\n"); break;
    case KW_T:
        printf("KW_T\t\t%s\n", keywordTable[t.attribute.codeType]);
        break;
    case CMT_T:
        printf("CMT_T\n");
        break;
    case EOS_T:
        printf("EOS_T\n");
        break;
    default:
        printf("Scanner error: Invalid token code: %d\n", t.code);
    }
}

/* --- printScannerData --- */
jms_void printScannerData(ScannerData scData) {
    printf("\nNumber of scanner errors: %d\n", scannerErrorCount);
    printf("----------------------------------\n");
    printf("Statistics:\n");
    printf("----------------------------------\n");
    for (jms_intg i = 0; i < NUM_TOKENS; i++) {
        if (scData.scanHistogram[i] > 0)
            printf("Token[%s] = %d\n", tokenStrTable[i], scData.scanHistogram[i]);
    }
    printf("----------------------------------\n");
}