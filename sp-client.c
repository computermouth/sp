
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include <assert.h>

// external dependencies
#include <nanomsg/nn.h>
#include <nanomsg/reqrep.h>

const unsigned short STRETCH_SIZE = 10;

const char two_plus_two[] = "2+2";
const char thr_plus_thr[] = "3+3";

int nn_msg ( void )
{
  int sz_two_plus_two = strlen(two_plus_two) + 1; // '\0' too
  int sz_thr_plus_thr = strlen(thr_plus_thr) + 1; // '\0' too
  char *buf = NULL;
  int bytes = -1;
  int sock = nn_socket (AF_SP, NN_REQ);
  assert (sock >= 0);
  assert (nn_connect (sock, "tcp://127.0.0.1:5555") >= 0);
  
  bytes = nn_send (sock, two_plus_two, sz_two_plus_two, 0);
  assert (bytes == sz_two_plus_two);
  bytes = nn_recv (sock, &buf, NN_MSG, 0);
  assert (bytes >= 0);
  printf("%s\n", buf);
  nn_freemsg (buf);
  
  bytes = nn_send (sock, thr_plus_thr, sz_thr_plus_thr, 0);
  assert (bytes == sz_thr_plus_thr);
  bytes = nn_recv (sock, &buf, NN_MSG, 0);
  assert (bytes >= 0);
  printf("%s\n", buf);
  nn_freemsg (buf);
  
  return nn_shutdown (sock, 0);
}

void spc_help(){
	
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
	printf("\n");
	
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
			if(src[i] == '\n'){
				// end of entry
				// manually cap with space and terminator
				// space is used for argument chunking later
				*(dest + *used + i) = '\0';
				*used += i + 1;
				return dest;
			}
		}
		
		// updated used size if endline wasn't found
		*used += STRETCH_SIZE;
	}
	
	//this will never be reached
	free(dest);
	dest = NULL;
	return dest;
}

int spc_ping_sites (char * cmd_buffer) {
	
	
	
	return 0;
}

int spc_parse_cmd (char * cmd_buffer) {
	
	char * tokenized = NULL;
	
	// split any arguments by spaces
	tokenized = strtok(cmd_buffer, " ");
	
	while (tokenized != NULL){
		
		if ( strncmp(tokenized, "pingSites", sizeof("pingSites")) == 0 ) {
			printf("[spc_pingsites]\n");
			return 0;
		} else if ( strncmp(tokenized, "showHandles", sizeof("showHandles")) == 0 ) {
			printf("[spc_showhandles]\n");
			return 0;
		} else if ( strncmp(tokenized, "showHandleStatus", sizeof("showHandleStatus")) == 0 ) {
			printf("[spc_showhandlestatus]\n");
			return 0;
		} else if ( strncmp(tokenized, "exit", sizeof("exit")) == 0 ){
			printf("quitting..\n");
			return 1;
		} else if ( strncmp(tokenized, "help", 5) == 0 ) {
			printf("\n");
			spc_help();
			return 0;
		} else {
			// help text catch-all
			printf("\n  Invalid command '%s'\n\n", tokenized);
			spc_help();
			return 0;
		}
		
	}
	
	// user hit Return at prompt
	printf("Please enter a command:\n");
	spc_help();
	return 0;
	
}

int main(void){
	
	bool exit = false;
	
	// create an input buffer that will grow if neccessary
	// mostly just to avoid overflows
	char * cmd_buffer = NULL;
	int cmd_buffer_length = STRETCH_SIZE + 1;
	int cmd_buffer_used = 0;
	cmd_buffer = calloc(cmd_buffer_length, sizeof(char));
	if(errno == ENOMEM){
		//alloc failed
		printf("Failed to allocate memory\n");
		return 1;
	}
	
	printf("\nsilverclient prompt\n");
	printf("\n");
	spc_help();
	
	while(!exit){
		
		printf("spsh ~> ");
		
		// safely get keyboard input into a stretchy buffer
		cmd_buffer = spc_scanf(cmd_buffer, &cmd_buffer_used, &cmd_buffer_length);
		if (cmd_buffer == NULL) {
			printf("Failed to allocate additional memory\n");
			return 1;
		}
		
		// limit size to server's input buffer size
		// parse commands if under the limit
		if ( cmd_buffer_used > 2048){
			printf("\nThat's too much information to request.\n");
			printf("Limit requests to 2000 characters\n\n");
		} else if ( spc_parse_cmd(cmd_buffer) == 1 ) {
			// 'exit' was entered
			exit = true;
		}
		
		// clear cmd_buffer for next use
		cmd_buffer_used = 0;
		memset(cmd_buffer, 0x00, cmd_buffer_length);
		
	}
	
	if(cmd_buffer != NULL)
		free(cmd_buffer);
	
	return 0;	
}
