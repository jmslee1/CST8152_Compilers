/*
************************************************************
* File name: Step3Scanner.h
* Course: CST 8152 – Compilers, Lab Section: [301]
* Assignment: A3
* Date: Jul 14 2026
* Purpose: Header file for Scanner – Language "J"
************************************************************
*/

#ifndef SCANNER_H_
#define SCANNER_H_

#include "Compilers.h"
#include "Step2Reader.h"

/* --- Constants --- */
#define VID_LEN     20
#define ERR_LEN     40
#define NUM_LEN     6
#define RTE_CODE    1

#define NUM_TOKENS  13

/* --- Token Codes --- */
enum TOKENS {
    ERR_T,      /*  0: Error token */
    MNID_T,     /*  1: Method / Command identifier */
    INT_T,      /*  2: Integer literal token */
    STR_T,      /*  3: String literal token */
    LPR_T,      /*  4: Left parenthesis */
    RPR_T,      /*  5: Right parenthesis */
    LBR_T,      /*  6: Left brace */
    RBR_T,      /*  7: Right brace */
    KW_T,       /*  8: Keyword */
    EOS_T,      /*  9: End of statement */
    RTE_T,      /* 10: Run-time error */
    SEOF_T,     /* 11: Source end-of-file */
    CMT_T       /* 12: Comment */
};

/* Token string table */
static jms_strg tokenStrTable[NUM_TOKENS] = {
    "ERR_T", "MNID_T", "INT_T", "STR_T", "LPR_T", "RPR_T",
    "LBR_T", "RBR_T", "KW_T", "EOS_T", "RTE_T", "SEOF_T", "CMT_T"
};

/* Token Attribute */
typedef union TokenAttribute {
    jms_intg codeType;
    jms_intg intValue;
    jms_intg contentString;
    jms_char idLexeme[VID_LEN + 1];
    jms_char errLexeme[ERR_LEN + 1];
    jms_intg seofType;
} TokenAttribute;

/* Token Structure */
typedef struct Token {
    jms_intg     code;
    TokenAttribute attribute;
} Token;

/* Scanner Statistics */
typedef struct scannerData {
    jms_intg scanHistogram[NUM_TOKENS];
} ScannerData;

/* --- Special Characters --- */
#define EOS_CHR     '\0'
#define EOF_CHR     0xFF
#define UND_CHR     '_'
#define HST_CHR     '#'
#define TAB_CHR     '\t'
#define SPC_CHR     ' '
#define NWL_CHR     '\n'
#define SCL_CHR     ';'
#define LPR_CHR     '('
#define RPR_CHR     ')'
#define LBR_CHR     '{'
#define RBR_CHR     '}'

/* Error and Final States */
#define ESNR    8
#define ESWR    9
#define FS      10

#define NOFS    0
#define FSNR    1
#define FSWR    2

/* Transition Table */
#define NUM_STATES     10
#define CHAR_CLASSES    6

#define L   0   /* Letter or underscore */
#define D   1   /* Digit */
#define Q   2   /* Double quote */
#define C   3   /* Hash # */
#define O   4   /* Other */
#define E   5   /* End of file */

/* --- Transition Table --- */
static jms_intg transitionTable[NUM_STATES][CHAR_CLASSES] = {
    /*   L    D    Q    O    C    E   */
    {  1,   3,   5,  ESNR, ESNR, ESWR }, // S0: Start
    {  1,   1, ESNR,   2,  ESNR, FSNR }, // S1: Building identifier → go to S2 on Other
    { FS,  FS,  FS,   FS,   FS,   FS  }, // S2: Identifier complete → calls funcKEY
    {  4,   3, ESNR, ESNR, ESNR, FSNR }, // S3: Number
    { ESNR,ESNR,ESNR, ESNR, ESNR, FSNR }, // S4: Number done
    {  5,   5,   6,    5,    5, ESWR }, // S5: String
    { ESNR,ESNR,ESNR, ESNR, ESNR, FSNR }, // S6: String done
    { ESNR,ESNR,ESNR, ESNR, ESNR, FSNR }, // S7
    { ESNR,ESNR,ESNR, ESNR, ESNR, FSNR }, // S8
    { ESNR,ESNR,ESNR, ESNR, ESNR, FSNR }  // S9
};

/* State Types */
static jms_intg stateType[NUM_STATES] = {
    NOFS, NOFS, FSNR, NOFS, FSNR,   // ← S1 must be NOFS
    NOFS, FSNR, NOFS, FSNR, FSWR
};
/* --- Function Prototypes --- */
jms_intg startScanner(BufferPointer psc_buf);
jms_intg nextState(jms_intg state, jms_char c);
jms_intg nextClass(jms_char c);
jms_void printScannerData(ScannerData scData);
Token  tokenizer(jms_void);

Token funcINT(jms_strg lexeme);
Token funcSL(jms_strg lexeme);
Token funcID(jms_strg lexeme);
Token funcCMT(jms_strg lexeme);
Token funcKEY(jms_strg lexeme);
Token funcErr(jms_strg lexeme);

/* --- Final State Table --- */
typedef Token(*PTR_ACCFUN)(jms_strg lexeme);

static PTR_ACCFUN finalStateTable[NUM_STATES] = {
    NULL,      /* S0 */
    NULL,      /* S1 ← Identifier/Keyword state (final) */
    funcKEY,   /* S2 */
    NULL,      /* S3 */
    funcINT,   /* S4 */
    NULL,      /* S5 */
    funcSL,    /* S6 */
    NULL,      /* S7 */
    funcErr,   /* S8 */
    funcErr    /* S9 */
};

/* --- Keywords --- */
#define KWT_SIZE 11

static jms_strg keywordTable[KWT_SIZE] = {
    "if", "then", "else", "when", "rule",
    "enable", "disable", "set", "run", "log", "activate"
};

/* Scanner Data */
extern ScannerData scData;

#endif