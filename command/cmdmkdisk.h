#ifndef CMDMKDISK_H_INCLUDED
#define CMDMKDISK_H_INCLUDED

#include "../file_manager/manager.h"
#include "../struct/parameter.h"
#include "../struct/mlist.h"
#include "../struct/sdisk.h"

/* TIPO DE PARAMETRO */
#define TP_SIZE 0
#define TP_PATH 1
#define TP_UNIT 2

/* TIPO DE DATO */
#define TD_INT 0
#define TD_CHAR 1
#define TD_STRING 2

void exec_mkdisk (MList * parameters)
{
    char * filename = NULL;
    char unit = 'm';
    int size = 0;
    MBR mbr;

    while (parameters->size > 0)
    {
        Parameter * parameter = (Parameter *)pop_front(&parameters);
        if (parameter->type == TP_PATH)
        {
            if (parameter->data_type == TD_STRING)
                filename = parameter->value;
            else
            {
                perror("Tipo de dato esperado: String\n");
                return;
            }
        }
        else if (parameter->type == TP_SIZE)
        {
            if (parameter->data_type == TD_INT)
                size = atoi(parameter->value);
            else
            {
                perror("Tipo de dato esperado: String\n");
                return;
            }
        }
        else if (parameter->type == TP_UNIT)
        {
            if (parameter->data_type == TD_CHAR)
                unit = parameter->value[0];
            else
            {
                perror("Tipo de dato esperado: Char\n");
                return;
            }
        }
    }

    if (filename == NULL)
    {
        perror("Parámetro requerido: filename\n");
        return;
    }
    if (size < 0)
    {
        perror("Parámetro requerido: +size\n");
        return;
    }

    if (unit == 'k' || unit == 'K')
        size *= KILOBYTE;
    else if (unit == 'm' || unit == 'M')
        size *= MEGABYTE;
    else
    {
        perror("Unidad no reconocida\n");
        return;
    }

    if (createFolder(filename))
        printf("[i] Folder listo\n");
    else
    {
        perror("Directorio inaccesible\n");
        return;
    }

    if (createDisk(filename, size))
        printf("[i] Disco creado\n");
    else
    {
        perror("Archivo no accesible\n");
        return;
    }
    mbr = newMBR(size);
    if (updateMBR(filename, &mbr))
        printf("MBR actualizado\n");
    else
    {
        perror("Archivo no accesible\n");
        return;
    }
}

#endif // CMDMKDISK_H_INCLUDED