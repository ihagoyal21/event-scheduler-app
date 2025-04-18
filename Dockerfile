# Use an official Ubuntu base image
FROM ubuntu:22.04

# Install dependencies
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    git \
    libboost-all-dev \
    libssl-dev

# Create working directory inside the container
WORKDIR /app

# Copy only the necessary files into the container (excluding build/ to avoid cache conflicts)
COPY . /app

# Remove any old CMake cache files to avoid conflicts
RUN rm -rf CMakeCache.txt CMakeFiles/

# Create the build directory and run cmake
RUN mkdir -p build && cd build && cmake .. && make

# Expose the port your app runs on (if required)
EXPOSE 8080

# Run the application (adjust path if necessary)
CMD ["./build/bin/event_scheduler_cpp"]
