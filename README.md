<p align="center">
  <a href="" rel="noopener">
 <img width=500px height=500px src="/assets/img/Oh_my_banana.png" alt="Project logo"></a>
</p>

<h1 align="center">OH-MY-BANANA!</h1>

<div align="center">

[![Status](https://img.shields.io/badge/status-active-success.svg)]()
[![GitHub Issues](https://img.shields.io/github/issues/TsukiNi22/42sh.svg)](https://github.com/TsukiNi22/42sh/issues)
[![GitHub Pull Requests](https://img.shields.io/github/issues-pr/TsukiNi22/42sh.svg)](https://github.com/TsukiNi22/42sh/pulls)
[![License](https://img.shields.io/badge/license-XARTANIA-blue.svg)](LICENSE)

</div>

<p align="center">
  <img src="/assets/img/latte.png" width="600" />
</p>

<p align="center"> A fabulous shell with it's home made terminal !
    <br> 
</p>

[![Typing SVG](https://readme-typing-svg.herokuapp.com?font=Fira+Code&weight=700&size=22&pause=1000&color=F7077E&vCenter=true&width=435&height=30&lines=🧐About)](https://git.io/typing-svg)
---

42sh is a comprehensive terminal and shell project inspired by tcsh but enhanced with modern features. This implementation, nicknamed "OH-MY-BANANA!".

Combines a colorful terminal interface with powerful shell functionality including intelligent autocompletion, command history navigation, robust signal handling, and advanced input/output redirection. 

The project focuses on creating an intuitive user experience with features like text cursor movement, special character handling, and clear error messaging, all while maintaining the reliability of traditional shells but with a visually appealing and productivity-enhancing interface. 

[![Typing SVG](https://readme-typing-svg.herokuapp.com?font=Fira+Code&weight=700&size=22&pause=1000&color=F7077E&vCenter=true&width=435&height=30&lines=🏁Installing)](https://git.io/typing-svg)
---

To execute the program you just have to follow the instuctions bellow:

```bash
git clone git@github.com:TsukiNi22/42sh.git
cd 42sh
make sys_cmd
```

```bash
42sh
```

🚩<strong>If you need <u>HELP</u> you have the builtin</strong>:

```bash
help
```

🥵 <strong>And there is a <u>terminal</u> B.a.S.H (Banana and Shell Handler) associated with the shell, you can run it like that:</strong>

```bash
42sh -t
```
The terminal also start on it's own if the 42sh is launched not from a terminal

[![Typing SVG](https://readme-typing-svg.herokuapp.com?font=Fira+Code&weight=700&size=22&pause=1000&color=F7077E&vCenter=true&width=435&height=30&lines=Prerequisite)](https://git.io/typing-svg)
---

    - You just have to be on linux (and preferably on archlinux😎)
[![Typing SVG](https://readme-typing-svg.herokuapp.com?font=Fira+Code&weight=700&size=22&pause=1000&color=F7077E&vCenter=true&width=435&height=30&lines=Handeling)](https://git.io/typing-svg)
---

| Handle                                   | Utility |
| ---------------------------------------- | ------- |
| Binary & Builtin (see under) execution   | Can execute binary and builtin defined under |
| Nice Prompt 😎                           | Prompt ascpect:`$[return_value] exec_path➤`, if you are in a github repo `$[return_value] exec_path: branch_name➤`|
| Logic `&&` and `\|\|`                    | For `first && second` only execute the second part if the first have return `0`, for `first \|\| second` only execute the second part if the first haven't return `0` |
| Line separator `;`                       | Can separate command on the same input `command_1; commande_2` |
| Redirection (`<`, `<<`, `>`, `>>`, `\|`) | Handle the listed redirection `<`, `<<`, `>`, `>>` and `\|`. Each of them can be combined in some order |
| Quotes `""` and `''`                     | `""` as the same use as `''`, can only be used to make one paramater with space and other thing like `"a single input"` same as `'a single input'` |
| Variable replacement `$VAR`              | Replace var in the user input like `echo $TERM` where `TERM='xterm-256color'` become in the execution `echo xterm-256color` |
| Alias                                    | Handle alias setup and alias replace in the user input |
| History `~/.banana_history`              | Store the past command used with a default limits of 50.000 or with the limits stoked in `$HISTSIZE` |
| Rc `~/.bananarc`                         | Execute each line stoked in the `~/.bananarc` as a input, also handle comment on single line with `#` |
| Source                                   | Execute each line of the given file as a input, if no file given execute the `~/.bananarc` |
| Autocompletion                           | When tab (`\t`) is pressed propose possible command or file (depend on the situation) who complete the actual word |
| Line edition 🡰 🡲 🡱 🡳                   | Handle arrow to change cursor position or go back in time with the past command stored in history |
| Terminal (B.a.S.H) 😎                    | Using the `-t` option or launch the shell in a non terminal place, that will start a minimal (`$TERM='minimal'`) terminal made in CSFML |

[![Typing SVG](https://readme-typing-svg.herokuapp.com?font=Fira+Code&weight=700&size=22&pause=1000&color=F7077E&vCenter=true&width=435&height=30&lines=🎈Usage)](https://git.io/typing-svg)
---

<strong>In the 42sh only 2 principale usage:</strong>
`binary_path [argument]+` <strong>|</strong> `builtin [argument]+`


| Builtin                                                         | Utility                                                                                                                                                                 |
| --------------------------------------------------------------- | ----------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| `help`                                                          | Display the usage of the 42sh and the builtin                                                                                                                           |
| `exit [exit_code]`                                              | Exit the shell with 0 or the given exit code                                                                                                                            |
| `cd [path \| -]`                                                | Change the execution path to the given path or undo the last changement with `-`, when no argument is given automaticly change to the environement variable `HOME`      |
| `which [command \| alias \| builtin]+`                          | Display the first found utility for each parameters in order: Builtin \| Alias \| Path found in `PATH` environement variable \| Not Found                               |
| `where [command \| alias \| builtin]+`                          | Display all the founded utilitys for each parameters in order: Not Found \| Path found in `PATH` environement variable \| Alias \| Builtin                              |
| `env`                                                           | Display the actual environement                                                                                                                                         |
| `setenv [var_name \| var_name var_value]`                       | Used to setup var of the environement (do the same as `env` when no argument is given)                                                                                  |
| `unsetenv [var_name]+`                                          | Delete the given environement variable(s)                                                                                                                               |
| `set [var_name \| var_name var_value]`                          | Used to setup variable of the shell                                                                                                                                     |
| `unset [var_name]+`                                             | Deletve the given shell variable(s)                                                                                                                                     |
| `alias [alias_name \| alias_name alias_value]`                  | Setup the alias with the given argument or if no argument is given, display all actual alias                                                                            |
| `unalias [alias_name]+`                                         | Delete the given alias                                                                                                                                                  |
| `source [-c \| --create \| file_path]`                          | `source [-c \| --create]` that setup the `~/.bananarc`, `source file_path` source the given file path and if no argument is given, automaticly source the `~/.bananarc` |
| `history [number_to_diplay]`                                    | Display all or the given number of the last command used (stoked in `~/.banana_history`)                                                                                |
| `![! \| number \| string]`                                      | Execute a past command stoked in `~/.banana_history`, for `!!` take the last command maden, for `!number` take the n° command in the history and for `!string` execute the command who the start correspond to the `string` |
| `rehash`                                                        | Reload the stoked hashtable of all of the binary recuparated with the environement variable `PATH`                                                                      |
| `clear`                                                         | Clear the whome terminal printing `^[[H^[[2J` same as `\033[H\033[2J` or `\x1b[H\x1b[2J` in normal terminal, in the `-t` option it's an intern clear of line stoked     |
| `color [r_value \| r_value g_value \| r_value g_value b_value]` | Change the input color to the given value, if a value is not given it will take by default `255`                                                                        |
| `.SILENT [true \| false]`                                       | Disable or Enabled the display of some information for some builtin                                                                                                     |

[![Typing SVG](https://readme-typing-svg.herokuapp.com?font=Fira+Code&weight=700&size=22&pause=1000&color=F7077E&vCenter=true&width=435&height=30&lines=⛏️Built_using)](https://git.io/typing-svg)
---

- [C](https://en.wikipedia.org/wiki/C_(programming_language)) - Programming Language  
- [Makefile](https://en.wikipedia.org/wiki/Make_(software)) - Build Automation Tool  
- [Shell](https://en.wikipedia.org/wiki/Shell_(computing)) - Command-Line Interface  


[![Typing SVG](https://readme-typing-svg.herokuapp.com?font=Fira+Code&weight=700&size=22&pause=1000&color=F7077E&vCenter=true&width=435&height=30&lines=✍️Authors)](https://git.io/typing-svg)
---

- [@Tsukini22](https://github.com/Tsukini22)
- [@Alexy33](https://github.com/Alexy33)
- [@YetAnotherMechanicusEnjoyer](https://github.com/YetAnotherMechanicusEnjoyer)
- [@Heizmei](https://github.com/Heizmei)
- [@Thomas-B22](https://github.com/Thomas-B22)
- [@MathieuCAZAYUS](https://github.com/MathieuCAZAYUS)
