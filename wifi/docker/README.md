# Clixon wifi-system docker image

This directory contains code for running, building and pushing the clixon
openconfig wifi-system docker container. 

## Example run

First, the container is started with a backend and a restconf listening on port 8080:
```
  $ sudo docker run --rm -p 8080:80 --name wifi -d clixon/wifi-system
```

You can start a CLI with some example commands:
```
  $ sudo docker exec -it wifi clixon_cli
wifi> mode provision-aps provision-ap 01:02:03:04:05:06
wifi> set config mac 01:02:03:04:05:06
wifi> set config hostname ap1
wifi> show configuration 
provision-aps {
    provision-ap {
        mac 01:02:03:04:05:06;
        config {
            mac 01:02:03:04:05:06;
            hostname ap1;
        }
    }
}
wifi> commit
wifi> q   
```

You can also use netconf via stdin/stdout:
```
  $ sudo docker exec -it wifi clixon_netconf
  <?xml version="1.0" encoding="UTF-8"?>
  <hello xmlns="urn:ietf:params:xml:ns:netconf:base:1.0">
     <capabilities><capability>urn:ietf:params:netconf:base:1.1</capability></capabilities>
   </hello>]]>]]>
  <rpc xmlns="urn:ietf:params:xml:ns:netconf:base:1.0"><get-config><source><running/></source></get-config></rpc>]]>]]>
  <rpc-reply xmlns="urn:ietf:params:xml:ns:netconf:base:1.0"><data><provision-aps xmlns="http://openconfig.net/yang/ap-manager"><provision-ap><mac>01:02:03:04:05:06</mac><config><mac>01:02:03:04:05:06</mac><hostname>ap1</hostname></config></provision-ap></provision-aps>...
```

Or using restconf using curl on exposed port 8080:
```
  $ curl -X GET http://localhost:8080/restconf/data/openconfig-ap-manager:provision-aps
{
    "openconfig-ap-manager:provision-aps": {
      "provision-ap": [
        {
          "mac": "01:02:03:04:05:06",
          "config": {
            "mac": "01:02:03:04:05:06",
            "hostname": "ap1"
          }
        }
      ]
    }
  }
```

## Build and push

Perform the build by `make docker`. This copies the latest committed clixon code into the container.

You may also do `make push` if you want to push the image, but you may then consider changing the image name (in the Makefile:s).

(You may have to login for push with sudo docker login -u <username>)
