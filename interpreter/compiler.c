#ifndef COMPILER_H_INCLUDED
#define COMPILER_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/*
 * Obtener tipo de comando
 */
int getCommandType(char * name)
{
    if (strcasecmp(name, "exec") == 0)
        return 0;
    else if (strcasecmp(name, "mkdisk") == 0)
        return 1;
    else if (strcasecmp(name, "rmdisk") == 0)
        return 2;
    else if (strcasecmp(name, "fdisk") == 0)
        return 3;
    else if (strcasecmp(name, "mount") == 0)
        return 4;
    else if (strcasecmp(name, "unmount") == 0)
        return 5;
    else if (strcasecmp(name, "rep") == 0)
        return 6;
    else
        return -1;
}

int validateParameter(char * param)
{
    if (strcasecmp(param, "size") == 0)
        return 0;
    else if (strcasecmp(param, "path") == 0)
        return 1;
    else if (strcasecmp(param, "unit") == 0)
        return 2;
    else if (strcasecmp(param, "name") == 0)
        return 3;
    else if (strcasecmp(param, "type") == 0)
        return 4;
    else if (strcasecmp(param, "fit") == 0)
        return 5;
    else if (strcasecmp(param, "delete") == 0)
        return 6;
    else if (strcasecmp(param, "add") == 0)
        return 7;
    else if (strcasecmp(param, "id") == 0)
        return 8;
    else
        return -1;
}

char * getValue(char * temp, int * type_value)
{
    char * value = (char *)malloc(strlen(temp));
    memset(value, 0, strlen(temp));
    strcpy(value, temp);

    if (*type_value != 0)
        *type_value = (strlen(temp) > 1) ? 2 : *type_value;

    return value;
}

void automaton(char * line)
{
    int type;
    int param;
    char * value;

    size_t TEMPSZ = sizeof(char) * 125;
    char * temp = (char *)malloc(TEMPSZ);
    memset(temp, 0, TEMPSZ);

    int step = 0; // 0-TYPE; 1-PARAMETER; 2-VALUE
    int type_value = 0; // 0-INT; 1-CHAR; 2-STRING
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
                type = getCommandType(temp);
                if (type >= 0)
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
                    type_value = (isdigit(*s)) ? 0 : 1;
            }
            if (*s == ' ')
            {
                if (quotation_marks > 0)
                    strcat(temp, "_");
                else
                {
                    value = getValue(temp, &type_value);
                    memset(temp, 0, TEMPSZ);
                    printf("Recognized value: %s, data type: %d \n", value, type_value);
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
        printf("Recognized value: %s, data type: %d \n", value, type_value);
        memset(temp, 0, TEMPSZ);
    }

    free(temp);
    temp = NULL;
}

void analizeLine(char * line)
{
    automaton(line);
}

#endif // COMPILER_H_INCLUDED