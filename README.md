# AOS Assignment-2

In this assignment I have implementation custom shell in C++ using various system calls and STL.

I have read the command from input using and ```getchar()``` then split the string using ```strtok()``` then based on the first parameter respective funtion for command is executed. 

**Display requirement:**
- 

Display custom terminal and split the command using ```strtok()``` and ```;``` delimiter.


Our goal is to display the terminal which contains hosr name, user name, current working directory. I have used the below system calls:

- ```getenv()``` - Username
- ```gethostname()``` - Hostname
- ```getcwd()``` - Curent working directory 

**cd, echo, pwd**
- 

System calls/commands used for ```cd```:

- ```struct stat``` - to check if directory exits
- ```chdir()``` - system call to change directory to given path

Used two variables ```pth``` and ```prev_pth``` to to keep track of current path and previous visted path and updated them based on input command

**ls**
- 

Implementing ls.

I have used below system calls/commands for ```ls```:

- ```struct stat``` - to get file information
- ```DIR``` - to iterate through the directory
- ```chdir()``` - to change directory and print file information

I have used two flags to check for ```-a``` and ```-l``` flags and stored directory paths in a vector.

**Handling background and Foreground processes**
- 

Pushes the given foreground process to background.

I have used below commands/ system calls:

- ```getpid(), setpid(), fork(), execvp() waitpid()```

Program checks for & and pushes that program to background. We can push multiple multiple processes. The parent process is not disturbed and continues it's process as we have use ```fork()```.

**pinfo**
- 
Gives the information of the given process id.

- I have used ```/proc/<pid>/``` to get the required information for that process id.

**search**
- 

Returns true if the given file is present else false.

I have used ```struct stat``` and ```DIR``` to iterate the directories. Performmed the above operation recursively on directories until I find the file.

**Piping and I/O Redirection**
- 
Executes pipes and I/O redirection. Used ```pipe() execvp() waitpid() fork() dup2()``` system calls. 

I have tokenized the command using ```|``` delimiter and then passed each command to io redirection function. Used ```dup2()``` to manage STDIN and STDOUT.

**Signals**
- 
**CTRL-D** - Exit the terminal.

**CTRL-Z** - It force quits the foreground process

**CTRL-C** - Kills foreground process if there is any otherwise it does nothing.

**Autocomplete**
- 
Autocompletes command/files/folders based on the inputs given. Used ```flush()``` to print inplace. 
If tab is pressed then pass the command as a pipe command to pipe command. Then store the stdout in a file and read the first line for sigle tab and print the entire file if we for double tab.

**History**
- 

Used ```deque``` as a buffer to store the the commands and output the required number of command based on the input.

**Up and down arrow** - Read up and down arrow using ```getchar()``` then iterate through deque based on the input.

```cd.cpp``` - cd funtion

```cwd.cpp``` - prints current working directory

```display.cpp``` - Takes input and passes it to respective function

```echo.cpp``` - Echo function

```history.cpp``` - prints history based on inputs

```io_pipe``` - Piping and I/O redirection

```search.cpp``` - Searches for a file/folder

```string_tokenizer.cpp``` - Tokenizes the string based on delimiter

```tab.cpp``` - Implemented autocomplete logic.

```main.cpp``` - It just calls ```display.cpp``` function

```pinfo.cpp``` - Gets process info using process id.

```bg_fg``` - pushes the program to background.

**Steps to run the program**
- 

Execute the below commands in terminal.

```make```

```./a.out```