<div align="center">
  <img src="http://www.clicon.org/Clixon_logga_liggande_med-ikon.png" width="400">
</div>

# Examples

This repo consists of examples broken out from the [main clixon
repository](https://github.com/clicon/clixon). The intention is to
show how clixon can be used and submissions are welcome.

To start with, the examples are generated OAM modules from YANG, but
lacking backend semantics. You can configure, using generated CLI,
Netconf or Restconf, but nothing "happens".

The examples include a local install and docker build (which can run directly from dockerhub).

- [Hello world](hello)
- [Openconfig-system](openconfig) from: [openconfig-system](https://github.com/openconfig/public)
- [Openconfig-wifi](wifi) Openconfig wifi module
- [3GPP](3gpp): 5G Network Resource Model from [3GPP 5G](http://www.3gpp.org/ftp//Specs/archive/28_series/28.541/28541-g30.zip)

## Build and install:

### Single native installation example

Example: wifi host installation

```
> ./configure
> cd wifi/src
> make
> sudo make install  # Install example applications locally
```

### Single docker example

Example: wifi docker installation

```
> ./configure
> cd wifi/docker
> make docker
```

### All examples

Install all examples
```
> ./configure
> make 
> sudo make install  # Install example applications locally
> make docker        # Build example docker containers
> make push          # Push docker containers to github (requires changing target)
```

See instructions on how to run the examples as locally installed or as docker containers in [the clixon docs quick start](https://clixon-docs.readthedocs.io/en/latest/quickstart.html) or in the specific example documentation

Example of a plugin is in the [wifi example](wifi/src)
