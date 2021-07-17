# Clixon wifi openconfig example

  * [Content](#content)
  * [Compile and run](#compile)
  * [Using the CLI](#using-the-cli)
  * [Netconf](#netconf)	
  * [Restconf](#restconf)
  
## Content

This directory contains a Clixon openconfig wifi host example. It contains the following files:
* `wifi.xml` the XML configuration file
* `clixon-wifi@2021-06-22<clixon-wifi@2021-06-22.yang>`_: a top-level cusomized YANG files importing relevant wifi config files
* `wifi_backend_plugin.c`: a backend plugin
* `wifi_cli.cli`: the CLIgen spec
* `startup_db`: The startup datastore containing restconf port configuration
* `Makefile.in`: where plugins are built and installed

Notes:
wifi.xml may not follow the way you set --prefix, --sysconfdir and others, you may need to hand-edit them to the values you used in the ./configure call. This could partly be acheived by using a wifi.xml.in meta-file, but not quite since autotools uses variables not evaluated by XML.

For example, in wifi.xml.in:
```
  <CLICON_CONFIGFILE>@sysconfdir@/wifi.xml</CLICON_CONFIGFILE>
```
may evaluate to, in wifi.xml:
```
  <CLICON_CONFIGFILE>${prefix}/etc/wifi.xml</CLICON_CONFIGFILE>
```
where `${prefix}` is a variable that is not evaluated in XML. One could add a second shell evaluating step but it gets complex.

## Compile and run

Before you start,
* Make [group setup](https://github.com/clicon/clixon/blob/master/doc/FAQ.md#do-i-need-to-setup-anything)
* Checkout openconfig: https://github.com/openconfig/public, this config assumes it to be checked out at: '/usr/local/share`

```
    make && sudo make install
```
Start backend in the background:
```
    sudo clixon_backend -f /usr/local/etc/wifi.xml -s startup
```
Note: use `-s init` instead if you want to start Clixon without the preconfigured restconf daemon

Start cli:
```
    clixon_cli
```

## Using the CLI

The example CLI allows you to modify and view the data model using `set`, `delete` and `show` via generated code.

The following example shows how to add a very simple configuration `hello world` using the generated CLI. The config is added to the candidate database, shown, committed to running, and then deleted.
```
   olof@vandal> clixon_cli -f /usr/local/etc/wifi.xml
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

## Netconf

Clixon also provides a Netconf interface. The following example starts a netconf client form the shell, adds the hello world config, commits it, and shows it:
```
olof@vandal> clixon_netconf -qf /usr/local/etc/wifi.xml
  <?xml version="1.0" encoding="UTF-8"?>
  <hello xmlns="urn:ietf:params:xml:ns:netconf:base:1.0">
     <capabilities><capability>urn:ietf:params:netconf:base:1.1</capability></capabilities>
   </hello>]]>]]>
  <rpc xmlns="urn:ietf:params:xml:ns:netconf:base:1.0"><get-config><source><running/></source></get-config></rpc>]]>]]>
  <rpc-reply xmlns="urn:ietf:params:xml:ns:netconf:base:1.0"><data><provision-aps xmlns="http://openconfig.net/yang/ap-manager"><provision-ap><mac>01:02:03:04:05:06</mac><config><mac>01:02:03:04:05:06</mac><hostname>ap1</hostname></config></provision-ap></provision-aps>...
olof@vandal> 
```

## Restconf

Clixon also provides a Restconf interface. See [documentation on RESTCONF](https://clixon-docs.readthedocs.io/en/latest/restconf.html).

The example startup datastore contains config for a pre-configured restconf server listening on port 80. Edit `startup_db` if you want to change options or start the backend without it using `-s init` if you dont want restconf.

Send restconf commands (using Curl):
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


