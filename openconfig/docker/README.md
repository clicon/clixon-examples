# Clixon openconfig-system docker image

This directory contains code for running, building and pushing the clixon
openconfig-system docker container. 

## Example run

First, the container is started with a backend and a restconf listening on port 8080:
```
  $ sudo docker run --rm -p 8080:80 --name openconfig -d clixon/openconfig
```

You can start a CLI with some example commands:
```
  $ sudo docker exec -it openconfig clixon_cli
openconfig> set ?
  components            Enclosing container for the components in the system.
  messages              Top-level container for Syslog messages.
  system                Enclosing container for system-related configuration and
        operational state data
  cli> set hello world
openconfig> set system 
  <cr>
  aaa                   Top-level container for AAA services
  clock                 Top-level container for clock configuration data
  config                Global configuration data for the system
  dns                   Enclosing container for DNS resolver data
  grpc-server           Top-level container for the gRPC server
  logging               Top-level container for data related to logging / syslog
  memory                Top-level container for system memory data
  messages              Top-level container for Syslog messages.
  ntp                   Top-level container for NTP configuration and state
  processes             Parameters related to all monitored processes
  ssh-server            Top-level container for ssh server
  telnet-server         Top-level container for telnet terminal servers
openconfig> set system clock config timezone-name Europe/Stockholm
openconfig> commit
openconfig> show configuration 
system {
    clock {
        config {
            timezone-name Europe/Stockholm;
        }
    }
}
openconfig> q

$ sudo docker kill 
```

You can also use netconf via stdin/stdout:
```
  $ sudo docker exec -it openconfig clixon_netconf
  <?xml version="1.0" encoding="UTF-8"?>
  <hello xmlns="urn:ietf:params:xml:ns:netconf:base:1.0">
     <capabilities><capability>urn:ietf:params:netconf:base:1.1</capability></capabilities>
   </hello>]]>]]>
  <rpc><get-config><source><running/></source></get-config></rpc>]]>]]>
  <rpc-reply><data><system xmlns="http://openconfig.net/yang/system"><clock><config><timezone-name>Europe/Stockholm</timezone-name></config></clock></system></data></rpc-reply>]]>]]>
```

Or using restconf using curl on exposed port 8080:
```
  $ curl -X GET http://localhost:8080/restconf/data/openconfig-system:system
{
    "openconfig-system:system": {
      "clock": {
        "config": {
          "timezone-name": "Europe/Stockholm"
        }
      }
    }
  }
```

## Build and push

Perform the build by `make docker`. This copies the latest committed clixon code into the container.

You may also do `make push` if you want to push the image, but you may then consider changing the image name (in the Makefile:s).

(You may have to login for push with sudo docker login -u <username>)
