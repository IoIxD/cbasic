#include "compiler.hpp"
#include <unistd.h>
#include <sys/wait.h>
#include <string>

void Compiler::print() {
    int pfd[2];
    std::string str;
    for(auto ins : mInstructions) {
        str += ins;
        str += "\n";
    }

    pipe(pfd);

    int pid = fork();

    if(pid == 0) {
        dup2(pfd[0], STDIN_FILENO);

        close(pfd[0]);
        close(pfd[1]);

        execvp("cat", NULL);

        exit(1);
    } else {
        write(pfd[1], str.c_str(), str.size());
        close(pfd[1]);

        int status;
        waitpid(pid,&status, 0);
    }
}


void Compiler::assemble() {
    int pfd[2];
    std::string str;
    for(auto ins : mInstructions) {
        str += ins;
        str += "\n";
    }

    pipe(pfd);

    int pid = fork();

    if(pid == 0) {
        dup2(pfd[0], STDIN_FILENO);

        close(pfd[0]);
        close(pfd[1]);

        const char* args[] = { "clang", "-g", "-x", "assembler", "-", "-masm=intel", "-L./runtime", "-lcbruntime", "-o", "a.out", NULL};
        execvp(args[0], (char**)args);

        exit(1);
    } else {
        write(pfd[1], str.c_str(), str.size());
        close(pfd[1]);

        int status;
        waitpid(pid,&status, 0);
    }
}
