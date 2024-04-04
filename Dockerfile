FROM alpine:latest

RUN apk update && apk add --no-cache build-base g++ cmake git
RUN apk add libpq-dev
COPY . .
RUN mkdir build && cd build && cmake .. && make -j4
ENTRYPOINT ["/bin/main"]