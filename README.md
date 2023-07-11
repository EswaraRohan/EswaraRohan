# How to run

- Open your teminal
- Type `make`
- Type `./run`

## Specification 1: Display requirement

When you execute your code, a shell prompt of the following form must appear along with it.

`<username@system\_name:curr\_dir>`

## Speciﬁcation 2: Builtin commands

Implementation fo builtin commands such as `echo`, `pwd` and `cd`.

Note: 'execvp' or similar commonds should not be used.

## Specification 3: ls command

**Flags:**

-l : displays extra information regarding files

-a : display all files, including hidden files

## Speciﬁcation 4: System commands with and without arguments

**Foreground processes**: For example, executing an "emacs" command in the foreground implies that your shell will wait for this process to complete and regain control when this process exits.

**Background processes**: Any command invoked with "&" is treated as a background command. This implies that your shell will spawn that process and doesn't wait for the process to exit. It will keep taking other user commands. Whenever a new background process is started, print the PID of the newly created background process on your shell also.

## Speciﬁcation 5: pinfo command

pinfo prints the process-related info of your shell program.

**Process Status Codes:**

1. R/R+: Running
2. S/S+: Sleeping in an interruptible wait
3. Z: Zombie

## Speciﬁcation 6: Finished Background Processes

If the background process exits then the shell must display the appropriate message to the user.

## Specification 7: history

Implement a ‘history’ command which is similar to the actual history command. The default number of commands it should output is 10. The default number of commands the history should store is 20. You must overwrite the oldest commands if more than the set number of commands are entered. You should track the commands across all sessions and not just one. DO NOT store the command in history if it is the exactly same as the previously entered command.

## Specification 8: Input/Output Redirection

Using the symbols `<`, `>` and `>>`, the output of commands, usually written to stdout, can be redirected to another file, or the input taken from a file other than stdin. Both input and output redirection can be used simultaneously. Your shell should support this functionality.

Your shell should handle these cases appropriately:

- An error message should be displayed if the input file does not exist.
- The output file should be created (with permissions 0644)if it does not already exist.
- In case the output file already exists, it should be overwritten in case of > and appended to in case of >>.

## Specification 9: Command Pipelines

A pipe, identified by `|`, redirects the output of the command on the left as input to the command on the right. One or more commands can be piped as the following examples show. Your program must be able to support any number of pipes

## Specification 10: I/O Redirection within Command Pipelines

Input/output redirection can occur within command pipelines, as the examples below show. Your shell should be able to handle this.

## Specification 11: User-defined Commands

1. **jobs**: This command prints a list of all currently running background processes spawned by the shell in alphabetical order of the command name, along with their job number (a sequential number assigned by your shell), process ID, and their state, which can either be running or stopped. There may be flags specified as well. If the flag specified is -r, then print only the running processes else if the flag is -s then print the stopped processes only.

2. **sig**: Takes the job number (assigned by your shell) of a running job and sends the signal corresponding to the signal number to that process. The shell should throw an error if no job with the given number exists. For a list of signals, look up the manual entry for 'signal' on manual page 7.

3. **fg**: Brings the running or stopped background job corresponding to job number to the foreground, and changes its state to running. The shell should throw an error if no job with the given job number exists.

4. **bg**: Changes the state of a stopped background job to running (in the background). The shell should throw an error if no background job corresponding to the given job number exists, and do nothing if the job is already running in the background.

## Specification 12: Autocompletion

The `tab` key can be pressed at any time of your prompt. When you press the tab key, it will print all files which have the same starting letters as the `ls` command and then again show the same prompt. If there is only one file/folder which has the same starting characters, the prompt should be filled with that. If it's a file, a space should be placed after the file. If it’s a folder, a `/` should be put after the folder name.

## Specification 13: Signal Handling

1. **CTRL-Z** It should push any currently running foreground job into the background, and change its state from ‘running’ to ‘stopped.’ This should have no effect on the shell if there is no foreground process running.
2. **CTRL-C** It should interrupt any currently running foreground job, by sending it the SIGINT signal. This should have no effect on the shell if there is no foreground process running.
3. **CTRL-D** It should log you out of your shell, without having any effect on the actual terminal.
