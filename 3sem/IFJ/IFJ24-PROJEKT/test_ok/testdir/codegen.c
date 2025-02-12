/**
 * @file codegen.c
 * @brief Generator of 3 address code 
 *
 * 
 * Authors:
 * - Milan Babuljak (xbabulm00), 2024
 * - Jan Rous (xrousja00), 2024
 * - Juraj Mesík (xmesikj00), 2024
 * - Marek Chalupka (xchalu18), 2024
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Enum to represent different frame types
typedef enum {
    GLOBAL_FRAME,      // GF
    TEMPORARY_FRAME,   // TF
    LOCAL_FRAME        // LF
} FrameType;

// Convert frame type to string representation
const char* GetFrameString(FrameType frame) {
    switch (frame) {
        case GLOBAL_FRAME:     return "GF";
        case TEMPORARY_FRAME:  return "TF";
        case LOCAL_FRAME:      return "LF";
        default:               return "GF";  // Default to Global Frame
    }
}

// Flag to ensure header is printed only once
static int g_alreadyPrintedHeader = 0;

// Convert special characters in strings for IFJcode24 compatibility
char* EscapeStringForIFJcode(const char* originalStr) {
    char* escapedStr = malloc(strlen(originalStr) * 4 + 1);
    if (!escapedStr) {
        fprintf(stderr, "Memory allocation failed\n");
        return NULL;
    }

    size_t i = 0, j = 0;
    while (originalStr[i] != '\0') {
        if (originalStr[i] == ' ') {
            // Replace space with \032
            escapedStr[j++] = '\\';
            escapedStr[j++] = '0';
            escapedStr[j++] = '3';
            escapedStr[j++] = '2';
        } else {
            escapedStr[j++] = originalStr[i];
        }
        i++;
    }
    escapedStr[j] = '\0';

    return escapedStr;
}

// Print IFJcode24 header
void PrintHeader() {
    if (!g_alreadyPrintedHeader) {
        printf(".IFJcode24\n");
        g_alreadyPrintedHeader = 1;
    }
}

// Write variable value with specified frame
void WriteVariable(FrameType frame, const char* varName) {
    printf("WRITE %s@%s\n", GetFrameString(frame), varName);
}

// Create a specific type of frame
void CreateFrame(FrameType frame) {
    switch (frame) {
        case TEMPORARY_FRAME:
            printf("CREATEFRAME\n");
            break;
        case LOCAL_FRAME:
            printf("PUSHFRAME\n");
            break;
        default:
            // No action for Global Frame
            break;
    }
}

// Print string with space escaping to specified frame
void PrintString(const char* str) {
    char* escapedStr = EscapeStringForIFJcode(str);
    if (!escapedStr) return;
    printf("WRITE string@%s\n", escapedStr);
    free(escapedStr);
}

void PrintExit(int exitCode) {
    printf("EXIT int@%d\n", exitCode);
}

// Assign string to a variable in specified frame
void AssignString(FrameType frame, const char* varName, const char* value) {
    char* escapedStr = EscapeStringForIFJcode(value);
    if (!escapedStr) return;

    printf("PUSHS string@%s\n", escapedStr);
    printf("POPS %s@%s\n", GetFrameString(frame), varName);
    free(escapedStr);
}

// Define function label
void PrintFuncLabel(const char* funcName) {
    printf("LABEL %s\n", funcName);
}

// Define variable in specified frame
void DefineVariable(FrameType frame, const char* varName) {
    printf("DEFVAR %s@%s\n", GetFrameString(frame), varName);
}

// Assign integer to a variable in specified frame
void AssignInt(FrameType frame, const char* varName, int value) {
    printf("MOVE %s@%s int@%d\n", GetFrameString(frame), varName, value);
}

// Assign float to a variable in specified frame
void AssignFloat(FrameType frame, const char* varName, double value) {
    printf("MOVE %s@%s float@%f\n", GetFrameString(frame), varName, value);
}

// Integer addition of two variables in specified frame
void AddIntVarVar(FrameType frame, const char* resultVar, 
                   const char* var1, const char* var2) {
    printf("ADD %s@%s %s@%s %s@%s\n", 
           GetFrameString(frame), resultVar, 
           GetFrameString(frame), var1, 
           GetFrameString(frame), var2);
}

// Integer addition of variable and constant in specified frame
void AddIntVarInt(FrameType frame, const char* resultVar, 
                   const char* var1, int intValue) {
    printf("ADD %s@%s %s@%s int@%d\n", 
           GetFrameString(frame), resultVar, 
           GetFrameString(frame), var1, intValue);
}

// Integer subtraction in specified frame
void SubInt(FrameType frame, const char* resultVar, 
            const char* var1, const char* var2) {
    printf("SUB %s@%s %s@%s %s@%s\n", 
           GetFrameString(frame), resultVar, 
           GetFrameString(frame), var1, 
           GetFrameString(frame), var2);
}

// Integer multiplication in specified frame
void MulInt(FrameType frame, const char* resultVar, 
            const char* var1, const char* var2) {
    printf("MUL %s@%s %s@%s %s@%s\n", 
           GetFrameString(frame), resultVar, 
           GetFrameString(frame), var1, 
           GetFrameString(frame), var2);
}

// Integer division in specified frame
void DivInt(FrameType frame, const char* resultVar, 
            const char* var1, const char* var2) {
    printf("DIV %s@%s %s@%s %s@%s\n", 
           GetFrameString(frame), resultVar, 
           GetFrameString(frame), var1, 
           GetFrameString(frame), var2);
}

typedef enum {
    TYPE_INT,      // Integer input
    TYPE_FLOAT,    // Floating-point input
    TYPE_STRING,   // String input
    TYPE_BOOL      // Boolean input
} InputType;

// Convert input type to corresponding IFJcode24 type string
const char* GetInputTypeString(InputType type) {
    switch (type) {
        case TYPE_INT:     return "int";
        case TYPE_FLOAT:   return "float";
        case TYPE_STRING:  return "string";
        case TYPE_BOOL:    return "bool";
        default:           return "int";  // Default to int
    }
}

// Convert integer to ASCII character in specified frame
void ConvertIntToASCII(FrameType frame, const char* resultVar, const char* intVar) {
    printf("INT2CHAR %s@%s %s@%s\n", 
           GetFrameString(frame), resultVar, 
           GetFrameString(frame), intVar);
}

// Overloaded ReadVariable function for already defined variables
void ReadVariableInto(FrameType frame, const char* varName, InputType type, int alreadyDefined) {
    // If the variable is not already defined, define it first
    if (!alreadyDefined) {
        printf("DEFVAR %s@%s\n", GetFrameString(frame), varName);
    }
    
    // Use READ instruction with specified type
    printf("READ %s@%s %s\n", 
           GetFrameString(frame), 
           varName, 
           GetInputTypeString(type)
    );
}

// Updated ReadVariable to use the new implementation
void ReadVariable(FrameType frame, const char* varName, InputType type) {
    // Call the new function with alreadyDefined set to 0 (false)
    ReadVariableInto(frame, varName, type, 0);
}




void AddFloatVarVar(FrameType frame, const char* resultVar, const char* var1, const char* var2) {
    printf("ADD %s@%s %s@%s %s@%s\n", 
           GetFrameString(frame), resultVar, 
           GetFrameString(frame), var1, 
           GetFrameString(frame), var2);
}

// Float subtraction in specified frame
void SubFloat(FrameType frame, const char* resultVar, const char* var1, const char* var2) {
    printf("SUB %s@%s %s@%s %s@%s\n", 
           GetFrameString(frame), resultVar, 
           GetFrameString(frame), var1, 
           GetFrameString(frame), var2);
}

void PrintIf_vars(FrameType frame, const char* cmpvar1, const char* cmpvar2,  const char* iflabel, const char* elselabel, const char* operator) {
    
    if(cmpvar1 == NULL)
        cmpvar1 = "nil@nil";

    if(cmpvar2 == NULL)
        cmpvar2 = "nil@nil";

    // get frame
    const char* frameString = GetFrameString(frame);
    
    if (strcmp(operator, "=") == 0) {
        printf("JUMPIFEQ %s %s@%s %s@%s\n", iflabel,frameString  , cmpvar1,frameString, cmpvar2);
        if (elselabel != NULL) {
            printf("JUMP %s\n", elselabel);
        }
    } else if (strcmp(operator, "!=") == 0) {
        printf("JUMPIFNEQ %s %s@%s %s@%s\n", iflabel, frameString,  cmpvar1, frameString, cmpvar2);
        if (elselabel != NULL) {
            printf("JUMP %s\n", elselabel);
        }
    } else if (strcmp(operator, "<") == 0) {
        // create temp frame
        CreateFrame(TEMPORARY_FRAME);
        DefineVariable(TEMPORARY_FRAME, "tempcmpvar");
        printf("LT TF@tempcmpvar %s %s\n", cmpvar1, cmpvar2);
        printf("JUMPIFEQ %s TF@tempcmpvar bool@true\n", iflabel);
        
        if (elselabel != NULL) {
            printf("JUMP %s\n", elselabel);
        }


    } else if (strcmp(operator, "<=") == 0) {
        CreateFrame(TEMPORARY_FRAME);
        DefineVariable(TEMPORARY_FRAME, "tempcmpvar");
        printf("LT TF@tempcmpvar %s %s\n", cmpvar1, cmpvar2);
        printf("JUMPIFEQ %s TF@tempcmpvar bool@true\n", iflabel);
        printf("JUMPIFEQ %s %s %s\n", iflabel, cmpvar1, cmpvar2);
        if (elselabel != NULL) {
            printf("JUMP %s\n", elselabel);
        }

    } else if (strcmp(operator, ">") == 0) {

        CreateFrame(TEMPORARY_FRAME);
        DefineVariable(TEMPORARY_FRAME, "tempcmpvar");
        printf("GT TF@tempcmpvar %s %s\n", cmpvar1, cmpvar2);
        printf("JUMPIFEQ %s TF@tempcmpvar bool@true\n", iflabel);
        if (elselabel != NULL) {
            printf("JUMP %s\n", elselabel);
        }


    } else if (strcmp(operator, ">=") == 0) {

        CreateFrame(TEMPORARY_FRAME);
        DefineVariable(TEMPORARY_FRAME, "tempcmpvar");
        printf("GT TF@tempcmpvar %s %s\n", cmpvar1, cmpvar2);
        printf("JUMPIFEQ %s TF@tempcmpvar bool@true\n", iflabel);
        printf("JUMPIFEQ %s %s %s\n", iflabel, cmpvar1, cmpvar2);
        if (elselabel != NULL) {
            printf("JUMP %s\n", elselabel);
        }

    }
}

// Float multiplication of two variables in specified frame
void MulFloatVarVar(FrameType frame, const char* resultVar, 
                    const char* var1, const char* var2) {
    printf("MUL %s@%s %s@%s %s@%s\n", 
           GetFrameString(frame), resultVar, 
           GetFrameString(frame), var1, 
           GetFrameString(frame), var2);
}

// Float division in specified frame
void DivFloatVarVar(FrameType frame, const char* resultVar, 
                    const char* var1, const char* var2) {
    printf("DIV %s@%s %s@%s %s@%s\n", 
           GetFrameString(frame), resultVar, 
           GetFrameString(frame), var1, 
           GetFrameString(frame), var2);
}
