# Searchable Encryption

## Overview

This project implements a searchable encryption scheme with performance evaluation. It is structured into different modules for encryption, logging, and performance analysis.

# Project Structure

SEARCHABLEENCRYPTION/
│── .vscode/ # VS Code configuration
│── src/ # Main source code directory
│ │── encryption.cpp # Implementation of encryption
│ │── encryption.hpp # Header file for encryption
│ │── main.cpp # Main entry point
│ │── Makefile # Compilation instructions
│ │── performance_plot.py # Script for performance visualization
│ │── performance_results.csv # CSV file with performance results
│ │── pibas.cpp # PIBAS implementation
│ │── pibas.hpp # PIBAS header file
│ └── src_log_brc/ # Additional source code for logging and BRC
│ │── all_together.cpp # Combined implementation
│ │── log-brc # Log-BRC executable
│ │── log-brc.cpp # Log-BRC implementation
│ │── log-brc.hpp # Log-BRC header file
│ │── main.cpp # Main entry point
│ │── Makefile # Compilation instructions
│ │── plot_results.py # Script for plotting results
│ └── results.csv # CSV file with logged results
│── .gitignore # Git ignore file
│── README.md # Project documentation
└── requirements.txt # Python dependencies

## Installation

## Prerequisites

A C++ compiler (g++)

Python (for performance visualization)

GNU Make

## Build Instructions

To compile the project, navigate to the src/ or src/src_log_brc/ directory and run:

make

Usage

### Running the Encryption Module

` ./src/main`

### Running Log-based Retrieval

`./src_log_brc/log-brc`

### Generating Performance Plots

`python3 src/performance_plot.py`

`python3 src_log_brc/plot_results.py`
