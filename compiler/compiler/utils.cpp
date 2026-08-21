#include "compiler.hpp"
#include <cctype>
#include <iostream>
#include <sstream>
#include <sys/wait.h>
#include <unistd.h>

std::string Compiler::escape_string(std::string msg) {
  std::stringstream result;
  int i = 0;
  for (unsigned char ch : msg) {
    result << (int)ch;
    if (i < msg.length() - 1) {
      result << ", ";
    }
    i += 1;
  }
  return result.str();
}

std::string Compiler::translate_string(std::string msg) {
  std::stringstream result;

  for (unsigned char ch : msg) {
    if (ch == ' ') {
      // Replace spaces with underscores
      result << '_';
    } else if (ch <= 31) {
      result << "_" << std::hex << static_cast<int>(ch);
    } else if (isalpha(ch)) {
      // ASCII characters: convert to lowercase directly
      result << static_cast<char>(std::tolower(ch));
    } else {
      // Non-ASCII/Unicode bytes: format as \u00XX
      result << "_" << std::hex << static_cast<int>(ch);
    }
  }

  return result.str();
}

void Compiler::exec(const char *__file, char *const __argv[]) {
  int pfd[2];
  std::string str;
  for (auto ins : mInstructions) {
    str += ins;
    str += "\n";
  }

  pipe(pfd);

  int pid = fork();

  if (pid == 0) {
    dup2(pfd[0], STDIN_FILENO);

    close(pfd[0]);
    close(pfd[1]);

    execvp(__file, __argv);

    exit(1);
  } else {
    write(pfd[1], str.c_str(), str.size());
    close(pfd[1]);

    int status;
    waitpid(pid, &status, 0);
  }
};
