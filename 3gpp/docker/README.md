# Clixon 3GPP docker image

This directory contains code for running, building, and pushing the clixon 3GPP docker container. 

## Example run

First, the container is started with a backend and a restconf listening on port 8080:
```
  $ sudo docker run --rm -p 8080:80 --name 3gpp -d clixon/3gpp
```

You can start a CLI with some example commands:
```
  $ sudo docker exec -it 3gpp clixon_cli
  3gpp> set ?
  EUtraNetwork          A subnetwork containing gNB external E-UTRAN entities.
  ManagedElement        Represents telecommunications equipment or 
                        ...
  NRNetwork             A subnetwork containing gNB external NR entities.
  SubNetwork            Represents a set of managed entities
  3gpp> set NRNetwork 22 attributes dnPrefix er
  3gpp> set NRNetwork 22 attributes priorityLabel 1
  3gpp> set ManagedElement 42 UDMFunction 23 
  3gpp> show configuration
  NRNetwork {
    id 22;
    attributes {
        dnPrefix er;
        priorityLabel 1;
    }
  }
  3gpp> commit
  3gpp> quit
```

Or using restconf using curl on exposed port 8080:
```
  $ curl -X GET http://localhost:8080/restconf/data/_3gpp-nr-nrm-nrnetwork:NRNetwork=22
  {
    "_3gpp-nr-nrm-nrnetwork:NRNetwork": [
      {
        "id": "22",
        "attributes": {
          "dnPrefix": "er",
          "priorityLabel": 1
        }
      }
    ]
  }
```

You can also use netconf via stdin/stdout:
```
  $ sudo docker exec -it 3gpp clixon_netconf
  <rpc><get-config><source><running/></source></get-config></rpc>]]>]]>
  <rpc-reply><data><NRNetwork xmlns="urn:3gpp:sa5:_3gpp-nr-nrm-nrnetwork"><id>22</id><attributes><dnPrefix>er</dnPrefix><priorityLabel>1</priorityLabel></attributes></NRNetwork></data></rpc-reply>]]>]]>
```

## Build and push

Perform the build by `make docker`. This copies the latest committed clixon code into the container.

You may also do `make push` if you want to push the image, but you may then consider changing the image name (in the Makefile:s).

(You may have to login for push with sudo docker login -u <username>)
