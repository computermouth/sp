
#include <stdio.h>
#include <string.h>
#include <assert.h>

// external dependencies
#include <nanomsg/nn.h>
#include <nanomsg/reqrep.h>

const char four[] = "4";
const char six[] = "6";

int main (const int argc, const char **argv)
{
	
  int sz_four = strlen(four) + 1; // '\0' too
  int sz_six = strlen(six) + 1; // '\0' too
  int sock = nn_socket (AF_SP, NN_REP);
  assert (sock >= 0);
  assert (nn_bind (sock, "tcp://127.0.0.1:5555") >= 0);
  while (1)
    {
      char *buf = NULL;
      int bytes = nn_recv (sock, &buf, NN_MSG, 0);
      assert (bytes >= 0);
      if (strncmp ("2+2", buf, sizeof("2+2")) == 0)
        {
          printf ("two_plus_two\n");
          bytes = nn_send (sock, four, sz_four, 0);
          assert (bytes == sz_four);
      } else if (strncmp ("3+3", buf, sizeof("3+3")) == 0){
          printf ("thr_plus_thr\n");
          bytes = nn_send (sock, six, sz_six, 0);
          assert (bytes == sz_six);
      }
      nn_freemsg (buf);
    }
  return nn_shutdown (sock, 0);
  
}
