#ifndef CMDRMDISK_H_INCLUDED
#define CMDRMDISK_H_INCLUDED

#include "../file_manager/manager.h"
#include "../struct/parameter.h"
#include "../struct/mlist.h"
#include "../struct/sdisk.h"

/* TIPO DE PARAMETRO */
#define TP_PATH 1

/* TIPO DE DATO */
#define TD_INT 0
#define TD_CHAR 1
#define TD_STRING 2

void exec_rmdisk (MList * parameters)
{
    char * filename;

    while (parameters->size > 0)
    {
        Parameter * parameter = (Parameter *) pop_front(&parameters);
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

        if (filename == NULL)
        {
            perror("Parámetro requerido: filename\n");
            return;
        }

        if (checkDisk(filename))
        {
            perror("Archivo no accesible\n");
            return;
        }
        if (deleteDisk(filename))
            printf("Disco eliminado\n");
        else
            perror("Disco no pudo eliminarse\n");
    }
}

#endif // CMDRMDISK_H_INCLUDED