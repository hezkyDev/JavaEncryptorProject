#!/bin/bash

# Create the bin directory if it doesn't exist
if [ ! -d "bin" ]; then
  mkdir bin
  echo "Created bin directory"
fi

# Navigate to the Java source directory
cd src/main/java || exit

# Compile the Java files and place the .class files in the bin folder
echo "Compiling Java files..."
javac -d ../../../bin StringEncryptor.java
if [ $? -eq 0 ]; then
  echo "Compilation successful!"
else
  echo "Compilation failed!" >&2
  exit 1
fi

# Navigate back to the project root directory
cd ../../../

# Package the .class files into a .jar file
echo "Creating JAR file..."
jar cvf bin/StringEncryptor.jar -C bin .
if [ $? -eq 0 ]; then
  echo "JAR file created successfully!"
else
  echo "Failed to create JAR file!" >&2
  exit 1
fi

# Done
echo "Build process completed!"