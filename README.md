# Her Cycle

A console-based C++ application developed in VS Code to track menstrual cycles, estimate upcoming period dates, log symptoms, and maintain a snapshot history of entries.

## Features

- **Email Login & Profile Setup:** Validates basic email format (`@` and `.`), records user name, and supports age entry either directly or calculated from Date of Birth (DOB) using `<ctime>`.
- **Next Period Estimation:** Adds the average cycle length to the last period start date with leap-year and variable month-length handling (`DD/MM/YYYY`), defaulting to `28` days for cycle length and `5` days for period duration if non-positive values are entered.
- **Symptom Logging:** Captures comma-separated symptoms (or `none`) for each cycle entry.
- **Cycle Summary & History:** Displays the latest cycle summary and stores snapshots (`PeriodRecord`) in a `std::vector` so each history entry preserves name, email, age, cycle details, estimate, and symptoms.
- **Input Validation:** Guards integer prompts against non-numeric input and prevents negative age values.

## Project Structure

- Single-file C++ console program (`HerCycleTracker` class, `PeriodRecord` struct, and date/input helper functions).

## Getting Started (VS Code)

### Prerequisites
- A C++ compiler supporting C++11 or later (such as `g++` / MinGW on Windows, `clang++` on macOS, or `g++` on Linux).
- Visual Studio Code with the **C/C++** extension installed.

### Build and Run
```bash
g++ -std=c++17 -Wall -Wextra her_cycle.cpp -o her_cycle
./her_cycle
