#include "command.h"
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <stdbool.h>
#include "console.h"

#include "gamemanager.h"

const int MaxParts = 25;

void extract_parts(char* command, size_t* count, char*** p)
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
    return;
}

void execute_command(char* command)
{
    int count;
    char** parts;
    char* repeat;
    int len = strlen(command);
    
    repeat = malloc(len + 2);
    repeat[0] = '>';
    repeat[1] = 0;
    strcat(repeat, command);
    repeat[len + 1] = 0;

    append_text(repeat);

    extract_parts(command, &count, &parts);

    if(count <= 0)
       return;

    if(strcmp(parts[0], "exit") == 0)
    {
        quit_game();
        return;
    }

    append_text("Unknown command. Use help to list commands.");
   
}
