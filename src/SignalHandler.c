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
#include <unistd.h>     // write() close() _Exit()
#include <signal.h>     // sigaction()

/// signal-safe print string to file
void writeStr(const int aFd, const char* apStr) {
    const size_t Size = strlen(apStr);
    write(aFd, (const void*)apStr, Size);
}

/// signal-safe print integer to file
void writeInt(const int aFd, const int aValue) {
    char Buffer[12];
    formatInteger(aValue, Buffer, sizeof(Buffer));
    writeStr(aFd, Buffer);
}

// signal-safe write the signal number to fil
void writeCrashReport(const int aSigNum) {
    const int Fd = open("crash_report.txt", O_WRONLY | O_CREAT | O_TRUNC);
    if (-1 != Fd) {
        writeStr(Fd, "\ncrash_report:\n");
        writeStr(Fd, "signal ");
        writeInt(Fd, aSigNum);
        writeStr(Fd, "\n\n");
        close(Fd);
    }
}

/// signal handler
void signalHandler(const int aSigNum) {
    writeCrashReport(aSigNum);
    _Exit(0); // signal-safe exit (without error as this is the normal behavior of this program)
}
#endif // __linux__

void installSignalHandler() {
 #ifdef __linux__
    struct sigaction new_action;

    /* Set up the structure to specify the new action. */
    new_action.sa_handler = signalHandler;
    sigemptyset (&new_action.sa_mask);
    new_action.sa_flags = 0;

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