/**
 * @file codegen.h
 * @brief Header for codegen.c 
 *
 * 
 * Authors:
 * - Milan Babuljak (xbabulm00), 2024
 * - Jan Rous (xrousja00), 2024
 * - Juraj Mesík (xmesikj00), 2024
 * - Marek Chalupka (xchalu18), 2024
 */


#ifndef IFJCODE_UTILS_H
#define IFJCODE_UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    GLOBAL_FRAME,
    TEMPORARY_FRAME,
    LOCAL_FRAME
} FrameType;

typedef enum {
    TYPE_INT,      
    TYPE_FLOAT,   
    TYPE_STRING,   
    TYPE_BOOL      
} InputType;

const char* GetFrameString(FrameType frame);
char* EscapeStringForIFJcode(const char* originalStr);
void PrintHeader();
void WriteVariable(FrameType frame, const char* varName);
void CreateFrame(FrameType frame);
void PrintString(const char* str);
void AssignString(FrameType frame, const char* varName, const char* value);
void PrintFuncLabel(const char* funcName);
void DefineVariable(FrameType frame, const char* varName);
void AssignInt(FrameType frame, const char* varName, int value);
void AssignFloat(FrameType frame, const char* varName, double value);
void AddIntVarVar(FrameType frame, const char* resultVar, const char* var1, const char* var2);
void AddIntVarInt(FrameType frame, const char* resultVar, const char* var1, int intValue);
void SubInt(FrameType frame, const char* resultVar, const char* var1, const char* var2);
void MulInt(FrameType frame, const char* resultVar, const char* var1, const char* var2);
void DivInt(FrameType frame, const char* resultVar, const char* var1, const char* var2);
void ReadVariable(FrameType frame, const char* varName, InputType type);
void PrintVariable(FrameType frame, const char* varName);
void ReadVariableInto(FrameType frame, const char* varName, InputType type, int alreadyDefined);
void PrintIf_vars(FrameType frame, const char* cmpvar1, const char* cmpvar2,  const char* iflabel, const char* elselabel, const char* operator);
void PrintExit(int exitCode);

#endif // IFJCODE_UTILS_H
