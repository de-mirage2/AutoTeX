#include <chrono>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <limits>
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

int main() {

  // Logging

  std::cout << "Save log file? (y/n; default n)\n";
  std::string tempLogDecision;
  getline(std::cin, tempLogDecision, '\n');
  bool logging = false;
  if (!tempLogDecision.empty())
    logging = tempLogDecision.at(0) == 'y' || tempLogDecision.at(0) == 'Y';
  std::cout << "Logging " << (logging ? "enabled" : "disabled");

  std::ofstream outlog("AuToTeX.log");

  logMsg(outlog, "Created Logfile");

  // Module List

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
  std::string inp;
  std::cin.clear();
  getline(std::cin, inp, '\n');

  logMsg(outlog, "Module input obtained = " + inp +
                     " and length = " + std::to_string(inp.length()));

  std::vector<moduleObj> modules;

  for (int i = 4; i < inp.length(); i += 6) { // AB123;CD456;EF789...
    // char tempSubject = inp.at(i-4);
    // char tempUnit = inp.at(i-3);
    // uint8_t tempModule =
    // (inp.at(i-2)-48)*100 + (inp.at(i-1)-48)*10 + inp.at(i)-48
    modules.push_back(
        moduleObj{inp.at(i - 4), inp.at(i - 3),
                  (uint16_t)((inp.at(i - 2) * 100) + (inp.at(i - 1) * 10) +
                             inp.at(i) - 5328)});
  }

  logMsg(outlog,
         "Module input interpreted, len. = " + std::to_string(modules.size()));

  // Additional Options

  std::cout << "\n\nAdditional Options (do not separate)\n"
            << "l\t\t\tto save the LaTeX code produced\n";
  std::string optString;
  std::cin.clear();
  getline(std::cin, optString, '\n');

  logMsg(outlog, "Options input obtained = " + optString);

  bool saveTex;
  bool modSpace;

  for (char u : optString) {
    saveTex ^= (u == 'l');
    modSpace ^= (u == 's');
  }

  logMsg(outlog, "Options interpreted");

  std::cout << "Produced LaTeX code will be "
            << (saveTex ? "saved" : "discarded") << '\n';

  std::ofstream outprob("outprob.tex");
  std::ofstream outsol("outsol.tex");

  logMsg(outlog, "Output files created");

  std::string write =
      "\\documentclass[12pt,notitlepage]{minimal}\n"
      "\\usepackage{mathtools,amssymb,amsfonts,empheq,mdframed}\n"
      "\\usepackage{tikz}\n\n"
      "\\begin{document}\n"
      "\\begin{enumerate}\n";
  outprob << write;
  outsol << write;

  // Generate Random Engine
  std::random_device rd;
  std::mt19937 rng(rd());
  std::uniform_int_distribution<short> rn_one(-9, 9);
  std::uniform_int_distribution<short> rn_ten(-99, 99);
  logMsg(outlog, "RNG Engine Created");

  std::cout << "\n\nLaTeX GENERATION:\n\n";

  // Problem Generation
  for (moduleObj u : modules) {
    std::cout << "Generating " << u.subject << u.unit
              << std::to_string(u.module);
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
          for (int i = 0; i < 3; i++) {
            a = rn_one(rng); // \lim_{x\to a} (x+a)/(x+a)
            b = rn_one(rng); // (x + b)
            c = a + b;       // FOIL: Outer, Inner
            d = a * b;       // FOIL: Last
            inProb = repl(R"(\lim_{x \to -%} \frac{x^2 + %x + %}{x + %})",
                          {a, c, d, a});
            inSol = std::to_string(a);
          }
        }
        }
      }
      }
    }
    }
    outprob << "\\item \\[" << inProb << "\\]\n";
    outsol << "\\item \\[" << inSol << "\\]\n";
  }

  outprob << "\n\\end{enumerate}\n\\end{document}";
  outsol << "\n\\end{enumerate}\n\\end{document}";
  return 0;
}
