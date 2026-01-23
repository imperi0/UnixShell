#include <iostream>
#include <string>
#include <vector>
#include <cstdlib> //For exit function and for getenv
#include <unistd.h> //Contains POSIX/C functions



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

  std::vector<std::string> builtIns = {"echo","exit","type"};
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
        }
      }
    }
    else if (commands[0] == "type" && commands.size() < 2) {
      std::cout << "type: missing argument" << std::endl;
    }
    else {
      std::cout << command << ": command not found" << std::endl;
    }
  }
}

