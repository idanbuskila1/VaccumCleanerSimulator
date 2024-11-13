# Use an official Ubuntu base image
FROM ubuntu:latest

# Set the working directory in the container
WORKDIR /project

# Install dependencies
RUN apt update && apt install -y \
    build-essential \
    cmake \
    libboost-all-dev \
    && rm -rf /var/lib/apt/lists/*

# Copy the entire project into the container
COPY . .

# Remove any existing build artifacts or cache from previous compilations
RUN rm -rf build/*

# Create the build directory
RUN mkdir -p build

# Run CMake and Make within the build directory
RUN make sim && make alg1 && make alg2

# Default command to run the simulator project (without re-compiling)
CMD ["make", "run"]

# docker build -t simulator . 
# docker run -it simulator 
# docker cp 3be6a1affd6a:/project/build ./docker_res

