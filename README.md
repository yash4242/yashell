# **Y**et **A**nother **SH**ell

## Assumptions
- max length of username is 32 chars
- max number of arguments + flags in a command is 10
- max length of hostname is 64
- max length of a path is 256
- max children spawned from the process is 16 (to simplify the program, although it is easily scalable by using linked lists)
- max length of a command is 256 chars
- all these can be changed at a single place and will reflect throughout the code

## Features
- Shows a prompt, which has username, hostname and the path, relative to the directory where the shell executable is located.
- `cd`
    - `cd` and `cd ~` change the cwd to the directory where the executable lives
- `ls` 
    - recognizes `-l` and `-a` flags
- `echo`, `pwd` implemented
- I/O redirection:
    - `>`
    - `>>`
    - `<`
    - one of `>` and `>>` is allowed. If both are passed, `>` is given priority and `>>` will be ignored.
- Piping
- Piping+redirection
    - caution: if multiple commands are piped, input redirection is allowed only in the first command, and one of output/append is allowed in the last piped command. No other scenario is allowed wrt redirection-within-piping.
- Foreground and Background process launching, zombie process handling and reaping
- Self implemented commands:
    - `pinfo`:  prints info about a process
    - `repeat`: repeats whatever was given as arguments
    - `job`: recognises `-r` and `-s` flags, lists jobs which are running and sleeping respectively. These jobs are sorted in lexicographical order
    - `sig`: send a signal to any job that was launched via **YAShell**
    - `fg`: brings the running or stopped background job to the foreground and changes its state to running.
    - `bg`: makes a stopped background job to running background job
- Signal handling:
    - Interrupt signal (<kbd>Ctrl</kbd> + <kbd>C</kbd>) to a forground job. Does **not** kill **YAShell**.
    - Stop Signal (<kbd>Ctrl</kbd> + <kbd>Z</kbd>) pushes a foreground job to background and pauses its execution
- Exiting the shell:
    - <kbd>Ctrl</kbd> + <kbd>D</kbd> logs you out and bash/zsh/fish/whateversh comes back    

- Return status: like the usual Bash/zsh shell prompt, return status of the previous command is shown. `:-)` in green if it returned 0, `:-(` in red otherwise.

## Execution
`make shell` will give an executable `shell`
in your terminal, run `./shell`

To delete the executable, `make clean`