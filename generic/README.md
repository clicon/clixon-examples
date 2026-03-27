# Generic Clixon Docker Container

A generic [clixon](https://github.com/clicon/clixon) container with no application-specific configuration baked in. YANG models and the clixon config file are provided as volume mounts at runtime, making the same image reusable across different applications.

## How it works

The image contains only the clixon binaries and runtime dependencies. At startup it reads a clixon config XML file from inside the container and starts the backend and SSH daemon. You supply:

- **Config file** — a clixon XML config file mounted into the container
- **YANG models** — a directory of `.yang` files mounted at a path referenced by your config

The clixon built-in YANG modules are always available at `/usr/local/share/clixon/`.

## Build

```sh
cd generic
make docker
```

Or directly:

```sh
sudo docker build -t clixon/generic . --no-cache
```

## Prepare host files

Create a directory layout like this on the host:

```
my-app/
  config/
    clixon.xml
  yang/
    my-model.yang
    ...
```

### Minimal `clixon.xml`

```xml
<clixon-config xmlns="http://clicon.org/config">
  <CLICON_CONFIGFILE>/usr/local/etc/clixon.xml</CLICON_CONFIGFILE>

  <!-- Built-in clixon yang modules (always present in the image) -->
  <CLICON_YANG_DIR>/usr/local/share/clixon</CLICON_YANG_DIR>
  <!-- Your yang models (mounted as a volume) -->
  <CLICON_YANG_DIR>/usr/local/share/yang</CLICON_YANG_DIR>

  <CLICON_YANG_MODULE_MAIN>my-model</CLICON_YANG_MODULE_MAIN>
  <CLICON_CLI_MODE>generic</CLICON_CLI_MODE>
  <CLICON_CLISPEC_DIR>/usr/local/lib/clixon/clispec</CLICON_CLISPEC_DIR>
  <CLICON_SOCK>/usr/local/var/clixon/clixon.sock</CLICON_SOCK>
  <CLICON_BACKEND_PIDFILE>/usr/local/var/clixon/clixon.pidfile</CLICON_BACKEND_PIDFILE>
  <CLICON_BACKEND_RESTCONF_PROCESS>true</CLICON_BACKEND_RESTCONF_PROCESS>
  <CLICON_XMLDB_DIR>/usr/local/var/clixon</CLICON_XMLDB_DIR>
  <CLICON_STARTUP_MODE>init</CLICON_STARTUP_MODE>
</clixon-config>
```

## Run

### With docker-compose

Edit `docker-compose.yml` to point the volume mounts at your host paths, then:

```sh
sudo docker-compose up
```

### With docker directly

```sh
sudo docker run -d \
  -v $(pwd)/config/clixon.xml:/usr/local/etc/clixon.xml:ro \
  -v $(pwd)/yang:/usr/local/share/yang:ro \
  -p 8080:80 \
  -p 2022:22 \
  -p 8830:830 \
  clixon/generic
```

### Override the config file path

If you prefer to mount the config at a different path inside the container, set the `CLIXON_CONFIG` environment variable:

```sh
sudo docker run -d \
  -e CLIXON_CONFIG=/mnt/myapp/config.xml \
  -v $(pwd)/config.xml:/mnt/myapp/config.xml:ro \
  -v $(pwd)/yang:/usr/local/share/yang:ro \
  clixon/generic
```

## Environment variables

| Variable        | Default                          | Description                          |
|-----------------|----------------------------------|--------------------------------------|
| `CLIXON_CONFIG` | `/usr/local/etc/clixon.xml`      | Path to the clixon config XML file   |
| `DBG`           | `0`                              | Debug level passed to the backend    |

## Exposed ports

| Port  | Protocol | Purpose                        |
|-------|----------|--------------------------------|
| 80    | TCP      | RESTCONF (HTTP)                |
| 22    | TCP      | NETCONF over SSH               |
| 830   | TCP      | NETCONF (standard port)        |

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
  -v $(pwd)/config/clixon.xml:/usr/local/etc/clixon.xml:ro \
  -v $(pwd)/yang:/usr/local/share/yang:ro \
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

### RESTCONF

```sh
curl http://localhost:8080/restconf/data
```

### Logs

```sh
sudo docker logs clixon
```

## Volume mount reference

| Host path (example)        | Container path                    | Purpose                         |
|----------------------------|-----------------------------------|---------------------------------|
| `./config/clixon.xml`      | `/usr/local/etc/clixon.xml`       | Clixon config file (default)    |
| `./yang/`                  | `/usr/local/share/yang/`          | Your YANG models                |

The container path for the config can be anything as long as `CLIXON_CONFIG` matches. The YANG mount path must match the `CLICON_YANG_DIR` entries in your config.
