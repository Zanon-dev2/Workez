# 🛠️ Workez

A personal C++ utility library built to make my life easier by simplifying repetitive tasks and boilerplate code.

## 📖 Overview

**Workez** is a growing, general-purpose helper library created to solve everyday annoyances in C++. Rather than focusing on a single domain, it bundles convenient wrappers for stream-based list inputs, vector summation, inter-process communication on Windows, and cleaner HTTP route definitions for the **Crow** web framework.

> ⚠️ **Work in Progress:** This library is actively evolving at its own pace as new utilities become useful to me.

## ⚙️ Requirements & Compatibility

* **C++ Standard:** C++17 or higher.
* **Operating System:** Designed primarily for **Windows** (`workez::piper` relies on Windows APIs and will not work on other platforms).
* **Dependencies:** Requires the [Crow](https://github.com) microframework (`workez::APIR` depends on Crow to function).

## ✨ Features & Usage

### 1. `zlist` — Stream-Based List Input
Reads a full line of space-separated inputs at once and populates a `std::vector` using overloaded stream operators.

```cpp
#include "workez.hpp"
#include <vector>
#include <iostream>

int main() {
    std::vector<int> numbers;
    std::cout << "Enter a list of space-separated numbers: ";
    
    // Reads the entire input line at once and populates the vector
    workez::zlist >> numbers; 
    
    std::cout << "Total items read: " << numbers.size() << "\n";
}
```

### 2. `soma()` — Vector Summation
A simple helper function originally created for testing that calculates the sum of all elements in a vector.

```cpp
#include "workez.hpp"
#include <vector>
#include <iostream>

int main() {
    std::vector<double> list = {1.2, 2.2, 5.7, 9.9};
    
    // Sums all elements in the list
    double total = soma(list);
    
    std::cout << total << "\n";
}
```

### 3. `piper` — External Executable Communication (Windows)
Manages permissions and facilitates communication with external executables by passing arguments/input and capturing their output.

```cpp
#include "workez.hpp"
#include <iostream>

int main() {
    // 1. Grant permission to "calculator.exe" under the "utilities" category
    workez::piper.parlist.add("utilities", "calculator");

    // 2. Call the executable passing an input string (e.g., "5 + 5")
    std::string output = workez::piper.call("5 + 5", "calculator", "utilities");
    
    std::cout << "External program output: " << output << "\n";

    // 3. Remove permission when no longer needed
    workez::piper.parlist.del("utilities", "calculator");
}
```

### 4. `APIR` — Simplified Crow Route Wrapper
Reduces boilerplate when defining HTTP routes in **Crow**, supporting direct return values as well as automatic `200 OK` responses for `void` handlers.

```cpp
#include "workez.hpp"

int main() {
    crow::SimpleApp app;

    // Create a simple GET route that directly returns a string
    workez::APIR<std::string>(app, "GET", [](const crow::request& req) {
        return "Hello World!";
    }, "/hello");

    // Or define the handler lambda externally
    auto handleSave = [](const crow::request& req) {
        // ... your processing logic here ...
    };

    // Create a POST route returning void (status 200 is sent automatically)
    workez::APIR<void>(app, "POST", handleSave, "/save");

    app.port(18080).multithreaded().run();
}
```

## 📄 License

This project is open-source and available for personal and educational use.

---
Developed by [Zanon-dev2](https://github.com/Zanon-dev2).
