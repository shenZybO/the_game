<!-- Use this file to provide workspace-specific custom instructions to Copilot. For more details, visit https://code.visualstudio.com/docs/copilot/copilot-customization#_use-a-githubcopilotinstructionsmd-file -->

This is a C++ 2D game project using raylib. Use modern C++ best practices and keep code modular.

When suggesting code using raytmx library, consider example code from https://github.com/luphi/raytmx/blob/main/example/raytmx-example.c.

If a comment spans multiple lines, use a block comment (/* ... */). Avoid chaining multiple // lines for long explanations.

Add Doxygen comments for public classes and public APIs (constructors, methods, free functions). Document public data members only when not self-explanatory. Prefer `///` for brief single-line comments and `/** ... */` for multi-line docs; include brief, details, and relevant tags (e.g., @param, @return, @throws).

When suggesting code, ensure it is compatible with C++20 standards (do not rely on C++23+ features). Assume `-std=c++20` across compilers..

Do not use one letter variable names. Use descriptive names instead. Exceptions: conventional loop indices (`i`, `j`, `k`) in small scopes and generic template parameters (`T`, `U`, `V`).

Avoid duplicate definitions and unused variables. Prefer reusing existing variables/functions; remove dead code after verifying it is unused.

When suggesting changes to existing code, do not remove or alter existing functionality unless explicitly asked. If behavior changes are necessary, clearly call them out and justify the impact.

When suggesting changes to existing code, maintain the project's existing style and conventions (follow .clang-format/.editorconfig if present).

Avoid redundant boolean comparisons and nested ternaries. Do not compare booleans to `true`/`false` (write `if (flag)` not `flag ? true : false`); prefer direct boolean expressions.

Avoid using unnecessary std::string constructions; Instead use constant string directly.