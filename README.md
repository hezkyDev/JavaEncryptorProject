Java String Encryption with Native C++ Integration

Description:

This project demonstrates how to encrypt a string using RSA encryption in a Java application, with native C++ code integration using the Java Native Interface (JNI). The project leverages OpenSSL for cryptographic operations and uses JNI to call the native C++ code from Java.

Project Structure:

/JavaEncryptorProject
   /src
      /main
         /java         (Java source code)
            StringEncryptor.java
         /native       (Native C++ code)
            libNativeHashingEncryption.dylib
            NativeHashingEncryption.cpp
         /resources    (Resources like the public key)
            pi-xcels_public_key.pem
   /scripts      (Python and other utility scripts)
      convert_to_pem.py  (Script to convert JWK to PEM format)
   /bin                 (Compiled .class and .jar files)
   README.md            (Project documentation)
   build.sh             (Build script for automating the process)

- src/main/java: Contains the Java source code (StringEncryptor.java).
- src/main/native: Contains the C++ source code and the compiled library (libNativeHashingEncryption.dylib).
- src/main/resources: Contains resources such as the public key (pi-xcels_public_key.pem).
- src/main/scripts: Contains utility scripts like `convert_to_pem.py` for converting JWK to PEM format.
- bin: Stores compiled .class and .jar files.
- README.md: Documentation for the project.
- build.sh: A bash script to automate the compilation and packaging process.

Prerequisites:

Before building and running this project, ensure that the following tools are installed:

- Java Development Kit (JDK): Version 11 or later.
- OpenSSL: Required for cryptographic operations in the C++ code.
- g++: The GNU C++ compiler for compiling the native code.
- Bash: If using the provided build script.
- Python: Required to run the `convert_to_pem.py` script.

Public Key Conversion (JWK to PEM):

This project includes a Python script (convert_to_pem.py) that converts the JWK (JSON Web Key) format into PEM format, which is needed for the encryption process. The script also logs each step and outputs the full path of the saved PEM file.

Usage

- Make sure the pi-xcels.json file (which contains the public key in JWK format) is in the /src/main/scripts folder.
- Run the Python script to convert the public key:

python src/main/scripts/convert_to_pem.py

- The script will log the process and save the converted pi-xcels_public_key.pem file in the /src/main/resources folder.

Building and Running the Project:

Step 1: Compile the C++ Code

To compile the C++ code into a shared library, run the following command from the project root:

g++ -shared -o src/main/native/libNativeHashingEncryption.dylib -fPIC \\
-I$JAVA_HOME/include -I$JAVA_HOME/include/darwin \\
-I/opt/homebrew/opt/openssl@3/include \\
-L/opt/homebrew/opt/openssl@3/lib -lssl -lcrypto \\
src/main/native/NativeHashingEncryption.cpp

This command generates the libNativeHashingEncryption.dylib file in the src/main/native directory.

Step 2: Compile the Java Code and Create a JAR File

You can either compile manually or use the provided build script. To automate the process, run:

./build.sh

This will:

- Compile the Java source files.
- Create a StringEncryptor.jar file in the bin directory.

Step 3: Run the Java Program

To run the program, use the following command:

java -Djava.library.path=src/main/native -cp bin/StringEncryptor.jar StringEncryptor

Example Usage:

$ java -Djava.library.path=src/main/native -cp bin/StringEncryptor.jar StringEncryptor
Enter the string to hash and encrypt:
HelloWorld
Public Key: {key details...}
Encrypted Hash: {encrypted output...}

Testing the Application:

Test the program with different inputs to ensure it behaves as expected. Here are some test cases you can try:

Test Case 1: Short String

Enter the string to hash and encrypt:
Hello

Test Case 2: Special Characters

Enter the string to hash and encrypt:
@Hello$World#123!

Test Case 3: Empty String

Test how the program handles empty input:

Enter the string to hash and encrypt:
(press Enter)

Test Case 4: Long String

Enter the string to hash and encrypt:
Lorem ipsum dolor sit amet, consectetur adipiscing elit... (long string)

Error Handling:

The program handles the following error cases:

- Missing Public Key File: If the pi-xcels_public_key.pem file cannot be found, the program will return an error message (Error: Unable to open public key file).
- Empty String: If the user inputs an empty string, the program will return an error message (Error: Input string cannot be empty).
- File Permissions: If the public key file lacks read permissions, the program will print an error message.
Ensure that the public key file is located in src/main/resources and has the appropriate permissions.

Known Issues:

- The program requires the OpenSSL library, which may not be pre-installed on all systems.
- The public key file path is hardcoded in the C++ code and must be updated if the file location changes.

License:

This project is licensed under the MIT License. You are free to use, modify, and distribute this project as long as the original license is included.