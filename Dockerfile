FROM alpine:latest

RUN apk update && apk add --no-cache build-base g++ cmake git
RUN apk add libpq-dev

# Clone project
COPY . .

# Download jwt-cpp for JWT tokens
RUN git clone https://github.com/Thalhammer/jwt-cpp.git && cd jwt-cpp && cmake . && cmake --install .

# Build project
RUN mkdir build && cd build && cmake .. && make -j8

# Run service
ENTRYPOINT ["/bin/main"]