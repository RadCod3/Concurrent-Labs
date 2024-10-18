
## Concurrent Programming Lab 2: Senate Bus Problem

This project implements the Senate Bus Problem using Java with semaphores and mutexes to synchronize bus and rider threads.

### Project Structure

The project contains the following files:
```
.
├── .gitignore
├── Bus.java
├── BusStop.java
├── Main.java
└── Rider.java
```

- Bus.java: The class representing the bus threads.
- BusStop.java: The class handling synchronization between the bus and rider threads.
- Rider.java: The class representing the rider threads.
- Main.java: The main class that initiates the simulation.

### How to Run

### Prerequisites

You need to have Java installed on your machine. If Java is not installed, you can install it by following these steps:

On Linux:

    sudo apt-get install default-jdk


On MacOS (using Homebrew):

    brew install openjdk


On Windows: Download and install Java from the Oracle website.

Verify the installation by running:

    java -version

### Steps to Compile and Run the Project

1.	Navigate to the project root directory where your .java files are located:

    cd /path/to/your/project
2.	Compile the Java files (if they are not already compiled). You can use the following command to compile all .java files in the directory:

    javac *.java

This will generate the corresponding .class files.

3.	Run the project by executing the Main class:

    java Main

