#include "command.h"
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <stdbool.h>
#include "console.h"

const int MaxParts = 25;

bool extract_parts(char* command, size_t* count, char*** p)
{
    size_t command_len = strlen(command);

    char** parts = malloc(sizeof(char*) * MaxParts);
    int part_index = 0;
    int index = 0;
    int start_index = 0;
    bool inquote = false;
    int part_len = 0;

    while(part_index < MaxParts && index < command_len)
    {
        if(command[index] == 0)
            break;

        if(command[index] == ' ' && start_index == index)
        {
            start_index = index + 1;
            index++;
            continue;
        }

        if(command[index] == ' ' && !inquote) 
        {
            part_len = index - start_index;
            parts[part_index] = malloc(sizeof(char) * part_len + 1);
            strncpy(parts[part_index], &command[start_index], part_len);
            parts[part_index][part_len] ='\0';

            part_index++;
            start_index = index + 1;
            index++;
            continue;
        }
        
        if(command[index] == '"')
        {
            if(inquote)
            {
                inquote = false;
                part_len = index - start_index; 
                parts[part_index] = malloc(sizeof(char) * part_len + 1);
                strncpy(parts[part_index], &command[start_index], part_len);
                parts[part_index][part_len] = 0;

                part_index++;
                start_index = index + 1;
                index++;
                continue;
            }
            else
            {
                inquote = true;
                start_index = index + 1;
            }
        }
        index++;
    }

    if(part_index == MaxParts)
    {
        printf("Overflowed options\n");
    }

    part_len = index - start_index;
    if(part_len > 0)
    {
        parts[part_index] = malloc(sizeof(char) * part_len + 1);
        strncpy(parts[part_index], &command[start_index], part_len);
        parts[part_index][part_len] = 0;
        part_index++;
    }

    *count = part_index;
    *p = parts;
    return true;
}

void execute_command(char* command)
{
   int count;
   char** parts;
    
   extract_parts(command, &count, &parts);

   for(int i = 0; i < count; i++)
   {
       append_text(parts[i]);
   }
    
}
