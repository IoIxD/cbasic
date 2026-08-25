#include "compiler.hpp"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Target/TargetMachine.h"
#include <iostream>
#include <llvm/ADT/SmallVector.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/TargetParser/Triple.h>
#include <sys/wait.h>

Compiler::Compiler() {
  mLLVM = std::make_unique<llvm::LLVMContext>();
  mIRBuilder = std::make_unique<llvm::IRBuilder<>>(*mLLVM);
  mModule = std::make_unique<llvm::Module>("cbasic", *mLLVM);

  auto i1 = mIRBuilder->getInt1Ty();
  auto i8 = mIRBuilder->getInt8Ty();
  auto i8_p = llvm::PointerType::get(mIRBuilder->getInt8Ty(), 0);
  auto f64 = mIRBuilder->getDoubleTy();
  auto v = mIRBuilder->getVoidTy();

#define FUNC(str, linkage, prototype)                                          \
  mFunctions.insert_or_assign(                                                 \
      str, llvm::Function::Create(prototype, linkage, str, mModule.get()));

  FUNC("puts",
       llvm::Function::ExternalLinkage,
       llvm::FunctionType::get(v, {i8_p}, false))
  FUNC("cb_clear_variables",
       llvm::Function::ExternalLinkage,
       llvm::FunctionType::get(v, false))
  FUNC("cb_variable_assign_null",
       llvm::Function::ExternalLinkage,
       llvm::FunctionType::get(v, {i8_p}, false))
  FUNC("cb_variable_assign_number",
       llvm::Function::ExternalLinkage,
       llvm::FunctionType::get(v, {i8_p, f64}, false))
  FUNC("cb_variable_assign_boolean",
       llvm::Function::ExternalLinkage,
       llvm::FunctionType::get(v, {i1}, false))
  FUNC("cb_variable_assign_string",
       llvm::Function::ExternalLinkage,
       llvm::FunctionType::get(v, {i8_p, i8_p}, false))
  FUNC("cb_eval_variable_true",
       llvm::Function::ExternalLinkage,
       llvm::FunctionType::get(i1, {i8_p}, false))
  FUNC("cb_eval_variable_false",
       llvm::Function::ExternalLinkage,
       llvm::FunctionType::get(i1, {i8_p}, false))
  FUNC("cb_eval_variable_eq",
       llvm::Function::ExternalLinkage,
       llvm::FunctionType::get(i1, {i8_p, i8_p}, false))
  FUNC("cb_eval_variable_gt",
       llvm::Function::ExternalLinkage,
       llvm::FunctionType::get(i1, {i8_p, i8_p}, false))
  FUNC("cb_eval_variable_lt",
       llvm::Function::ExternalLinkage,
       llvm::FunctionType::get(i1, {i8_p, i8_p}, false))
  FUNC("cb_eval_variable_ge",
       llvm::Function::ExternalLinkage,
       llvm::FunctionType::get(i1, {i8_p, i8_p}, false))
  FUNC("cb_eval_variable_le",
       llvm::Function::ExternalLinkage,
       llvm::FunctionType::get(i1, {i8_p, i8_p}, false))
}

static void exec(std::string str, const char *__file, char *const __argv[]) {
  int pfd[2];
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
void Compiler::finish() {
  std::error_code ec;
  // llvm::raw_fd_ostream outFile("out.ll", ec, llvm::sys::fs::OF_None);
  // mModule->print(outFile, nullptr);

  std::string v;
  llvm::raw_string_ostream outBuf(v);
  mModule->print(outBuf, nullptr);

  const char *args[] = {
      "clang", "-x", "ir", "-", "-Lruntime", "-lcbruntime", NULL};
  exec(v, "clang", (char **)args);

  // llvm::InitializeAllTargetInfos();
  // llvm::InitializeAllTargets();
  // llvm::InitializeAllTargetMCs();
  // llvm::InitializeAllAsmParsers();
  // llvm::InitializeAllAsmPrinters();

  // auto targetTriple = llvm::sys::getDefaultTargetTriple();
  // auto triple = llvm::Triple(targetTriple);
  // mModule->setTargetTriple(triple);

  // std::string error;
  // auto target = llvm::TargetRegistry::lookupTarget(triple, error);

  // llvm::TargetOptions opt;
  // auto targetMachine = target->createTargetMachine(
  //     triple, "generic", "", opt, llvm::Reloc::PIC_);

  // mModule->setDataLayout(targetMachine->createDataLayout());

  // std::error_code ec;
  // llvm::raw_fd_ostream dest("out.o", ec, llvm::sys::fs::OF_None);

  // if (llvm::verifyModule(*mModule, &llvm::errs())) {
  //   llvm::errs() << "Module verification failed!\n";
  //   // don't proceed to codegen
  // } else {
  //   llvm::legacy::PassManager pass;
  //   auto fileType = llvm::CodeGenFileType::ObjectFile;
  //   if (targetMachine->addPassesToEmitFile(pass, dest, nullptr, fileType)) {
  //     llvm::errs() << "TargetMachine can't emit this file type";
  //   }
  //   pass.run(*mModule);
  //   dest.flush();
  // }
}
