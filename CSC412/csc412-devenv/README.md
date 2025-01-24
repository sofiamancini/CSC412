# CSC 412: development environment

This repo contains a minimal dev environment setup for CSC 412. It provides scripts to create the Docker container used throughout the CSC 412 container.

## Getting started

Install Docker: https://www.docker.com/
Docker should be running after you install it.
Then, run the following commands and scripts in a terminal.
You will need at last 2 GB of hard drive space to create the 

```
# 1. build docker image locally, open terminal and run:
cd docker
./csc412-build-docker
```

No need to run the command "`docker scan`" command at the end. Now start the container:

```
# 2. start development environment
cd ..
./csc412-run-docker
```
## Where does my code for assignments and labs live?

In the `home` folder in this project directory. :)


## Acknowledgements

This setup is a modified version of the setup used by
[Brown's CSCI 0300](https://cs.brown.edu/courses/csc412/) and [Harvard's CS61](https://cs61.seas.harvard.edu/site/2021/) and reused
with permission.
