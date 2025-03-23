# CS5600 - Practicum 1: Memory Hierarchy Simulation
Author: Shorena K. Anzhilov  
Northeastern University  
Spring 2025

## 📁 Project Overview
This project simulates a hierarchical memory system for message storage, including:
- Message creation and disk storage (Part 1)
- In-memory caching with LRU strategy (Part 2)
- Page replacement algorithms: LRU and Random (Part 3)
- Cache evaluation with hit/miss metrics (Part 4)

## 🛠️ How to Compile
Run the following command in the terminal:
make

To clean up compiled files:
make clean


## How to Run
After compiling, run the executable:
./memory_sim


## Output
- Displays test output for manual messages
- Shows cache hits/misses and hit ratio for LRU and Random replacement strategies

## Included Files
- `main.c`, `message.c`, `message.h`, `storage.c`, `storage.h` – source code
- `Makefile` – build script
- `Report_Shorena Anzhilov_Practicum_CS5600.pdf` – full project report
- `metrics.xlsx` – Excel file with hit/miss results
- `Shorena_Anzhilov_CS5600_Practicum1_Grading_Rubric` - Grading Rubric
- `README.txt` – this file

##  Notes
- The cache is fixed to 16 messages, with 1024 bytes per message.
- Message accesses are simulated with 1000 random requests.
- See the report for detailed explanation and results.
