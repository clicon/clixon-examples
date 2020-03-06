# Clixon helloworld docker image

This directory contains code for building, running and pushing the clixon helloworld docker
container. By default it is pushed to docker hub clixon/clixon, but you can change
the IMAGE in Makefile.in and push it to another name.

## Build and push

Perform the build by `make docker`. This copies the latest committed clixon code into the container.

You may also do `make push` if you want to push the image, but you may then consider changing the image name (in the makefile:s).

(You may have to login for push with sudo docker login -u <username>)

## Example run

The following shows a simple example of how to run the example
application. First, the container is started with the backend running:
```
  $ sudo docker run --rm --name hello -d clixon/clixon clixon_backend -Fs init
```
Then a CLI is started, and finally the container is removed:
```
  $ sudo docker exec -it hello clixon_cli
  cli> set ?
   hello                 
  cli> set hello world 
  cli> show configuration 
  hello world;
  > q
  $ sudo docker kill hello
```

Note that the clixon example application is a special case since the example is
already a part of the installation. If you want to add your own
application, such as plugins, cli syntax files, yang models, etc, you
need to extend the base container with your own additions.
