# 🎯 Simple Recursive Descent Parser and Evaluator in C

Welcome to this **simple recursive descent parser** and evaluator written in C!  
It parses and evaluates a tiny language supporting:

- Variable assignment (`:=`)
- Arithmetic expressions with `+` and `*`
- Parentheses for grouping
- `write` statement to output results

---

## 🚀 Features

- 🔍 **Lexer & Parser:** Tokenizes input and parses statements & expressions recursively  
- 🗃️ **Symbol Table:** Stores and retrieves variable values  
- 🧮 **Expression Evaluation:** Computes values during parsing  
- ⚠️ **Error Handling:** Provides clear error messages on syntax errors  

---

## 💻 How to Use

1. **Rename files** with parentheses in their names to avoid shell issues:  

   ```bash
   mv "slr(1).c" parser.c


Compile the code using GCC:
gcc parser.c -o parser
Run the parser:
./parser
Modify the program being parsed by editing the input string inside the main() function.

📝 Example Program

Here’s a sample program stored in the input string:

x := 2 + 3 * 4; write x;
Expected output:

14
Parsing completed successfully.


⚠️ Notes
Filenames containing parentheses ( or ) may cause issues in some shells.
Rename or escape such files before compiling or running.

📦 Included Files

Filename	Description
slr(1).c	Original parser source code (with parentheses in filename)
parser.c	Cleaned-up and improved parser code
🙌 Contributions & Extensions

Feel free to fork this project and add:
Support for more operators (-, /, etc.)
Control flow constructs (if, while)
Functions and procedures
Enhanced error reporting

📫 Contact my linkedin to connect!
Questions or suggestions? Open an issue or reach out!
