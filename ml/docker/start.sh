#!/bin/bash

# to build it, use command:
# docker build -t tklearnml .

# after run, open http://localhost:8888/lab?

docker run -e ML_USER=ktar -p 8888:8888 -p 6006:6006 -v $(pwd)/notebooks:/notebooks tklearnml
