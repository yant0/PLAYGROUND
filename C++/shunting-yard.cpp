#include <iostream>
#include <deque>
#include <cmath>
using namespace std;

int precedence(char op) {
    if (op == '^') return 3;
    if (op == '*' || op == '/') return 2;
    if (op == '+' || op == '-') return 1;
    return 0;
}

string shuntingYard(string input, char showSteps) {
    deque<char> operators;
    string output;
    string temp;
    int dotcheck = 0;

    for (int i = 0; i < input.size(); i++) {
        if (isspace(input[i])) continue;                        //* White space

        if (isdigit(input[i])) {                                //* NUMBER
            while (isdigit(input[i]) || input[i] == '.') {
                if (input[i] == '.') dotcheck += 1;
                if (dotcheck > 1)return "Error, multiple decimal inside one number";
                temp += input[i];
                i++;
            }
            output += temp + " ";
            temp = "";
            dotcheck = 0;
            i--;
        } else if (input[i] == '(') {                           //* " ( "
            operators.push_front(input[i]);
        } else if (input[i] == ')') {                           //* " ( "
            while (operators.front() != '(') {
                output.push_back(operators.front());
                output += " ";
                operators.pop_front();
            }
            if (!operators.empty() && operators.front() == '(') {
                operators.pop_front();
            }
        } else if (input[i] == '.') {                           //* " . "
            do {
                temp += input[i];
                i++;
            } while (isdigit(input[i]));
            output += temp + " ";
            temp = "";
            if (input[i] == '.') return "Error, multiple decimal inside one number";
        } else if (input[i] == '-' && !isspace(input[i+1])) {   //* NEGATIVE
            do {
                if (input[i] == '.') dotcheck += 1;
                if (dotcheck > 1)return "Error, multiple decimal inside one number";
                temp += input[i];
                i++;
                if (input[i] == '-') return "Error, multiple negatives";
            } while (isdigit(input[i]));
            output += temp + " ";
            temp = "";
            dotcheck = 0;
            if (input[i] == '.') return "Error, multiple decimal inside one number";
        } else if (!isdigit(input[i])) {                        //* OPERATORS
            while (!operators.empty() && (precedence(input[i]) <= precedence(operators.front()))) {
                if (input[i] == '^' && (precedence(input[i]) == precedence(operators.front()))) {
                    break;
                }
                output.push_back(operators.front());
                output += " ";
                operators.pop_front();
            }
            operators.push_front(input[i]);
        }

        if (showSteps == 'y') {
            cout << "\noutput : " << output;
            cout << "\t\toperators : ";
            auto it = operators;
            while (!it.empty()) {
                cout << it.front() << " ";
                it.pop_front();
            }
        }
    }
    while (!operators.empty()) {                      //* Adds remeaning operators
        output.push_back(operators.front());
        output += " ";
        operators.pop_front();
    }
    return output;
}

double calculateOperator(double a, double b, char op) {
    double result;
    switch (op) {
    case '+': return result = a + b;
    case '-': return result = a - b;
    case '*': return result = a * b;
    case '/': return result = a / b;
    case '^': return result = pow(a, b);
    default: return 0;
    }
}

double calculatePostfix(string input, char steps) {
    deque<double> num;
    string token;
    char op;
    int steps_it=1;
    bool calculated = false;

    for (int i = 0; i < input.size(); i++) {
        if (isspace(input[i]) && !token.empty()) {
            try {
                num.push_back(stod(token));
                token.clear();
            } catch(const exception& e){
                cerr << "Error: " << e.what() << "\n";
                return 0;
            }
        } else if (precedence(input[i]) && isspace(input[i+1])) {
            try {
            calculated = true;
            double b = num.back(); num.pop_back();
            double a = num.back(); num.pop_back();
            num.push_back(calculateOperator(a, b, input[i]));
            if (steps == 'y') cout << "\n" << steps_it << ": " << a << input[i] << b; steps_it++;
            token.clear();
            continue;
            } catch (const exception& e) {
                cerr << "Error: " << e.what() << "\n";
                return 0;
            }
        }
        token += input[i];
    }
    if (!calculated) {cout << "Error: No calculations made\n"; return 0;}
    return num.back();
}

int main() {
    string input;
    char steps = 'n';

    cout << "[calculator with Shunting Yard & RPN implementation]\n"
             "[q for quit]\t"
             "[s for steps]\n\n";
    while (input != "q") {
        cout << "> ";
        getline(cin, input);
        if (input == "q") return 0;
        if (input == "s") {
            cout << "steps activated\n";
            steps = 'y';
            continue;
        }

        string output = shuntingYard(input, steps);
        double result = calculatePostfix(output, steps);

        if (steps == 'y') {
            cout << "\nPost-fix: " << output;
            cout << "\nResult\t: " << result << "\n";
        } else cout << result << "\n";
    }
}