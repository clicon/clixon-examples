# Clixon hello world docker image

This directory contains code for running, building, and pushing the clixon hello world docker container. 

## Build

Build the docker:
```
  $ make docker
```

## Example run

First, the container is started with a backend and a restconf listening on port 8080:
```
  $ sudo docker run --rm -p 8080:80 --name hello -d clixon/hello
```

You can start a CLI with some example commands:
```
  $ sudo docker exec -it hello clixon_cli
  cli> set ?
   hello                 
  cli> set hello world 
  cli> show configuration 
  hello world;
  cli> commit
  cli> q
  $ sudo docker kill hello
```

You can also use netconf via stdin/stdout:
```
  $ sudo docker exec -it hello clixon_netconf
  <?xml version="1.0" encoding="UTF-8"?>
  <hello xmlns="urn:ietf:params:xml:ns:netconf:base:1.0">
     <capabilities><capability>urn:ietf:params:netconf:base:1.1</capability></capabilities>
   </hello>]]>]]>
  <rpc xmlns="urn:ietf:params:xml:ns:netconf:base:1.0"><get-config><source><running/></source></get-config></rpc>]]>]]>
```

Or using restconf using curl on exposed port 8080:
```
  $ curl -X GET http://localhost:8080/restconf/data/clixon-hello:hello
 ```

## Build and push

Perform the build by `make docker`. This copies the latest committed clixon code into the container.

You may also do `make push` if you want to push the image, but you may then consider changing the image name (in the Makefile:s).

(You may have to login for push with sudo docker login -u <username>)

