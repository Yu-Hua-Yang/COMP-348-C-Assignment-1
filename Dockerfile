# trivial dockerfile to create Programming Language environment

# Build the Docker image with a tag (name) "my_programming_env"
# docker build -t my_programming_env .


FROM gcc

RUN apt-get update && apt-get install -y \
    bash-completion   \
    cmake             \
    valgrind          \
    nano              \
    scons             \
    clojure           \
    leiningen         \
    erlang            \
    rebar             \  
    && rm -rf /var/lib/apt/lists/*