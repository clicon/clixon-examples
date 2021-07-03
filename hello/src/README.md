# Clixon hello world example

  * [Content](#content)
  * [Compile and run](#compile)
  * [Using the CLI](#using-the-cli)
  * [Netconf](#netconf)	
  * [Restconf](#restconf)
  * [Next steps](#next-steps)
  
## Content

This directory contains a simple Clixon helloworld host example. It contains the following files:
* `hello.xml` the XML configuration file
* `clixon-hello@2019-04-17.yang <../yang/clixon-hello@2019-04-17.yang>`_: the YANG spec
* `hello_cli.cli`: the CLIgen spec
* `startup_db`: The startup datastore containing restconf port configuration
* `Makefile.in`: where plugins are built and installed

Notes:
hello.xml may not follow the way you set --prefix, --sysconfdir and others, you may need to hand-edit them to the values you used in the ./configure call. This could partly be acheived by using a hello.xml.in meta-file, but not quite since autotools uses variables not evaluated by XML.

For example, in hello.xml.in:
```
  <CLICON_CONFIGFILE>@sysconfdir@/hello.xml</CLICON_CONFIGFILE>
```
may evaluate to, in hello.xml:
```
  <CLICON_CONFIGFILE>${prefix}/etc/hello.xml</CLICON_CONFIGFILE>
```
where `${prefix}` is a variable that is not evaluated in XML. One could add a second shell evaluating step but it gets complex.

## Compile and run

Before you start,
* Make [group setup](https://github.com/clicon/clixon/blob/master/doc/FAQ.md#do-i-need-to-setup-anything)

```
    make && sudo make install
```
Start backend in the background:
```
    sudo clixon_backend -f /usr/local/etc/hello.xml -s startup
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
   olof@vandal> clixon_cli -f /usr/local/etc/hello.xml
   cli> set <?>
     hello                 
   cli> set hello world 
   cli> show configuration 
   hello world;
   cli> commit 
   cli> delete <?>
     all                   Delete whole candidate configuration
     hello                 
   cli> delete hello 
   cli> show configuration 
   cli> commit 
   cli> quit
   olof@vandal> 
```

## Netconf

Clixon also provides a Netconf interface. The following example starts a netconf client form the shell, adds the hello world config, commits it, and shows it:
```
   olof@vandal> clixon_netconf -qf /usr/local/etc/hello.xml
   <rpc><edit-config><target><candidate/></target><config><hello xmlns="urn:example:hello"><world/></hello></config></edit-config></rpc>]]>]]>
   <rpc-reply><ok/></rpc-reply>]]>]]>
   <rpc><commit/></rpc>]]>]]>
   <rpc-reply><ok/></rpc-reply>]]>]]>
   <rpc><get-config><source><running/></source></get-config></rpc>]]>]]>
   <rpc-reply><data><hello xmlns="urn:example:hello"><world/></hello></data></rpc-reply>]]>]]>
olof@vandal> 
```

## Restconf

Clixon also provides a Restconf interface. See [documentation on RESTCONF](https://clixon-docs.readthedocs.io/en/latest/restconf.html).

The example startup datastore contains config for a pre-configured restconf server listening on port 80. Edit `startup_db` if you want to change options or start the backend without it using `-s init` if you dont want restconf.

Send restconf commands (using Curl):
```
   olof@vandal> curl -X POST http://localhost/restconf/data -H "Content-Type: application/yang-data+json" -d '{"clixon-hello:hello":{"world":null}}'
   olof@vandal> curl -X GET http://localhost/restconf/data 
   {
     "data": {
       "clixon-hello:hello": {
         "world": null
       }
     }
   }
```

## Next steps

The hello world example only has a Yang spec and a template CLI
spec. For more advanced applications, customized backend, cli, netconf
and restconf code callbacks becomes necessary.

Further, you may want to add upgrade, RPC:s, state data, notification
streams, authentication and authorization. The [main example](../main)
contains examples for such capabilities.

There are also [container examples](../../docker) and lots more.


