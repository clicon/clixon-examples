# Generic Clixon Docker Container

A generic [clixon](https://github.com/clicon/clixon) container with no application-specific configuration baked in.
You just provide the YANG files of your application, and the container provides CLI, NETCONF and RESTCONF for you.
This means the same container can be reused for different applications.
However, not that the container is management-only, the commits are no-op, there is no real semantics in the container, it is purely for exercising the management and YANG API:s.

## YANGs

### Main dir

Assume you have YANG files in a directory, say /usr/local/share/myyang.

Then you start a docker with::
```sh
sudo docker run -d \
  -v /usr/local/share/myyang:/usr/local/share/yang/main:ro \
  --name clixon-generic  \
  clixon/generic
```

### Module + lib dir

Some projects prefer a single top-level yang module and a library of YANG files. Then start as follows:
```sh
sudo docker run -d \
  -v /usr/local/share/myyang:/usr/local/share/yang/lib:ro \
  -e MODULE=mymodule \
  --name clixon-generic  \
  clixon/generic

Yuou can combine the main and lib dir options.

## Startup

```sh
sudo docker run -d \
  -v /usr/local/share/myyang:/usr/local/share/yang/main:ro \
  -v mystartup:/usr/local/var/clixon/startup_db:ro \
  --name clixon-generic  \
  clixon/generic
```

## CLI

When the docker has started you can start a cli in it:
```sh
docker exec -it clixon-generic clixon_cli
```

## NETCONF

## RESTCONF

Map ports and run:
```
sudo docker run -d \
  -v /usr/local/share/myyang:/usr/local/share/yang/main:ro \
  -p 8080:80 -p 830:430
  --name clixon-generic  \
  clixon/generic

curl http://localhost:8080/restconf/data
```

## gRPC/gNMI

Run with the grpc port: `-p 9339` plain-text port:

```sh
sudo docker run -d \
  -v /usr/local/share/myyang:/usr/local/share/yang/main:ro \
  -p 9339:9339
  --name clixon-generic  \
  clixon/generic

grpcurl -plaintext -import-path /usr/local/share/clixon/proto -import-path /usr/include -proto gnmi.proto -d '{"path":[{"elem":[]}],"type":"ALL","encoding":"ASCII"}' localhost:9339 gnmi.gNMI/Get
```
## Build

```sh
make docker
```
## docker-compose

Edit `docker-compose.yml` to point the volume mounts at your host paths, then:

```sh
sudo docker-compose up
```
## Environment variables

| Variable        | Default                          | Description                          |
|-----------------|----------------------------------|--------------------------------------|
| `DBG`           | `0`                              | Debug level passed to the backend    |
| `MODULE`        | `""`                             | Name of main YANG module             |

## Exposed ports

| Port  | Protocol | Purpose                        |
|-------|----------|--------------------------------|
| 80    | TCP      | RESTCONF (HTTP)                |
| 22    | TCP      | NETCONF over SSH               |
| 830   | TCP      | NETCONF (standard port)        |
| 9339  | TCP      | gRPC/gNMI                      |

## Accessing the container

### NETCONF over SSH

Default credentials: user `noc`, password `noc`.

```sh
ssh -p 2022 noc@localhost -s netconf
```

### SSH key authentication

To use a key instead of a password, mount your public key as `authorized_keys` inside the container:

```sh
sudo docker run -d \
  -v /usr/local/share/myyang:/usr/local/share/yang/main:ro \
  -v ~/.ssh/id_rsa.pub:/home/noc/.ssh/authorized_keys:ro \
  -p 2022:22 \
  clixon/generic
```

The startup script automatically sets the correct ownership (`noc:noc`) and permissions (`700`/`600`) on the `.ssh` directory and `authorized_keys` file, which sshd requires.

Connect without a password:

```sh
ssh -p 2022 -i ~/.ssh/id_rsa noc@localhost -s netconf
```

In docker-compose, add the mount under `volumes`:

```yaml
volumes:
  - ./config/clixon.xml:/usr/local/etc/clixon.xml:ro
  - ./yang/:/usr/local/share/yang/:ro
  - ~/.ssh/id_rsa.pub:/home/noc/.ssh/authorized_keys:ro
```

### Logs

```sh
sudo docker logs clixon
```
