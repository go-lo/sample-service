Sample Service
==

This sample service provides an API that we can use to demonstrate go-lo. It provides an http-alike tcp listener that responds to anything sent with a 200- whether it's an http request or not.

## Building

```bash
$ make
gcc -pthread server.c -o service
```


## Running

This service runs on port 8080 and runs natively bypassing, where possible, abstraction layers like docker

```bash
$ ./service
Got a connection from 127.0.0.1 on port 57275
Got a connection from 127.0.0.1 on port 57276
Got a connection from 127.0.0.1 on port 57278
Got a connection from 127.0.0.1 on port 57281
Got a connection from 127.0.0.1 on port 57282
```

To test the service is up:

```bash
curl -kvvv http://localhost:8080/
*   Trying ::1...
* TCP_NODELAY set
* Connection failed
* connect to ::1 port 8080 failed: Connection refused
*   Trying 127.0.0.1...
* TCP_NODELAY set
* Connected to localhost (127.0.0.1) port 8080 (#0)
> GET / HTTP/1.1
> Host: localhost:8080
> User-Agent: curl/7.54.0
> Accept: */*
>
< HTTP/1.1 200 OK
< content-type: text/plain; charset=utf-8
< server: golo
< Date: Wed, 05 Sep 2018 11:03:48 GMT
< Content-Length: 12
<
* Connection #0 to host localhost left intact
Hello World!
```
