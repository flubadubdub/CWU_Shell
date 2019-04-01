/* Matt Tabert, CS 470, 8:00 AM
   This lab developed on Ubuntu 18.04.1 using the gcc compiler
   command: g++ cwushell.cpp                                   */

#include <iostream>
#include <stdlib.h>
#include <cstdlib>
#include <stdio.h>
#include <string.h>
#include <bits/stdc++.h>
#include <signal.h>

enum inputCodes{ exit_, prompt_, cpuinfo_, meminfo_, manual_, other_ };

std::string prompt = "cwushell>";
std::string command;
char *parameters;
int exitCode = 0;

void parseEntry(std::string entry);
inputCodes getInputCode(char *chrCmd);
void changePrompt(char *newPrompt);
void cpuInfoCommand(char *cmdSwitch);
void memoryInfoCommand(char* cmdSwitch);
void exitCommand(char* xc);
void systemCommand(std::string entry);
void printHelp(inputCodes ic);
void manual();

int main(){
	std::string input;
	do{
		std::cout << prompt;
		std::getline(std::cin, input);
		parseEntry(input);
	}while(true);
}

void parseEntry(std::string entry){
	//convert entry to a c string so it can be tokenized
	char* chrEntry = new char[entry.length()+1];
	strcpy(chrEntry, entry.c_str());
	//tokenize chrEntry
	//get first token from chrEntry
	char *chrCmd = strtok(chrEntry, " ");
	char *chrParameters = strtok(NULL, " ");
	//use chrCmd to getInputCode
	switch(getInputCode(chrCmd)){
		case exit_:
			exitCommand(chrParameters);
			break;
		case prompt_:
			//setting a new prompt takes in one argument
			changePrompt(chrParameters);
			break;
		case cpuinfo_:
			//which cpu info is printed is based on a single argument
			cpuInfoCommand(chrParameters);
			break;
		case meminfo_:
			memoryInfoCommand(chrParameters);
			break;
		case other_:
			systemCommand(entry);
			break;
		case manual_:
			manual();
	}
	delete[] chrEntry;
}

inputCodes getInputCode(char *chrCmd){
	std::string str(chrCmd);
	if (str == "exit")
		return exit_;
	else if (str == "prompt")
		return prompt_;
	else if (str == "cpuinfo")
		return cpuinfo_;
	else if (str == "meminfo")
		return meminfo_;
	else if (str == "manual")
		return manual_;
	else
		return other_;
}

void changePrompt(char *newPrompt){
	if(newPrompt == nullptr){
		prompt = "cwushell>";
	}
	else{
		std::string str(newPrompt);
		if(str[str.length()] != '>')
			str.append(">");
		prompt = str;
	}
}

void cpuInfoCommand(char *cmdSwitch){
	//-h: print help
	if(cmdSwitch == nullptr || cmdSwitch[1] == 'h'){
		printHelp(cpuinfo_);
	}
	//-c: print the cpu clock
	else if(cmdSwitch[1] == 'c'){
		exitCode = system("lscpu | grep \"CPU MHz\"");
	}
	//-t: print the cpu type
	else if(cmdSwitch[1] == 't'){
		exitCode = system("cat /proc/cpuinfo | grep \"model name\"");
	}
	//-n: print the number of cpu cores
	else if(cmdSwitch[1] == 'n'){
		exitCode = system("cat /proc/cpuinfo | grep \"cpu cores\"");
	}
	//incorrect input
	else{
		std::cout << "bad entry" << std::endl;
		printHelp(cpuinfo_);
		exitCode = 1;
	}
}

void memoryInfoCommand(char *cmdSwitch){
	//-h: print help
	if(cmdSwitch == nullptr || cmdSwitch[1] == 'h'){
		printHelp(meminfo_);
	}
	//-t: print the total RAM available in the system
	if(cmdSwitch[1] == 't'){
		exitCode = system("cat /proc/meminfo | grep MemTotal | awk '{print $2}'");
	}
	//-u: print the used RAM in memory
	else if(cmdSwitch[1] == 'u'){
		exitCode = system("vmstat -s | grep \"used memory\"");
	}
	//-c: print the size of the L2 cache/core in bytes
	else if(cmdSwitch[1] == 'c'){
		exitCode = system("cat /sys/devices/system/cpu/cpu0/cache/index2/size");
	}
	else{
		std::cout << "bad entry" << std::endl;
		printHelp(meminfo_);
		exitCode = 1;
	}
}

void exitCommand(char *xc){
	if(xc == nullptr)
		exit(exitCode);
	else{
		std::string str(xc);
		int enteredExitCode = std::stoi(xc, nullptr, 10);
		exit(enteredExitCode);
	}
}

void systemCommand(std::string entry){
	exitCode = system(entry.c_str());
}

void printHelp(inputCodes ic){
	std::ifstream helpFile;
	std::string line;
	if(ic == cpuinfo_){
		helpFile.open("cpuinfo.txt");
		if(helpFile.is_open()){
			while(std::getline(helpFile, line))
				std::cout << line << std::endl;
			helpFile.close();
		}
		else
			std::cout << "help file is missing" << std::endl;
	}
	else if(ic == meminfo_){
		helpFile.open("meminfo.txt");
		if(helpFile.is_open()){
			while(std::getline(helpFile, line))
				std::cout << line << std::endl;
			helpFile.close();
		}
		else
			std::cout << "help file is missing" << std::endl;
	}
}

void manual(){
	printHelp(cpuinfo_);
	printHelp(meminfo_);
}