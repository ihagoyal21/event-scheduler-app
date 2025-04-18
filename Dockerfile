# Use an official Ubuntu base image
FROM ubuntu:22.04

# Install dependencies
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    git \
    libboost-all-dev \
    libssl-dev

# Copy your app files into the container
WORKDIR /app
COPY . /app

# Build your app (adjust if you use a build folder)
RUN mkdir -p build && cd build && cmake .. && make

# Expose the port your app runs on (e.g., 8080)
EXPOSE 8080

# Run the server (adjust path if needed)
CMD ["./build/bin/event_scheduler_cpp"]

