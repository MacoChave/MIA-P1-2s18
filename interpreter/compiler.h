#ifndef COMPILER_H_INCLUDED
#define COMPILER_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "../struct/mlist.h"
#include "../struct/parameter.h"

/* TIPO DE COMANDO */
#define TC_EXEC 0
#define TC_MKDISK 1
#define TC_RMDISK 2
#define TC_FDISK 3
#define TC_MOUNT 4
#define TC_UNMOUNT 5
#define TC_REP 6

/* TIPO DE PARAMETRO */
#define TP_SIZE 0
#define TP_PATH 1
#define TP_UNIT 2
#define TP_NAME 3
#define TP_TYPE 4
#define TP_FIT 5
#define TP_DELETE 6
#define TP_ADD 7
#define TP_ID 8

/* TIPO DE DATO */
#define TD_INT 0
#define TD_CHAR 1
#define TD_STRING 2

/*
 * OBTENER TIPO DE COMANDO
 */
int getCommandType (char * name)
{
    if (strcasecmp(name, "exec") == 0)
        return TC_EXEC;
    else if (strcasecmp(name, "mkdisk") == 0)
        return TC_MKDISK;
    else if (strcasecmp(name, "rmdisk") == 0)
        return TC_RMDISK;
    else if (strcasecmp(name, "fdisk") == 0)
        return TC_FDISK;
    else if (strcasecmp(name, "mount") == 0)
        return TC_MOUNT;
    else if (strcasecmp(name, "unmount") == 0)
        return TC_UNMOUNT;
    else if (strcasecmp(name, "rep") == 0)
        return TC_REP;
    else
        return -1;
}

/*
 * OBTENER TIPO DE PARAMETRO
*/
int validateParameter (char * param)
{
    if (strcasecmp(param, "size") == 0)
        return TP_SIZE;
    else if (strcasecmp(param, "path") == 0)
        return TP_PATH;
    else if (strcasecmp(param, "unit") == 0)
        return TP_UNIT;
    else if (strcasecmp(param, "name") == 0)
        return TP_NAME;
    else if (strcasecmp(param, "type") == 0)
        return TP_TYPE;
    else if (strcasecmp(param, "fit") == 0)
        return TP_FIT;
    else if (strcasecmp(param, "delete") == 0)
        return TP_DELETE;
    else if (strcasecmp(param, "add") == 0)
        return TP_ADD;
    else if (strcasecmp(param, "id") == 0)
        return TP_ID;
    else
        return -1;
}

/*
 * OBTENER VALOR
*/
char * getValue (char * temp, int * type_value)
{
    char * value = (char *)malloc(strlen(temp));
    memset(value, 0, strlen(temp));
    strcpy(value, temp);

    if (*type_value != TD_INT)
        *type_value = (strlen(temp) > 1) ? TD_STRING : *type_value;
    
    if (value[strlen(value) - 1] == '\n')
        value[strlen(value) - 1] = '\0';

    return value;
}

