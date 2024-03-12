/*
Name: Justin Barfield
Class: 3341-001
Date: 3/4/24
File Name: 341HW2.cpp
Description: This program reads arithmetic operations and operands from an input file, performs the operations, and sends the results back to the parent process through pipes for communication.
*/

#include <iostream>
#include <fstream>
#include <unistd.h>
#include <sys/wait.h>

using namespace std;

int main() {
    int fd[2]; // File descriptors for pipe
    if(pipe(fd) == -1) { // Creating a pipe, fd[0] for reading and fd[1] for writing
        cerr << "Pipe failed" << endl; // Error handling if pipe creation fails
        return 1;
    }

    pid_t pid = fork(); // Fork a child process
    if(pid == -1) { // Error handling for fork failure
        cerr << "Fork failed" << endl;
        return 1;
    }

    if(pid > 0) { // Parent process
        close(fd[0]); // Close reading end of pipe in parent
        ifstream inputFile("input.txt"); // Open input file
        if(!inputFile) { // Error handling if input file cannot be opened
            cerr << "Error opening input file" << endl;
            return 1;
        }
        string operation;
        float num;
        while(inputFile >> operation) { // Read operation from file
            string line = operation;
            while(inputFile >> num) { // Read numbers from file
                line += " " + to_string(num);
            }
            // Send line to child through pipe
            write(fd[1], line.c_str(), line.size() + 1);
            char result[100];
            // Read result from child
            read(fd[0], result, sizeof(result));
            cout << "Original: " << line << " = " << result << endl; // Print original line and result
        }
        close(fd[1]); // Close writing end of pipe in parent
        wait(NULL); // Wait for child process to finish
    } else { // Child process
        close(fd[1]); // Close writing end of pipe in child
        char buffer[100];
        while(read(fd[0], buffer, sizeof(buffer)) > 0) { // Read from parent through pipe
            string line(buffer);
            istringstream iss(line);
            string operation;
            iss >> operation;
            float result = 0;
            float num;
            while(iss >> num) { // Perform arithmetic operations
                if(operation == "ADD") {
                    result += num;
                } else if(operation == "SUB") {
                    result -= num;
                } else if(operation == "MUL") {
                    result *= num;
                } else if(operation == "DIV") {
                    if(num != 0) {
                        result /= num;
                    } else {
                        cerr << "Error: Division by zero" << endl;
                        return 1;
                    }
                }
            }
            string resultStr = to_string(result);
            // Send result back to parent through pipe
            write(fd[1], resultStr.c_str(), resultStr.size() + 1);
        }
        close(fd[0]); // Close reading end of pipe in child
    }
    return 0;
}
