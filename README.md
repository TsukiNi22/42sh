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

[![Typing SVG](https://readme-typing-svg.herokuapp.com?font=Fira+Code&weight=700&size=22&pause=1000&color=F7077E&vCenter=true&width=435&height=30&lines=🎈Usage)](https://git.io/typing-svg)
---

<strong>In the 42sh only 2 principale usage:</strong>
`binary_path [argument]+` <strong>|</strong> `builtin [argument]+`


| Builtin                                                         | Utility                                                                                                                                                                 |
| --------------------------------------------------------------- | ----------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| `help`                                                          | Display the usage of the 42sh and the builtin                                                                                                                           |
| `exit [exit_code]`                                              | Exit the shell with 0 or the given exit code                                                                                                                            |
| `cd [path \| -]`                                                | Change the execution path to the given path or undo the last changement with `-`, when no argument is given automaticly change to the environement variable `HOME`      |
| `env`                                                           | Display the actual environement                                                                                                                                         |
| `setenv [var_name \| var_name var_value]`                       | Used to setup var of the environement (do the same as `env` when no argument is given)                                                                                  |
| `unsetenv [var_name]+`                                          | Delete the given environement variable(s)                                                                                                                               |
| `set [var_name \| var_name var_value]`                          | Used to setup variable of the shell                                                                                                                                     |
| `unset [var_name]+`                                             | Delete the given shell variable(s)                                                                                                                                      |
| `alias [alias_name \| alias_name alias_value]`                  | Setup the alias with the given argument or if no argument is given, display all actual alias                                                                            |
| `unalias [alias_name]+`                                         | Delete the given alias                                                                                                                                                  |
| `source [-c \| --create \| file_path]`                          | `source [-c \| --create]` that setup the `~/.bananarc`, `source file_path` source the given file path and if no argument is given, automaticly source the `~/.bananarc` |
| `history [number_to_diplay]`                                    | Display all or the given number of the last command used (stoked in `~/.banana_history`)                                                                                |
| `rehash`                                                        | Reload the stoked hashtable of all of the binary recuparated with the environement variable `PATH`                                                                      |
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
