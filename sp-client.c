
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>

const unsigned short STRETCH_SIZE = 10;

void spc_help(){
	
	printf("silverclient prompt\n");
	printf("\n");
	printf("  commands:\n");
	printf("    pingSites [domain_x] [domain_y] [domain_z]\n");
	printf("      - create a request to test the ping of one or more domains\n");
	printf("      - at least one domain name is required\n");
	printf("    showHandles\n");
	printf("      - print unique handle of all domain requests\n");
	printf("    showHandleStatus [handle]\n");
	printf("      - print status of a handle's request\n");
	printf("      - if no handle is supplied, prints status of all requests\n");
	printf("    exit\n");
	printf("      - exits the prompt\n");
	printf("    help\n");
	printf("      - shows this help text\n");
	
}

char * spc_scanf(char *dest, int *used, int *length){
	
	while(true){
		// check if the buffer can fit another 20 chars
		// realloc the buffer if it can't
		if(*used >= *length - STRETCH_SIZE){
			*length += STRETCH_SIZE;
			dest = realloc(dest, *length);
			memset(dest + (*length-STRETCH_SIZE), 0x00, STRETCH_SIZE);
			if(errno == ENOMEM){
				//realloc failed
				free(dest);
				dest = NULL;
				return dest;
			}
		}
		
		// fill temporary buffer
		// fgets adds an extra '\0' at the end, pad by one extra char
		char src[STRETCH_SIZE + 1];
		fgets(src, sizeof(src), stdin);
		
		// copy from temporary buffer to command buffer,
		// as long as it's not an endline
		for(int i = 0; i < STRETCH_SIZE; i++){
			*(dest + *used + i) = src[i];
			if(src[i] == '\n')
				return dest;
		}
		
		// updated used size if endline wasn't found
		*used += STRETCH_SIZE;
	}
	
	//this will never be reached
	free(dest);
	dest = NULL;
	return dest;
}

int main(void){
	
	bool exit = false;
	
	char * cmd_buffer = NULL;
	int cmd_buffer_length = STRETCH_SIZE + 1;
	int cmd_buffer_used = 0;
	cmd_buffer = calloc(cmd_buffer_length, sizeof(char));
	if(errno == ENOMEM){
		//alloc failed
		printf("Failed to allocate memory\n");
		return 1;
	}
	
	spc_help();
	
	while(!exit){
		
		printf("> ");
		
		// safely get keyboard input into a stretchy buffer
		cmd_buffer = spc_scanf(cmd_buffer, &cmd_buffer_used, &cmd_buffer_length);
		if (cmd_buffer == NULL) {
			printf("Failed to allocate additional memory\n");
			return 1;
		}
		
		printf("%s\n", cmd_buffer);
		
		// clear cmd_buffer for next use
		cmd_buffer_used = 0;
		memset(cmd_buffer, 0x00, cmd_buffer_length);
		
		// fake exit
		exit = true;
		
	}
	
	if(cmd_buffer != NULL)
		free(cmd_buffer);
	
	return 0;	
}
