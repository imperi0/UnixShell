#include <iostream>
#include <string>
#include <vector>
#include <cstdlib> //For exit function and for getenv
#include <unistd.h> //Contains POSIX/C functions
#include <sys/wait.h> //For wait()


std::vector<std::string> split(std::string command,char sp){

  std::vector<std::string> cmds;
  std::string cmd = "";
  
  for(char c : command){
    if(c==sp){
      if(!cmd.empty()){
        cmds.push_back(cmd);
        cmd.clear();
      }
    }
    else{
      cmd+=c;
    }
  
  }
  if (!cmd.empty())
    cmds.push_back(cmd);
  return cmds;
}

std::string pwd(){
  return "";
}

int main() {

  // Flush after every std::cout / std:cerr
  // This ensures real time outoput in console
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  std::string PATH;
  char* pathVar = getenv("PATH");
  if(pathVar != nullptr){
    PATH = pathVar;
  }

  std::vector<std::string> builtIns = {"echo","exit","type","pwd"};
  //Read eval print loop
  while (true){

    std::cout << "$ ";
    std::string command;
    std::getline(std::cin, command);

    std::vector<std::string> commands = split(command,' ');

    if (commands.empty()){
      continue;
    }

    if(commands[0] == "exit"){
      exit(0);
    } 
    else if(commands[0] == "echo"){
      for(int i=1;i<commands.size();i++){
        std::cout << commands[i];
        if(i+1<commands.size()){
          std::cout << " ";
        }
      }
      std::cout << std::endl;
    }
    else if (commands[0] == "type" && commands.size() >= 2){
      bool bIn = false;
      for(auto builtIn : builtIns){
        if(commands[1] == builtIn){
          bIn = true;
          break;
        }
      }
      if(bIn){
        std::cout << commands[1] << " is a shell builtin" << std::endl;
      }
      else{  
        auto paths = split(PATH,':');
        bool executable = false;
        for(const auto& path : paths ){
          std::string filePath = path + "/" + commands[1];
          if(access(filePath.c_str(),X_OK) == 0){
            std::cout << commands[1] << " is " << filePath << std::endl;
            executable = true;
            break;
          }
        }
        if(!executable){
          std::cout << commands[1] << ": not found" << std::endl;
          // std::perror("%d : not found", commands[1]);
        }
      }
    }
    else if (commands[0] == "type" && commands.size() < 2) {
      std::cout << "type: missing argument" << std::endl;
    }
    else if(commands[0] == "pwd"){
      if(commands.size() < 2){
        char buf[1024];
        // if(getcwd(buf, 1024)!=nullptr){ This also would work 
        char *res = (getcwd(buf, 1024));
        if(res != nullptr){
          std::cout << buf << std::endl;
        } else {
          perror("getcwd");
        }
      } else {
        std::cout << "More than required arguments !" << std::endl;
      }
    }
    else {
      std::vector<char*> argv;
      std::vector<std::vector<char>> storage;
      for(auto &cmds : commands){
        std::vector<char> s;
        for(char c: cmds){
          s.push_back(c);
        }
        s.push_back('\0');
        storage.push_back(s);
      }
      for(auto &it : storage){
        argv.push_back(it.data());
      }
      argv.push_back(nullptr);
      //This whole logic is for computing argc which is passed later to the execvp

      pid_t pid = fork();
      if(pid==0){
        execvp(argv[0],argv.data());
        std::cout << argv[0] << ": command not found\n";
	_exit(1);
      }
      else if(pid<0){
        perror("Child process not created...");
      }
      else{
        wait(nullptr);
      }
      /*else{
        std::cout << command << ": command not found" << std::endl;
      }*/ 
    }
  }
}



