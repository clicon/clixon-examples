# Clixon openconfig-system docker image

This directory contains code for running, building and pushing the clixon
openconfig-system docker container. 

## Example run

First, the container is started with the backend running:
```
  $ sudo docker run --rm --name openconfig -d clixon/openconfig-system clixon_backend -Fs init
```
Then a CLI is started, and finally the container is removed:
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

## Build and push

Perform the build by `make docker`. This copies the latest committed clixon code into the container.

You may also do `make push` if you want to push the image, but you may then consider changing the image name (in the Makefile:s).

(You may have to login for push with sudo docker login -u <username>)