/*
 * AUTOMATA PARA LÍNEAS DE COMANDO
*/
MList * automaton (char * line, int * cmd_type)
{
    int type;
    int param;
    char * value;

    MList * parameters = newMList();

    size_t TEMPSZ = sizeof(char) * 125;
    char * temp = (char *)malloc(TEMPSZ);
    memset(temp, 0, TEMPSZ);

    int step = 0; // 0-TYPE; 1-PARAMETER; 2-VALUE
    int type_value = TD_INT;
    int quotation_marks = -1; // FALSE < var < TRUE

    char * s = line;

    while(*s){
        if (*s == '#')
            break;

        if (step == 0)
        {
            if (*s != ' ')
            {
                sprintf(temp, "%s%c", temp, *s);
                s++;
                continue;
            }
            else
            {
                *cmd_type = getCommandType(temp);
                if (*cmd_type >= 0)
                    printf("Recognized command: %s\n", temp);
                else
                {
                    printf("Unrecognizable command: %s\n", temp);
                    break;
                }
                memset(temp, 0, TEMPSZ);
                s += 2;
                step = 1;
            }
        }
        else if (step == 1)
        {
            if (*s != '-')
            {
                sprintf(temp, "%s%c", temp, *s);
                s++;
                continue;
            }
            else
            {
                param = validateParameter(temp);
                if (param >= 0)
                    printf("Recognized param: %s\n", temp);
                else
                {
                    printf("Unrecognizable param: %s\n", temp);
                    type = -1;
                    break;
                }
                memset(temp, 0, TEMPSZ);
                s += 2;
                step = 2;
            }
        }
        else // step == 2
        {
            if (*s == '\"')
                quotation_marks *= -1;

            if (*s != ' ')
            {
                sprintf(temp, "%s%c", temp, *s);

                if (temp[strlen(temp) - 1] == 10)
                {
                    temp[strlen(temp) - 1] = '\0';
                    break;
                }
                else
                    type_value = (isdigit(*s)) ? TD_INT : TD_CHAR;
            }
            if (*s == ' ')
            {
                if (quotation_marks > 0)
                    strcat(temp, "_");
                else
                {
                    value = getValue(temp, &type_value);
                    memset(temp, 0, TEMPSZ);
                    Parameter * parameter = newParameter(param, value, type_value);
                    push_back(&parameters, parameter);
                    printf("Recognized value: %s, data type: %d \n", value, type_value);

                    free(value);
                    value = NULL;
                    s += 2;
                    step = 1;
                    continue;
                }
            }
            s++;
        }
    }

    if (strlen(temp) > 0)
    {
        value = getValue(temp, &type_value);
        Parameter * parameter = newParameter(param, value, type_value);
        push_back(&parameters, parameter);
        printf("Recognized value: %s, data type: %d \n", value, type_value);

        free(value);
        value = NULL;
        memset(temp, 0, TEMPSZ);
    }

    free(temp);
    temp = NULL;

    return parameters;
}

/*
 * EJECUTAR COMANDO
*/
void execute (int cmd_type, MList * parameters)
{
    switch (cmd_type)
    {
        case TC_MKDISK:
            printf("mkdisk\n");
            break;
        case TC_RMDISK:
            printf("rmdisk\n");
            break;
        case TC_FDISK:
            printf("fdisk\n");
            break;
        case TC_MOUNT:
            printf("mount\n");
            break;
        case TC_UNMOUNT:
            printf("unmount\n");
            break;
        case TC_REP:
            printf("rep\n");
            break;
    }

    clearMList(&parameters);
}

/*
 * ANALIZAR ARCHIVO
*/
void analizeFile (const char * filename)
{
    FILE * file;
    
    if ((file = fopen(filename, "r")) == NULL)
    {
        perror("analizeFile()\n");
        return;
    }

    while (!feof(file))
    {
        MList * parameters = NULL;
        int cmd_type = 0;
        char string[200];
        memset(string, 0, 200);
        fgets(string, 200, file);
        parameters = automaton(string, &cmd_type);
        if (parameters->size > 0 && cmd_type > 0)
            execute(cmd_type, parameters);
        
        free(parameters);
        parameters = NULL;
    }
    fclose(file);
}

/*
 * ANALIZAR LINEA DE COMANDO
*/
void analizeLine (char * line)
{
    int cmd_type = -1;
    MList * parameters = automaton(line, &cmd_type);
    
    if (parameters->size == 0)
        return;

    if (cmd_type > TC_EXEC)
        execute(cmd_type, parameters);
    else if (cmd_type == TC_EXEC)
    {
        char * filename = NULL;
        while (parameters->size > 0)
        {
            Parameter * parameter = (Parameter *)pop_front(&parameters);
            if (parameter->type == TP_PATH && parameter->data_type == TD_STRING)
            {
                filename = (char *)malloc(strlen(parameter->value) + 1);
                strcpy(filename, parameter->value);
                deleteParameter(&parameter);
                free (parameter);
                parameter = NULL;
            }
        }
        analizeFile(filename);
    }

    free(parameters);
    parameters = NULL;
}

#endif // COMPILER_H_INCLUDED