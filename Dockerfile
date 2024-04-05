FROM alpine:latest

RUN apk update && apk add --no-cache build-base g++ cmake git
RUN apk add libpq-dev

# Clone project
COPY . .

# Build project
RUN mkdir build && cd build && cmake .. && make -j8

# Run service
ENTRYPOINT ["/bin/main"]