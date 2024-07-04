#include <filesystem>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using dir_iter = std::filesystem::recursive_directory_iterator;

// char key = 'b';

void encrypt (std::fstream& encryption, char key) {
    char read;
    if(encryption.is_open()){
        while(!encryption.eof()){
            read = encryption.get();
            encryption.seekp((encryption.tellp() - static_cast<std::streampos>(1)));
            encryption.put(read ^ key);
            encryption.seekp(encryption.tellp());
        }
    }
}

int check (char key){
    std::fstream encryption ("checksum.txt");
    std::stringstream buf;
    buf << encryption.rdbuf();
    std::string st = buf.str();
    encryption.close();
    for(size_t i = 0; i < st.length(); ++i){
        st[i] = st[i] ^ key;
    }
    std::cout << "st: " << st << std::endl;
    std::string exp = "checkcheckcheckycheck\n\n";
    for(size_t i = 0; i < st.length(); ++i){
        if(st[i] != exp[i]) {
            std::cout << i<< std::endl;
            return 1;
        }
    }
    return 0;
}

void help_func(){
  printf("Provide 2 args, [E]ncrypt or [D]ecrypt and the key to encrypt of decrypt against\n");
}

int main (int argc, char *argv[]){
    // std::fstream checksum ("checksum.txt");
    std::string file;
    char key;
    char inp;
    // std::string check_text;
    int flag = 0;
    int enc_de = 0;

    for(const auto& dirEntry : dir_iter(std::filesystem::current_path())){
        file = dirEntry.path().generic_string();
        if(file.find("checksum") != std::string::npos) flag = 1;
    }

    if(flag == 0){
        std::ofstream checksum ("checksum.txt");
        checksum << "checkcheckcheckycheck\n" << std::endl;

        checksum.close();
    }
    
    for(const auto& dirEntry : dir_iter(std::filesystem::current_path())){
        file = dirEntry.path().generic_string();
        if((file.find("a.exe") != std::string::npos) | (file.find("encrypt.cpp") != std::string::npos)) continue;
        std::cout << file << std::endl;
    }

    std::fstream checksum_i ("checksum.txt");

    if(argc == 3){
      inp = *argv[1];
      key = *argv[2];
    } else {
      help_func();
      return 1;
    }

    // checksum_i.open("checksum.txt");

    if((inp == 'D') && (check(key) == 0)){
        std::cout << "great!" << std::endl;
        for(const auto& dirEntry : dir_iter(std::filesystem::current_path())){
            file = dirEntry.path().generic_string();
            if((file.find("a.exe") != std::string::npos) | (file.find("encrypt.cpp") != std::string::npos)) continue;
            std::cout << file << std::endl;
            std::fstream rd (file);
            encrypt(rd, key);
        }
    }
    else if ((inp == 'E')){
        for(const auto& dirEntry : dir_iter(std::filesystem::current_path())){
            file = dirEntry.path().generic_string();
            if((file.find("a.exe") != std::string::npos) | (file.find("encrypt.cpp") != std::string::npos)) continue;
            std::cout << file << std::endl;
            std::fstream rd (file);
            encrypt(rd, key);
        }
    }
    else {
        std::cout << ":( no go" << std::endl;

    }
    return 0;
}


