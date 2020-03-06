# Clixon 5G NRM docker image

This directory contains code for running, building, and pushing the clixon 5G NRM docker container. 

## Example run

The following shows a simple example of how to run the example
application. First, the container is started with the backend running:
```
  $ sudo docker run --rm --name nrm -d clixon/nrm clixon_backend -Fs init
```
Then a CLI is started, and finally the container is removed:
```
  $ sudo docker exec -it nrm clixon_cli
  cli> set ?
  > q
  $ sudo docker kill nrm
```

## Build and push

Perform the build by `make docker`. This copies the latest committed clixon code into the container.

You may also do `make push` if you want to push the image, but you may then consider changing the image name (in the Makefile:s).

(You may have to login for push with sudo docker login -u <username>)
