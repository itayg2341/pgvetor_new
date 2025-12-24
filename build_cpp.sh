#!/bin/bash

# Build script for pgvector C++ library

echo "Building pgvector C++ library..."

cd src_cpp

# Build the library
echo "Building library..."
make clean
make all

# Run tests
echo "Running tests..."
make test

# Return to original directory
cd ..

echo "Build complete!"
