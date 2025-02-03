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

struct opts {
  bool saveLog, saveTeX;
  std::string course, instructor, location, title;
  uint8_t columns;
};

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

std::string repl(std::string str1, std::vector<int> intArr,
                 std::vector<char> condenseArr) {
  // `repl` with ability to condense forms of + and -
  std::string ret = str1;
  int count = 0;
  for (int i = 0; i < ret.length(); i++) {
    if (ret.at(i) == '%') {
      int u = intArr.at(count);
      switch (condenseArr.at(count)) {
      case 0: { // do nothing
        ret.replace(i, 1, std::to_string(u));
        break;
      }
      case 1: { // turn % = -7 into -7 and % = 7 into +7
        ret.replace(i, 1,
                    (u > 0 ? "+" : "") + (u != 0 ? std::to_string(u) : ""));
        break;
      }
      case 2: { // case 1 but additionally turn 1 into "", -1 into "-",
        if (u == 0) {
          while (str1.at(i) != '+' || str1.at(i) != '-') {
            ret.replace(i, 1, "");
          }
        } else if (u == 1) {
          ret.replace(i, 1, "+");
        } else if (u == -1) {
          ret.replace(i, 1, "-");
        } else
          ret.replace(i, 1, (u > 0 ? "+" : "") + std::to_string(u));
      }
      }
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
             std::ofstream &outprobObj, std::ofstream &outsolObj,
             const opts o) {
  std::string write =
      "\\documentclass[12pt,notitlepage]{article}\n"
      "\\usepackage{mathtools,amssymb,amsfonts,empheq,mdframed,tikz}\n\n"
      "\\makeatletter\n\\renewcommand\\@author{" +
      o.instructor + "}\\renewcommand\\@title{" + o.title +
      "}\\newcommand\\@course{" + o.course + "}\\newcommand\\@location{" +
      o.location +
      "}\\renewcommand\\maketitle{\\topskip0pt\\noindent Name: "
      "\\rule{6cm}{0.5pt} \\hfill ID: \\rule{3cm} {0.5pt} \\hfill Period: "
      "\\rule{2cm}{0.5pt}\\begin{center}\\scshape\\@location\\qquad\\@"
      "course\\\\\\@author\\\\\\textbf{\\@title}\\end{center}}"
      "\\makeatother\n\n"
      "\\begin{document}\n"
      "\\maketitle"
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
    std::string inProb, inSol;                       // Strings that enter
    int a1, a2, a3, a4, b1, b2, b3, c1, c2, d, e, f; // Short [-32768, 32767]
    bool x, y, z;                                    // Booleans

    // The Spaghetti never fails you. You fail the Spaghetti.

    switch (u.subject) {    // Subject
    case 'C': {             // -Calculus
      switch (u.unit) {     // Unit
      case 'L': {           // --Limits
        switch (u.module) { // Module
        case 110: {         // ---Limit Evaluation via Factoring lv. 1
          a1 = rn_one(rng); // \lim_{x\to -a1} (x+a1)(x+a2)/(x+a1)
          a2 = rn_one(rng); // (x + a2)
          inProb = repl(R"(\lim_{x \to %} \frac{x^2%x%}{x%})",
                        {-a1, a1 + a2, a1 * a2, a1}, {0, 2, 1, 1});
          inSol = std::to_string(a2 - a1);
          break;
        }
        case 111: {         // ---Limit Evaluation via Factoring lv. 2
          a1 = rn_one(rng); // \lim_{x\to a1} (x+a1)(x+a2)(x+a3)/(x+a1)
          a2 = rn_one(rng); // (x + a2)
          a3 = rn_one(rng); // (x + a3)
          inProb = repl(R"(\lim_{x \to %} \frac{x^3%x^2%x%}{x%})",
                        {-a1, a1 + a2 + a3, a1 * a2 + a2 * a3 + a1 * a3,
                         a1 * a2 * a3, a1},
                        {0, 2, 2, 1, 1});
          inSol = std::to_string((a2 - a1) * (a3 - a1));
          break;
        }
        case 112: {
          a1 = rn_one(rng);
          b1 = rn_one(rng);
          a2 = rn_one(rng);
          b2 = rn_one(rng);
          a3 = rn_one(rng);
          b3 = rn_one(rng);
          inProb =
              repl(R"(\lim_{x \to \frac{%}{%}} \frac{%x^3+%x^2+%x+%}{x+%})",
                   {-b1, a1, a1 * a2 * a3,
                    a1 * a2 * b3 + a1 * b2 * a3 + b1 * a2 * a3,
                    a1 * b2 * b3 + b1 * a2 * b3 + b1 * b2 * a3, b1 * b2 * b3},
                   {0, 0, 0, 2, 2, 2, 1, 1});
          inSol = std::to_string(3);
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
  outprobObj.close();
  outsolObj.close();
};

void cleanTeX(std::ofstream &logObj, std::ofstream &outprobObj,
              std::ofstream &outsolObj) {
  std::string cleaningLine;
}

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

    std::cout << "Select Modules:\n\n";

    std::ifstream currmod("currmodules.txt");
    std::string currmodLine;
    while (getline(currmod, currmodLine)) {
      std::cout << currmodLine << '\n';
    }
    /* << "List each module to be added with the format "
              << "A-B-WW,XX,YY,...;C-D-ZZ "
              << "where A is the subject letter, B is the unit letter, and
              WW, "
              << "XX, etc. being the modules. Separate different subject
              or unit "
              << "module collections by a single semicolon:\n\n"*/
    std::cout << std::endl;
    std::cin.clear();
    getline(std::cin, inp, '\n');
  } else {
    inp = argv[1];
  }

  logMsg(outlog, "Module input obtained = " + inp +
                     " and length = " + std::to_string(inp.length()));

  std::vector<moduleObj> modules = interpretModules(inp);

  logMsg(outlog,
         "Module input interp, len. = " + std::to_string(modules.size()));

  // Additional Options
  opts curropts;
  curropts.course = "AutoTeX 101";
  curropts.instructor = "AutoTeX User";
  curropts.location = "AutoTeX University";
  curropts.title = "AutoTeX Limits";
  curropts.saveLog = false;
  curropts.saveTeX = true;
  curropts.columns = 2;

  char gopt;
  while (gopt != -1) {
    switch (gopt = getopt(argc, argv, "c:i:l:t:LTC:")) {
    case -1:
      break;
    case 'c':
      curropts.course = *optarg; //
      continue;
    case 'i':
      curropts.instructor = *optarg; //
      continue;
    case 'l':
      curropts.location = *optarg; //
      continue;
    case 't':
      curropts.title = *optarg; //
      continue;
    case 'L':
      curropts.saveLog = true;
      continue;
    case 'T':
      curropts.instructor = false;
      continue;
    case 'C':
      curropts.columns = (uint8_t)(optarg[0] - '0'); //
      continue;
    }
  }

  logMsg(outlog, "Options interpreted");

  std::filesystem::create_directory("out");
  std::ofstream outprob("out/outprob.tex"), outsol("out/outsol.tex");

  logMsg(outlog, "Output files created");

  fillTeX(outlog, modules, outprob, outsol, curropts);

  logMsg(outlog, "Finished outprob.tex, outsol.tex generation");

  cleanTeX(outlog, outprob, outsol);

  logMsg(outlog, "Cleaned outprob.tex, outsol.tex");

  // system("pdflatex outprob.tex && pdflatex outsol.tex");

  // logMsg(outlog, "Finished outprob.pdf & outsol.pdf generation");
  outlog.close();

  return 0;
}
