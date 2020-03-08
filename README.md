# clixon-examples

Example clixon applications.

These are just empty OAM modules of some applications, there is no
backend semantics, nothing "happens" except you can run CLI or use
netconf and change the configured state. 

The examples are not complete but the docker images should work

- Hello world
  - [docker](hello/docker)
- Openconfig system from: [openconfig-system](https://github.com/openconfig/public)
  - [docker](openconfig/docker)
- 5G Network Resource Model (NRM) from 3GPP: [3GPP 5G](http://www.3gpp.org/ftp//Specs/archive/28_series/28.541/28541-g10.zip)
  - [docker](nrm/docker)

## Build and install:
```
> ./configure
> make
> sudo make install  # Install example applications locally
> make docker        # Build example docker containers
> make push          # Push docker containers to github (requires changing target)
```

See instructions on how to run the examples as locally installed or as docker containers [here](https://clixon-docs.readthedocs.io/en/latest/quickstart.html) or in the specific example documentation

