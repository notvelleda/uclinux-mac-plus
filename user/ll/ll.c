#include <unistd.h>
#include <stdio.h>
#include <sysexits.h>

int main(int argc, char **argv) {
#if 0
    if (argc > 2) { fputs("too many args\n", stderr); return EX_USAGE; }
    if (argc > 1 && argv[1][0] == '-') { fputs("no options allowed\n", stderr); return EX_USAGE; }
    if (geteuid() != 0) { fputs("not correctly installed as setuid-root\n",stderr); return EX_UNAVAILABLE; }
    if (setuid(0)  1) p[6] = argv[1];

    /* If all goes well, execv won't return
     * (because this program will have been replaced by tcpdump) */
    execv(p0, p);
    /* if we reach here then execv must have failed */

    perror(p0);
#endif
    int i;

    char **arr = malloc(sizeof(char *) * (argc + 2));

    arr[0] = "ls";
    arr[1] = "-lah";

    for (i = 1; i < argc; i ++) {
        arr[i + 1] = argv[i];
    }

    arr[argc + 1] = NULL;

    execv("/bin/ls", arr);

    perror("/bin/ls");

    return EX_OSFILE;
}
