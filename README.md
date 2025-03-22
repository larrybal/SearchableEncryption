# Searchable Encryption

## Overview

This project implements a searchable encryption scheme with performance evaluation. It is structured into different modules for encryption, logging, and performance analysis.

# Project Structure

src/ - source code for the PiBas SSE implementation

- encryption.cpp
  - helper function to implement encryption building blocks like key gen and enc and dec algorithms
- encryption.hpp
  - header function for encryption.cpp
- pibas.cpp
  - function definitions for setup and search of a client server encrypted SSE based on PiBas
- pibas.hpp
  - header file for pibas.cpp
- main_static_dataset.cpp
  - main function for fixed dataset size but varying range query experiment
- main_static_range.cpp
  - main function for fixed range but varying dataset size query experiment
- plot_performance_static_dataset.py
  - performance plots for the output of main_static_dataset.cpp
- plot_performance_static_range.py
  - performance plots for the output of main_static_range.cpp
- Makefile
  - logic to compile main\*

src_log_brc/ - source code for the LOG BRC implememtation to do range queries on an encrypted dataset

- log_brc.cpp
  - function definitions for building the index tree and performing a range search query
- log_brc.hpp
  - header function for log_brc.cpp
- main_static_dataset.cpp
  - main function for fixed dataset size but varying range query experiment
- main_static_range.cpp
  - main function for fixed range but varying dataset size query experiment
- plot_performance_static_dataset.py
  - performance plots for the output of main_static_dataset.cpp
- plot_performance_static_range.py
  - performance plots for the output of main_static_range.cpp
- Makefile
  - logic to compile main\*

requirements.txt - python package requirements

## Installation

Install openssl (macos Sierra):

`brew install openssl`

Link the package:

`ln -s /usr/local/opt/openssl/include/openssl /usr/local/include`

Install the python dependencies:

`pip install -r requirements.txt`

## Prerequisites

A C++ compiler (g++)

Python3 (for performance visualization)

GNU Make

## Build Instructions

To compile the project, navigate to the src/ or src_log_brc/ directory and run:

make

Usage

### Running the Encryption Module

`./src/main_static_dataset`
`./src/main_static_range`

### Running Log-based Retrieval

`./src_log_brc/main_static_dataset`
`./src_log_brc/main_static_range`

### Generating Performance Plots

`python3 [src or src_log_brc]/plot_performance_static_dataset.py`

`python3 [src or src_log_brc]/plot_performance_static_range.py`
