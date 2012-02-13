#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sysexits.h>
#include <unistd.h>

void handler(int sig, siginfo_t * siginfo, void * vctx) {
        fprintf(
                stderr,
                "\n\n\nRECIBÍ LA SEÑAL %d!\n\n"
                "si_signo   == %d\n"
                "si_errno   == %d\n"
                "si_code    == %d\n"
                "si_pid     == %d\n"
                "si_uid     == %d\n"
                "si_status  == %d\n"
                "si_utime   == %d\n"
                "si_stime   == %d\n"
                "si_int     == %d\n"
                "si_ptr     == %p\n"
                "si_overrun == %d\n"
                "si_timerid == %d\n"
                "si_addr    == %p\n"
                "si_band    == %ld\n"
                "si_fd      == %d\n",
                sig,
                siginfo->si_signo,
                siginfo->si_errno,
                siginfo->si_code,
                siginfo->si_pid,
                siginfo->si_uid,
                siginfo->si_status,
                (int)siginfo->si_utime,
                (int)siginfo->si_stime,
                siginfo->si_int,
                siginfo->si_ptr,
                siginfo->si_overrun,
                siginfo->si_timerid,
                siginfo->si_addr,
                siginfo->si_band,
                siginfo->si_fd
        );
}

int main() {
        int i;

        struct sigaction act;
        act.sa_flags = SA_RESTART | SA_SIGINFO;
        act.sa_sigaction = &handler;

        for (i = 1; i <= 31; ++i) {
                // Estas señales no se pueden manejar.
                if (i == SIGKILL || i == SIGSTOP) continue;

                if (sigaction(i, &act, NULL) != 0) {
                        fprintf(stderr, "No se pudo instalar el manejador para la señal %d; sigaction: ", i);
                        perror(NULL);
                }
        }

        while (1) pause();
}
