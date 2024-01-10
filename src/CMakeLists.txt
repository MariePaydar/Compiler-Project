add_executable (gsm
  GSM.cpp
  CodeGen.cpp
  Lexer.cpp
  Parser.cpp
  Sema.cpp
  Optimizer.cpp
  )
target_link_libraries(gsm PRIVATE ${llvm_libs})
