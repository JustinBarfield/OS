#include <iostream>
#include <fstream>
#include <sstream>
#include <unistd.h>

using namespace std;

// Function to perform arithmetic operations
double performOperation(string operation, const vector<double>& numbers) {
    double result = numbers[0];
    for (int i = 1; i < numbers.size(); ++i) {
        if (operation == "ADD") {
            result += numbers[i];
        } else if (operation == "SUB") {
            result -= numbers[i];
        } else if (operation == "MUL") {
            result *= numbers[i];
        } else if (operation == "DIV") {
            result /= numbers[i];
        }
    }
    return result;
}

int main() {
    int pipefd[2]; // File descriptors for pipe
    if (pipe(pipefd) == -1) {
        cerr << "Pipe creation failed";
        return 1;
    }

    pid_t pid = fork();
    if (pid < 0) {
        cerr << "Fork failed";
        return 1;
    }

    if (pid > 0) { // Parent process
        close(pipefd[0]); // Close reading end of pipe
        ifstream inputFile("input.txt");
        string line;
        while (getline(inputFile, line)) {
            write(pipefd[1], line.c_str(), line.size()); // Write data to pipe
            char buffer[100]; // Buffer to read child's response
            read(pipefd[0], buffer, sizeof(buffer)); // Read child's response
            cout << line << " = " << buffer << endl; // Print result
        }
        close(pipefd[1]); // Close writing end of pipe
    } else { // Child process
        close(pipefd[1]); // Close writing end of pipe
        char buffer[100];
        while (read(pipefd[0], buffer, sizeof(buffer)) > 0) { // Read data from parent
            stringstream ss(buffer);
            string operation;
            ss >> operation;
            vector<double> numbers;
            double num;
            while (ss >> num) {
                numbers.push_back(num);
            }
            double result = performOperation(operation, numbers); // Perform arithmetic operation
            stringstream result_ss;
            result_ss << result;
            string result_str = result_ss.str();
            write(pipefd[1], result_str.c_str(), result_str.size()); // Write result to pipe
        }
        close(pipefd[0]); // Close reading end of pipe
    }

    return 0;
}
