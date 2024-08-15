#include <algorithm>
#include <bits/getopt_core.h>
#include <chrono>
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iostream>
// #include <limits>
#include <filesystem>
#include <random>
#include <string>
#include <vector>
// #include <stdlib.h>

struct moduleObj {
  char subject;
  char unit;
  uint16_t module;
  // uint8_t count;
};

union option {
  char c;
  int i;
};

std::string repl(std::string str1, std::vector<int> intArr) {
  std::string ret = str1;
  int count = 0;
  for (int i = 0; i < ret.length(); i++) {
    if (ret.at(i) == '%') {
      ret.replace(i, 1, std::to_string(intArr.at(count)));
      count++;
    }
  }
  return ret;
}

void logMsg(std::ofstream &logObj, std::string msg) {
  logObj << std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::system_clock::now().time_since_epoch())
                .count()
         << " - " << msg << '\n';
}

void fillTeX(std::ofstream &logObj, std::vector<moduleObj> moduleVec,
             std::ofstream &outprobObj, std::ofstream &outsolObj) {
  std::string write =
      "\\documentclass[12pt,notitlepage]{minimal}\n"
      "\\usepackage{mathtools,amssymb,amsfonts,empheq,mdframed}\n"
      "\\usepackage{tikz}\n\n"
      "\\begin{document}\n"
      "\\begin{enumerate}\n";
  outprobObj << write;
  outsolObj << write;
  // Generate Random Engine
  std::random_device rd;
  std::mt19937 rng(rd());
  std::uniform_int_distribution<short> rn_one(-9, 9);
  std::uniform_int_distribution<short> rn_ten(-99, 99);
  logMsg(logObj, "RNG Engine Created");
  // logMsg(outlog, "RNG Test: ")
  std::cout << "\n\nLaTeX GENERATION:\n\n";

  for (moduleObj u : moduleVec) {
    std::string moduleName =
        u.unit + std::to_string(u.subject) + std::to_string(u.module);
    logMsg(logObj, "Begin Generation: " + moduleName);
    std::cout << "Generating " << moduleName << '\n';
    std::string inProb, inSol; // Strings that enter
    int a, b, c, d, e, f;      // Short [-32768, 32767]
    bool x, y, z;              // Booleans

    // The Spaghetti never fails you. You fail the Spaghetti.

    switch (u.subject) {    // Subject
    case 'C': {             // -Calculus
      switch (u.unit) {     // Unit
      case 'L': {           // --Limits
        switch (u.module) { // Module
        case 110: {         // ---Limit Evaluation via Factoring lv. 1
          a = rn_one(rng);  // \lim_{x\to a} (x+a)/(x+a)
          b = rn_one(rng);  // (x + b)
          c = a + b;        // FOIL: Outer, Inner
          d = a * b;        // FOIL: Last
          inProb = repl(R"(\lim_{x \to -%} \frac{x^2 + %x + %}{x + %})",
                        {a, c, d, a});
          inSol = std::to_string(a);
        }
        }
      }
      }
    }
    }
    outprobObj << "\\item \\[" << inProb << "\\]\n";
    outsolObj << "\\item \\[" << inSol << "\\]\n";
    logMsg(logObj, "End Generation");
  }

  outprobObj << "\n\\end{enumerate}\n\\end{document}";
  outsolObj << "\n\\end{enumerate}\n\\end{document}";
};

void cleanTeX(std::ofstream &logObj, std::ofstream &outprobObj,
              std::ofstream &outsolObj) {}

std::vector<moduleObj> interpretModules(std::string moduleString) {
  std::vector<moduleObj> retModuleVect;
  for (int i = 4; i < moduleString.length(); i += 6) { // AB123;CD456;EF789...
    // char tempSubject = inp.at(i-4);
    // char tempUnit = inp.at(i-3);
    // uint8_t tempModule =
    // (inp.at(i-2)-48)*100 + (inp.at(i-1)-48)*10 + inp.at(i)-48
    retModuleVect.push_back(moduleObj{
        moduleString.at(i - 4), moduleString.at(i - 3),
        (uint16_t)((moduleString.at(i - 2) * 100) +
                   (moduleString.at(i - 1) * 10) + moduleString.at(i) - 5328)});
  }
  return retModuleVect;
}

int main(int argc, char *argv[]) {

  // Logging

  std::ofstream outlog("AuToTeX.log");

  logMsg(outlog, "Created Logfile");

  logMsg(outlog, "args length = " + std::to_string(argc));

  {
    std::string argString;
    for (int i = 0; i < argc; i++) {
      argString += argv[i];
      argString += " ";
    }
    logMsg(outlog, "args = " + argString);
  }

  // Module List

  std::string inp;

  if (argc == 1) {

    std::cout /* << "List each module to be added with the format "
              << "A-B-WW,XX,YY,...;C-D-ZZ "
              << "where A is the subject letter, B is the unit letter, and
              WW, "
              << "XX, etc. being the modules. Separate different subject
              or unit "
              << "module collections by a single semicolon:\n\n"*/
        << "\nSelect Modules:\n\n"
        << "~~ CALCULUS ~~ (C)\n"
        << "~ LIMITS ~ (L)\n"
        //<< "000 - Graphical Limits (*)\n"
        << "110 - Limit evaluation via Factoring (*)\n"
        << "111 - Limit evaluation via Factoring (**)\n"
        << "112 - Limit evaluation via Factoring (***)\n"
        << "113 - Limit evaluation via Factoring (***)\n"
        << "114 - Limit evaluation via Factoring (****)\n"
        << "120 - Limit evaluation via Division (*)\n"
        << "121 - Limit evaluation via Division (**)\n"
        << "122 - Limit evaluation via Division (***)\n"

        //<< "31 - Continuity at a Point (*)\n"
        //<< "32 - Continuity at a Point (**)\n"
        //<< "41 - Discontinuity Removal (*)\n"
        << std::endl;
    std::cin.clear();
    getline(std::cin, inp, '\n');
  } else {
    inp = argv[1];
  }

  logMsg(outlog, "Module input obtained = " + inp +
                     " and length = " + std::to_string(inp.length()));

  std::vector<moduleObj> modules = interpretModules(inp);

  logMsg(outlog,
         "Module input interpreted, len. = " + std::to_string(modules.size()));

  // Additional Options

  bool saveTeX = false;
  bool saveLog = false;
  uint8_t columns;

  char gopt;
  while ((gopt = getopt(argc, argv, "c:lt")) != -1) {
    switch (gopt) {
    case 'c':
      columns = *optarg; //
      break;
    case 't':
      saveTeX = true;
      break;
    case 'l':
      saveLog = true;
      break;
    }
  }

  logMsg(outlog, "Options interpreted");

  std::filesystem::create_directory("out");
  std::ofstream outprob("out/outprob.tex");
  std::ofstream outsol("out/outsol.tex");

  logMsg(outlog, "Output files created");

  /*if (!t) {

  }*/

  fillTeX(outlog, modules, outprob, outsol);

  logMsg(outlog, "Finished outprob.tex, outsol.tex generation");

  cleanTeX(outlog, outprob, outsol);

  logMsg(outlog, "Cleaned outprob.tex, outsol.tex");

  // system("pdflatex outprob.tex && pdflatex outsol.tex");

  // logMsg(outlog, "Finished outprob.pdf & outsol.pdf generation");

  return 0;
}
