/**
 * @file    SignalHandler.c
 * @ingroup CrashHandler
 * @brief   A basic Linux crash signal handler in C.
 *
 * Copyright (c) 2018 Sebastien Rombauts (sebastien.rombauts@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
*/
#include "SignalHandler.h"
#include "Format.h"

#include <fcntl.h>      // open()
#include <string.h>     // strlen()

#ifdef __linux__
#include <unistd.h>     // write() close(
#include <signal.h>     // sigaction()
#include <time.h>       // clock_gettime()

/// signal-safe print string to file
void writeStr(const int aFd, const char* apStr) {
    const size_t Size = strlen(apStr); // signal-safe
    const size_t Ret = write(aFd, (const void*)apStr, Size); // signal-safe
    (void)Ret;
}

/// signal-safe print 64-bit integer to file
void writeInt(const int aFd, const long long aValue) {
    char Buffer[21];
    formatInteger(aValue, Buffer, sizeof(Buffer)); // signal-safe
    writeStr(aFd, Buffer);
}

// signal-safe write the signal number to file
void writeCrashReport(const int aSigNum) {
    const int Fd = open("crash_report.txt", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH); // signal-safe
    if (-1 != Fd) {
        struct timespec TsNow;
        const int Res = clock_gettime (CLOCK_REALTIME, &TsNow); // signal-safe
        writeStr(Fd, "crash_report:\n");
        if (0 == Res) {
            writeInt(Fd, TsNow.tv_sec);
            writeStr(Fd, ".");
            writeInt(Fd, TsNow.tv_nsec);
            writeStr(Fd, " ");
        }
        writeStr(Fd, "signal ");
        writeInt(Fd, aSigNum);
        writeStr(Fd, "\n\n");
        close(Fd); // signal-safe
    }
}

/// signal handler
void signalHandler(const int aSigNum) {
    // Avoid signal handler recursion
    static sig_atomic_t _bFirstCall = 1;
    if (_bFirstCall) {
        _bFirstCall = 0;

        writeCrashReport(aSigNum);
    }

    // Resume default behavior for the signal to exit without calling back signalHandler()
    // Raise it to get a core, with gdb pointing directly at the right thread, and also return the right exit code.
    signal(aSigNum, SIG_DFL);
    raise(aSigNum);
}
#endif // __linux__

void installSignalHandler() {
 #ifdef __linux__
    struct sigaction new_action;

    /* Set up the structure to specify the new action. */
    new_action.sa_handler = signalHandler;
    sigemptyset (&new_action.sa_mask);
    new_action.sa_flags = SA_ONSTACK; // Use dedicated alternate signal stack

    sigaction (SIGHUP, &new_action, NULL);  // 1
    sigaction (SIGINT, &new_action, NULL);  // 2
    sigaction (SIGQUIT, &new_action, NULL); // 3
    sigaction (SIGILL, &new_action, NULL);  // 4
    sigaction (SIGTRAP, &new_action, NULL); // 5
    sigaction (SIGABRT, &new_action, NULL); // 6
    sigaction (SIGFPE, &new_action, NULL);  // 8
    sigaction (SIGSEGV, &new_action, NULL); // 11
    sigaction (SIGTERM, &new_action, NULL); // 15

#endif // __linux__
}
